// #define SDL_MAIN_HANDLED // Avoid SDL from defining main() for you

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "core/window/Window.hpp"
#include "games/flappy_bird/flappy.hpp"

#ifdef _WIN32
#include <include/SDL.h>
#include <include/SDL_ttf.h>
#include <include/SDL_mixer.h>
#include <include/SDL_image.h>
#include <Windows.h>
#endif

// int main(int argc, char *argv[]) for console

// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;

        return -1;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init could not be initialized! SDL_Error " << std::endl;
        SDL_Quit();
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_Init(IMG_INIT_PNG))
    {
        std::cerr << "SDL_INIT_VIDEO could not be initialized! SDL_Error " << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_mixer with format and channels
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    const int win_width = 600;
    const int win_height = 400;

    int resize_win_width;
    int resize_win_height;

    SDL_Event event;

    bool isRunning = true;


    // SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_POPUP_MENU | SDL_WINDOW_RESIZABLE);
    // Access the global SDL_Window*
    SDL_Window *window = Window::get_window();
    if (!window)
    {
        std::cerr << "Window is null!" << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // get the window height and width on resize
    SDL_GetWindowSize(window, &resize_win_width, &resize_win_height);
    const int targetFPS = 90;                   // Target FPS
    const int frameDuration = 1000 / targetFPS; // Duration of one frame in milliseconds
    int frameTime;
    Uint32 lastFrameTime = SDL_GetTicks();

    FlappyBird bird(renderer, window);

    while (isRunning)
    {
        Uint32 frameStart = SDL_GetTicks();                       // Capture the start time of the frame
        float deltaTime = (frameStart - lastFrameTime) / 1000.0f; // Calculate deltaTime in seconds
        lastFrameTime = frameStart;                               // Update for the next frame

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
            bird.Controls(renderer, event, window, lastFrameTime);
        }

        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        bird.Draw(renderer, window, lastFrameTime);


        SDL_RenderPresent(renderer);
        // Cap the frame rate to 60 FPS
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDuration > frameTime)
        {
            SDL_Delay(frameDuration - frameTime);
        }
    }

    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}