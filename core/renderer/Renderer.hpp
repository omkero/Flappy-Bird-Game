#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

#ifdef WIN_32
#include "include/SDL.h"
#endif

class Renderer
{
public:
    // Access the renderer singleton instance
    static Renderer &get_instance();

    static SDL_Renderer *get_renderer();

private:
    Renderer();  // Constructor is private
    ~Renderer(); // Destructor

    Renderer(const Renderer &) = delete;            // Prevent copy construction
    Renderer &operator=(const Renderer &) = delete; // Prevent assignment

    // original renderer
    SDL_Renderer* renderer;
};

#endif