//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_H
#define MYCOOLGAMEENGINE_RENDERER_H

struct renderer;
struct subsystem_collection;

struct subsystem* renderer_create(const struct subsystem_collection* subsystems);

struct renderer_pipeline* renderer_get_pipeline(const struct renderer* this);
struct renderer_layer_manager* renderer_get_layer_manager(const struct renderer* this);

#endif //MYCOOLGAMEENGINE_RENDERER_H
