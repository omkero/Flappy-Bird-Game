#ifndef FLAPPY_H
#define FLAPPY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <chrono>
#include <vector>
#include <SDL2/SDL_ttf.h>

struct PipeVec {
    SDL_Rect pipe;
    float angle;
};

class FlappyBird {
    public:
    FlappyBird(SDL_Renderer *renderer, SDL_Window *window);
    void Draw(SDL_Renderer *renderer, SDL_Window *window, Uint32 lastFrameTime);
    void Jump(SDL_Renderer *renderer, SDL_Window *window);
    void GeneratePipes(SDL_Renderer *renderer,int height, int width, float angel, int x, int y);
    void Controls(SDL_Renderer *renderer, SDL_Event &event, SDL_Window *window, Uint32 lastFrameTime);
    void PlayAgain(SDL_Renderer *renderer,SDL_Window *window);

    private:
    int scoore = 0;

    bool activate_gravity = false;
    bool is_game_over = false;
    int global_movment = 2;
    int cloud_movment = 1;
    std::chrono::steady_clock::time_point last_creation_time;

    // bird
    float velocity = 0.0f;
    float gravity = 0.4f;
    float jump_strength = -7.0f;
    float bird_angle = 0;
    float bird_jump_speed = 5.00f;
    int bird_size = 70;
    int bird_x = 30;
    int bird_y = 300;
    SDL_Texture *bird_texture;
    SDL_Surface *bird_surface;
    SDL_Color bird_Color = {255, 255, 255, 255}; // white color
    SDL_Rect bird_rect;


    // floor
    SDL_Texture *floor_texture;
    SDL_Surface *floor_surface;
    SDL_Color floor_Color = {127, 186, 93, 255}; // white color
    int floor_height = 100;
    int floor_width;
    SDL_Rect floor_rect_1;
    SDL_Rect floor_rect_2;
    
    // cloud
    SDL_Texture *cloud_texture;
    SDL_Surface *cloud_surface;
    int cloud_height = 90;
    int cloud_width = 200;
    SDL_Rect cloud_rect_one;

    // pipes
    std::vector<PipeVec> pipes;
    SDL_Texture *pipe_texture;
    SDL_Surface *pipe_surface;
    float pipe_angle = 180;
    int pipe_height = 230;
    int pipe_width = 90;
    SDL_Rect pipe_rect;

    SDL_Texture *forest_texture;
    SDL_Surface *forest_surface;
    SDL_Rect forest_rect;

    SDL_Texture *over_texture;
    SDL_Surface *over_surface;
    SDL_Rect over_rect;
    int over_height = 400;
    int over_width = 430;

    SDL_Color text_color = {255, 255, 255};
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    TTF_Font *text_font;
    SDL_Rect text_rect;
};

#endif