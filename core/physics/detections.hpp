#pragma once
#ifndef DETECTIONS_H
#define DETECTIONS_H

#include <SDL2/SDL.h>


class Detections
{
    public:
    static bool isColiding(SDL_Rect primary_rect, SDL_Rect target_rect);
    static Uint32 get_pixel(SDL_Surface *surface, int x, int y);
    static bool pixelPerfectCollision(SDL_Surface *aSurface, SDL_Rect aRect, SDL_Surface *bSurface, SDL_Rect bRect);
};

#endif