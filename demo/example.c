#include <SDL3/SDL.h>

static void draw_grid(SDL_Renderer *renderer, const int width, const int height) {
    SDL_SetRenderDrawColor(renderer, 40, 44, 52, 255);
    for (int x = 0; x < width; x += 40) {
        SDL_RenderLine(renderer, (float)x, 0.0f, (float)x, (float)height);
    }
    for (int y = 0; y < height; y += 40) {
        SDL_RenderLine(renderer, 0.0f, (float)y, (float)width, (float)y);
    }
}

static void draw_wave(SDL_Renderer *renderer, const int width, const int height, const float elapsed) {
    SDL_FPoint points[256];
    for (int i = 0; i < 256; i++) {
        points[i].x = (float)width * (float)i / 255.0f;
        points[i].y = (float)height * 0.8f + SDL_sinf((float)i * 0.08f + elapsed * 3.0f) * 40.0f;
    }
    SDL_SetRenderDrawColor(renderer, 97, 175, 239, 255);
    SDL_RenderLines(renderer, points, 256);
}

static void draw_palette(SDL_Renderer *renderer) {
    const SDL_Color colors[] = {
        { 224, 108, 117, 255 },
        { 229, 192, 123, 255 },
        { 152, 195, 121, 255 },
        { 86, 182, 194, 255 },
        { 198, 120, 221, 255 },
    };
    for (int i = 0; i < 5; i++) {
        const SDL_FRect rect = { 20.0f + (float)i * 50.0f, 60.0f, 40.0f, 40.0f };
        SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        SDL_RenderFillRect(renderer, &rect);
    }
}

static void draw_triangle(SDL_Renderer *renderer, const float center_x, const float center_y, const float angle) {
    const SDL_FColor colors[] = {
        { 1.0f, 0.3f, 0.3f, 1.0f },
        { 0.3f, 1.0f, 0.3f, 1.0f },
        { 0.3f, 0.3f, 1.0f, 1.0f },
    };
    SDL_Vertex vertices[3];
    for (int i = 0; i < 3; i++) {
        const float a = angle + (float)i * 2.0f * SDL_PI_F / 3.0f;
        vertices[i].position.x = center_x + SDL_cosf(a) * 80.0f;
        vertices[i].position.y = center_y + SDL_sinf(a) * 80.0f;
        vertices[i].color = colors[i];
        vertices[i].tex_coord.x = 0.0f;
        vertices[i].tex_coord.y = 0.0f;
    }
    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
}

static void draw_cursor(SDL_Renderer *renderer, const float x, const float y) {
    SDL_FPoint points[48];
    for (int i = 0; i < 48; i++) {
        const float a = (float)i * 2.0f * SDL_PI_F / 48.0f;
        points[i].x = x + SDL_cosf(a) * 16.0f;
        points[i].y = y + SDL_sinf(a) * 16.0f;
    }
    SDL_SetRenderDrawColor(renderer, 229, 192, 123, 255);
    SDL_RenderPoints(renderer, points, 48);
}

int main2(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("MyCoolGameEngine", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_FRect player = { 600.0f, 320.0f, 50.0f, 50.0f };
    const float speed = 300.0f;

    float elapsed = 0.0f;
    float fps = 0.0f;
    float fps_timer = 0.0f;
    int fps_frames = 0;

    Uint64 previous = SDL_GetTicksNS();
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        const Uint64 now = SDL_GetTicksNS();
        const float dt = (float)(now - previous) / 1e9f;
        previous = now;
        elapsed += dt;

        fps_timer += dt;
        fps_frames++;
        if (fps_timer >= 0.5f) {
            fps = (float)fps_frames / fps_timer;
            fps_timer = 0.0f;
            fps_frames = 0;
        }

        const bool *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) {
            player.y -= speed * dt;
        }
        if (keys[SDL_SCANCODE_S]) {
            player.y += speed * dt;
        }
        if (keys[SDL_SCANCODE_A]) {
            player.x -= speed * dt;
        }
        if (keys[SDL_SCANCODE_D]) {
            player.x += speed * dt;
        }

        float mouse_x, mouse_y;
        const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

        int width, height;
        SDL_GetRenderOutputSize(renderer, &width, &height);

        SDL_SetRenderDrawColor(renderer, 24, 26, 32, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer, width, height);
        draw_wave(renderer, width, height, elapsed);
        draw_palette(renderer);
        draw_triangle(renderer, (float)width - 160.0f, 200.0f, elapsed);

        const SDL_FRect glass = { 180.0f, 40.0f, 200.0f, 120.0f };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 60);
        SDL_RenderFillRect(renderer, &glass);

        const float player_center_x = player.x + player.w / 2.0f;
        const float player_center_y = player.y + player.h / 2.0f;
        SDL_SetRenderDrawColor(renderer, 171, 178, 191, 255);
        SDL_RenderLine(renderer, player_center_x, player_center_y, mouse_x, mouse_y);

        if (buttons & SDL_BUTTON_LMASK) {
            SDL_SetRenderDrawColor(renderer, 224, 108, 117, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 152, 195, 121, 255);
        }
        SDL_RenderFillRect(renderer, &player);

        const SDL_FRect outline = { player.x - 6.0f, player.y - 6.0f, player.w + 12.0f, player.h + 12.0f };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &outline);

        draw_cursor(renderer, mouse_x, mouse_y);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 20.0f, 20.0f, "WASD - move, mouse - aim, LMB - color, Esc - quit");
        SDL_RenderDebugTextFormat(renderer, 20.0f, 36.0f, "fps: %.0f", fps);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
