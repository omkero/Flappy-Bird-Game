#include "flappy.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <random>

#ifdef _WIN32
#include <include/SDL_events.h>
#include <include/SDL_keycode.h>
#include <include/SDL_render.h>
#include <include/SDL_scancode.h>
#include <include/SDL_image.h>
#include <include/SDL_stdinc.h>
#include <include/SDL_timer.h>
#include <include/SDL_video.h>
#endif

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}

bool pixelPerfectCollision(SDL_Surface *aSurface, SDL_Rect aRect, SDL_Surface *bSurface, SDL_Rect bRect) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&aRect, &bRect, &intersection))
        return false;

    for (int y = 0; y < intersection.h; y++) {
        for (int x = 0; x < intersection.w; x++) {
            int ax = intersection.x - aRect.x + x ;
            int ay = intersection.y - aRect.y + y;

            int bx = intersection.x - bRect.x + x;
            int by = intersection.y - bRect.y + 5 + y;

            Uint32 aPixel = get_pixel(aSurface, ax, ay);
            Uint32 bPixel = get_pixel(bSurface, bx, by);

            Uint8 aAlpha, bAlpha;
            SDL_GetRGBA(aPixel, aSurface->format, nullptr, nullptr, nullptr, &aAlpha);
            SDL_GetRGBA(bPixel, bSurface->format, nullptr, nullptr, nullptr, &bAlpha);

            if (aAlpha > 0 && bAlpha > 0) {
                return true;
            }
        }
    }
    return false;
}


FlappyBird::FlappyBird(SDL_Renderer *renderer, SDL_Window *window) {
    int window_width;
    int window_height;

    SDL_GetWindowSize(window,  &window_width, &window_height);

    bird_rect.h = bird_size;
    bird_rect.w = bird_size;
    bird_rect.x = (window_width / 2) - bird_rect.w ;
    bird_rect.y = (window_height / 2) - (bird_rect.h / 2);

    // load bird textures and surface
    bird_surface = IMG_Load("assets/textures/flappy/bird.png");
    if (!bird_surface) {
        std::cerr << "something went wrong with laoding bird_surface" << std::endl;
        SDL_Quit();
    }

    bird_texture = SDL_CreateTextureFromSurface(renderer, bird_surface);
    if (!bird_texture) {
        std::cerr << "something went wrong with creating bird_texture" << std::endl;
        SDL_Quit();
    }


    floor_rect_1.y = window_height - floor_height + 6;
    floor_rect_1.x = -2;
    floor_rect_1.w = window_width + 6;
    floor_rect_1.h = floor_height;

    floor_rect_2.y = window_height - floor_height + 6;
    floor_rect_2.x = floor_rect_1.x + floor_rect_1.w - 9;;
    floor_rect_2.w = window_width + 6;
    floor_rect_2.h = floor_height;

    // load floor textures and surface
    floor_surface = IMG_Load("assets/textures/flappy/floor_1.png");
    if (!bird_surface) {
        std::cerr << "something went wrong with laoding floor_surface" << std::endl;
        SDL_Quit();
    }
    
    floor_texture = SDL_CreateTextureFromSurface(renderer, floor_surface);
    if (!floor_texture) {
        std::cerr << "something went wrong with creating floor_texture" << std::endl;
        SDL_Quit();
    }

    cloud_rect_one.y = 30;
    cloud_rect_one.x = 600 + cloud_width;
    cloud_rect_one.h = cloud_height;
    cloud_rect_one.w = cloud_width;

    // load cloud textures and surface
    cloud_surface = IMG_Load("assets/textures/flappy/clouds.png");
    if (!bird_surface) {
        std::cerr << "something went wrong with laoding cloud_surface" << std::endl;
        SDL_Quit();
    }
    
    cloud_texture = SDL_CreateTextureFromSurface(renderer, cloud_surface);
    if (!floor_texture) {
        std::cerr << "something went wrong with creating cloud_texture" << std::endl;
        SDL_Quit();
    }

    pipe_rect.x = 305;
    pipe_rect.y = 0;
    pipe_rect.h = pipe_height;
    pipe_rect.w = pipe_width;

        // load cloud textures and surface
    pipe_surface = IMG_Load("assets/textures/flappy/pipe.png");
    if (!pipe_surface) {
        std::cerr << "something went wrong with laoding cloud_surface" << std::endl;
        SDL_Quit();
    }
        
    pipe_texture = SDL_CreateTextureFromSurface(renderer, pipe_surface);
    if (!pipe_texture) {
        std::cerr << "something went wrong with creating cloud_texture" << std::endl;
        SDL_Quit();
    }

    forest_rect.x = 0;
    forest_rect.y = 0;
    forest_rect.h = window_height;
    forest_rect.w = window_width;

    // load cloud textures and surface
    forest_surface = IMG_Load("assets/textures/flappy/forest.png");
    if (!pipe_surface) {
        std::cerr << "something went wrong with laoding cloud_surface" << std::endl;
        SDL_Quit();
    }
                
    forest_texture = SDL_CreateTextureFromSurface(renderer, forest_surface);
    if (!forest_texture) {
        std::cerr << "something went wrong with creating cloud_texture" << std::endl;
        SDL_Quit();
    }
    over_rect.h = over_height;
    over_rect.w = over_width;
    over_rect.x = (window_width / 2) - (over_width / 2);
    over_rect.y = (window_height / 2) - (over_height / 2);

    // load cloud textures and surface
    over_surface = IMG_Load("assets/textures/flappy/game_over.png");
    if (!over_surface) {
        std::cerr << "something went wrong with laoding cloud_surface" << std::endl;
        SDL_Quit();
    }
                
    over_texture = SDL_CreateTextureFromSurface(renderer, over_surface);
    if (!over_texture) {
        std::cerr << "something went wrong with creating cloud_texture" << std::endl;
        SDL_Quit();
    }
    // load the font


    text_rect.x = 30;
    text_rect.y = 30;
    text_rect.h = 20;
    text_rect.w = 20;


    text_font = TTF_OpenFont("assets/fonts/RobotoMedium.ttf", 30);
    if (!text_font)
    {
        std::cerr << "Error cannot open or fin ttf file." << std::endl;
        throw std::runtime_error("Error cannot open or fin ttf file.");
        return;
    }

    text_surface = TTF_RenderText_Blended(text_font, std::to_string(this->scoore).c_str(), text_color);
    if (!text_surface)
    {
        std::cerr << "Error cannot init text_surface" << std::endl;
        throw std::runtime_error("Error cannot init text_surface");
        return;
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_surface)
    {
        std::cerr << "Error cannot init text_texture" << std::endl;
        throw std::runtime_error("Error cannot init text_texture");
        SDL_FreeSurface(text_surface);
        return;
    }
    

    last_creation_time = std::chrono::steady_clock::now();
};

