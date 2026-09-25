//
// Created by nikita on 24.09.2026.
//

#include "engine.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "engine_execution_context.h"
#include "engine_init_context.h"
#include "../interpreter/interpreter.h"
#include "update_context.h"
#include "../utils/action.h"
#include "events/engine_events.h"
#include "version.h"
#include "../subsystems/subsystem_collection.h"

struct engine {
    /*
     * TODO
     *
     * Мне не очень нравится что тут получилось.
     *
     * Много шаблонного кода.
     *
     * Большой шанс где-то забыть позвать create / awake / disable / destroy или перепутать их порядок.
     *
     * Семантика init_context execution_context мне кажется достаточно высосанная.
     *
     * Семантикой init_context execution_context начинают пользоваться объекты низко по дереву хотя это возможно внутренняя деталь реализации движка.
     *
     * Объекты, которым нужен лишь определенный набор подсистем, вынужденны включать заголовки,
     * где содержатся упоминания огромного количества нерелевантных подсистем (пусть хоть и через forward declaration).
     * Например, этому файлу нужен только интерпретатор и события. Но инклуды сверху сейчас сюда копируют упоминания клавиатуры,
     * сцены и огромного количества ненужных тут подсистем. Да, это не влияет на время компиляции из-за forward_declaration, но
     * с точки зрения проектирования, как мне кажется, это нежелательная связанность.
     *
     * Может быть это надо переписать.
     *
     * Наверное стоит рассмотреть вариант переписать это по тому паттерну что используется для компонентов сущностей сцен.
     * Т.е. engine_subsystem - базовый класс с виртуальным ключом подсистемы, on_awake, on_disable, on_destroy,
     * engine_subsystem_collection - класс-коллекция подсистем.
     * А вместо вручную написанных геттеров писать что-то вроде
     * struct engine_subsystem* engine_subsystem_collection_get(struct engine_subsystem_collection *this, const char* subsystem_key)
     * с последующим кастом указателя.
     * Мне кажется что такой подход чем-то похож на бины в спринге.
     * Если выносить запрос к подсистемам на awake (кеширование на awake стадартная практика),
     * то при неверном ключе программа будет падать хоть и в рантайме, но на самом ее старте.
     */
    struct subsystem_collection *subsystems;
    struct engine_init_context *init_context;
    struct engine_execution_context *execution_context;
};

struct engine* engine_create(struct engine_init_arguments arguments) {
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    logger_info("Engine is creating...");
    struct engine *this = malloc(sizeof(struct engine));
    this->subsystems = subsystem_collection_create();
    this->init_context = engine_init_context_try_create(this, arguments);
    if (this->init_context == NULL) {
        free(this);
        return NULL;
    }
    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");
    subsystem_collection_destroy(this->subsystems);
    engine_init_context_destroy(this->init_context);
    free(this);
}

static bool engine_execute_step(struct engine *this, const struct engine_execution_arguments arguments) {
    if (arguments.script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return 0;
    }
    this->execution_context = engine_execution_context_create(this, arguments);

    const struct interpreter* interpreter = engine_init_context_get_interpreter(this->init_context);
    if (interpreter_eval(interpreter, arguments.script_path) != INTERPRETER_OK) {
        engine_execution_context_destroy(this->execution_context);
        return 0;
    }

    engine_init_context_awake(this->init_context);
    engine_execution_context_awake(this->execution_context);

    struct update_context update_context = {
        .dt = 0
    };
    Uint64 previous = SDL_GetTicksNS();

    struct engine_events* events = engine_init_context_get_events(this->init_context);
    while (engine_execution_context_is_running(this->execution_context)) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        action_invoke(engine_events_pre_frame(events), &update_context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            action_invoke(engine_events_on_native_event(events), &event);
        }

        action_invoke(engine_events_pre_physics(events), &update_context);
        action_invoke(engine_events_on_physics(events), &update_context);
        action_invoke(engine_events_post_physics(events), &update_context);

        action_invoke(engine_events_pre_rendering(events), &update_context);
        action_invoke(engine_events_on_rendering(events), &update_context);
        action_invoke(engine_events_post_rendering(events), &update_context);
    }

    const bool rerun_required = engine_execution_context_if_rerun_required(this->execution_context);

    engine_execution_context_disable(this->execution_context);
    engine_execution_context_destroy(this->execution_context);
    engine_init_context_disable(this->init_context);

    return rerun_required;
}

void engine_execute(struct engine *this, const struct engine_execution_arguments arguments) {
    while (engine_execute_step(this, arguments)) {}
}

struct engine_init_context* engine_get_init_context(const struct engine *this) {
    return this->init_context;
}
struct engine_execution_context* engine_get_execution_context(const struct engine *this) {
    return this->execution_context;
}