void FlappyBird::Draw(SDL_Renderer *renderer, SDL_Window *window, Uint32 lastFrameTime) {
    int window_height;
    int window_width;
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // the default pos
    text_rect.h = text_surface->h;
    text_rect.w = text_surface->w;
  

    SDL_GetWindowSize(window, &window_width, &window_height);

    // backgound color
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);


     // forest
     SDL_RenderCopyEx(renderer, forest_texture, NULL, &forest_rect, 0, nullptr, SDL_FLIP_NONE);

    // clouds
    SDL_RenderCopyEx(renderer, cloud_texture, NULL, &cloud_rect_one, 0, nullptr, SDL_FLIP_NONE);
   
    // Render floors
    SDL_RenderCopyEx(renderer, floor_texture, NULL, &floor_rect_1, 0, nullptr, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, floor_texture, NULL, &floor_rect_2, 0, nullptr, SDL_FLIP_NONE);

    auto current_time = std::chrono::steady_clock::now();
    auto time_since_last_creation = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_creation_time).count();
    if (time_since_last_creation >= 1500)
    {
        activate_gravity = true;
        std::random_device seeder;
        std::mt19937 engine(seeder());
        
        std::uniform_int_distribution<int> dist_h_top(100, 320);
        int rand_height_top = dist_h_top(engine);
        
        std::uniform_int_distribution<int> dist_gap(100, 155); // based on difficulty
        int gap = dist_gap(engine);
        
        int rand_height_bottom = 600 - rand_height_top - gap; // height is 600

        if  (!is_game_over) {
            text_surface = TTF_RenderText_Blended(text_font, std::to_string(this->scoore).c_str(), text_color);
            text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            scoore += 1;
        }
        this->GeneratePipes(renderer, rand_height_top, pipe_width, 180, 1300, 0); // top pipe
        this->GeneratePipes(renderer, rand_height_bottom, pipe_width, 0, 1300, rand_height_top + gap);
   
        last_creation_time = current_time;
    }

    for (auto& pipe : pipes) {
        // pipe 
        SDL_RenderCopyEx(renderer, pipe_texture, NULL, &pipe.pipe, pipe.angle, nullptr, SDL_FLIP_NONE);
        pipe.pipe.x -= global_movment;
    }
    for (auto& pipe : pipes) {
        if (pixelPerfectCollision(bird_surface, bird_rect, pipe_surface, pipe.pipe)) {
            global_movment = 0;
            cloud_movment = 0;
            velocity = 0;
            is_game_over = true;
           // bird_angle = 0;
        }
        if (pipe.pipe.x <= 0 - pipe.pipe.w) {
            pipes.erase(pipes.begin());;
        }
    }
    // floor

    // Move floors left nad cloud
    floor_rect_1.x -= global_movment;
    floor_rect_2.x -= global_movment;
    cloud_rect_one.x -= cloud_movment;

    if (cloud_rect_one.x < 0 - cloud_rect_one.w) {
        cloud_rect_one.x = 700 + cloud_rect_one.w;
    }

    // Reset if floor goes off screen
    if (floor_rect_1.x + floor_rect_1.w <= 0) {
      floor_rect_1.x = floor_rect_2.x + floor_rect_2.w - 9;
    }
    if (floor_rect_2.x + floor_rect_2.w <= 0) {
      floor_rect_2.x = floor_rect_1.x + floor_rect_1.w - 9;
    }

    if (bird_rect.y <= 0 - bird_rect.h) {
        is_game_over = true;
    }

    // bird
    SDL_RenderCopyEx(renderer, bird_texture, NULL, &bird_rect, bird_angle, nullptr, SDL_FLIP_NONE);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    if (is_game_over) {
        SDL_RenderCopyEx(renderer, over_texture, NULL, &over_rect, 0, nullptr, SDL_FLIP_NONE);
    }

   this->Jump(renderer, window);

   if (pixelPerfectCollision(bird_surface, bird_rect, floor_surface, floor_rect_1)) {
    velocity = 0;
    bird_angle = 0;
    global_movment = 0;
    cloud_movment = 0;

    is_game_over = true;
   }
   if (pixelPerfectCollision(bird_surface, bird_rect, floor_surface, floor_rect_2)) {
    velocity = 0;
    bird_angle = 0;
    global_movment = 0;
    cloud_movment = 0;
    is_game_over = true;
   }

}

void FlappyBird::Controls(SDL_Renderer *renderer,  SDL_Event &e, SDL_Window *window, Uint32 lastFrameTime) {
    if (!is_game_over) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP) {
            activate_gravity = true;
            velocity = jump_strength; // apply upward velocity
            bird_angle = -45.0;       // tilt the bird up
        }
    }
    if (e.type == SDL_KEYDOWN) {
       if (e.key.keysym.sym == SDLK_SPACE && is_game_over) {
         this->PlayAgain(renderer, window);
       }
    }
 }


 void FlappyBird::PlayAgain(SDL_Renderer *renderer, SDL_Window *window) {
    int window_height; 
    int window_width;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // reset everything
    pipes.clear();
    is_game_over = false;
    activate_gravity = false;
    global_movment = 2;
    cloud_movment = 1;
    bird_angle = 0;
    bird_rect.x = (window_width / 2) - bird_rect.w ;
    bird_rect.y = (window_height / 2) - (bird_rect.h / 2);

    scoore = 0;
    text_surface = TTF_RenderText_Blended(text_font, std::to_string(this->scoore).c_str(), text_color);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

}

 void FlappyBird::Jump(SDL_Renderer *renderer, SDL_Window *window) {
    if (activate_gravity) {
        velocity += gravity; // gravity pulls down
        bird_rect.y += static_cast<int>(velocity); // apply vertical movement
    
        if (bird_angle < 45.0) {
            bird_angle += 2.0; // tilts downward gradually
        }
    }
 }

 void FlappyBird::GeneratePipes(SDL_Renderer *renderer, int height, int width, float angel, int x, int y) {
    SDL_Rect pipe_rect;
    pipe_rect.x = x;
    pipe_rect.y = y;
    pipe_rect.h = height;
    pipe_rect.w = width;

     PipeVec pipe = {
        pipe_rect,
        angel,
     };

     pipes.push_back(pipe);
 }