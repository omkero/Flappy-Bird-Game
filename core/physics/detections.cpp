#include "detections.hpp"

bool Detections::isColiding(SDL_Rect primary_rect, SDL_Rect target_rect) {
    // Check if there is no overlap (non-collision)
    if (primary_rect.x + primary_rect.w <= target_rect.x ||  // primary is left of target
        target_rect.x + target_rect.w <= primary_rect.x ||  // target is left of primary
        primary_rect.y + primary_rect.h <= target_rect.y || // primary is above target
        target_rect.y + target_rect.h <= primary_rect.y) {  // target is above primary
        return false; // No collision
    }
    return true; // There is a collision
}
Uint32 Detections::get_pixel(SDL_Surface *surface, int x, int y) {
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

bool Detections::pixelPerfectCollision(SDL_Surface *aSurface, SDL_Rect aRect, SDL_Surface *bSurface, SDL_Rect bRect) {
    SDL_Rect intersection;
    if (!SDL_IntersectRect(&aRect, &bRect, &intersection))
        return false;

    for (int y = 0; y < intersection.h; y++) {
        for (int x = 0; x < intersection.w; x++) {
            int ax = intersection.x - aRect.x + x;
            int ay = intersection.y - aRect.y + y;

            int bx = intersection.x - bRect.x + x;
            int by = intersection.y - bRect.y + y;

            Uint32 aPixel = Detections::get_pixel(aSurface, ax, ay);
            Uint32 bPixel = Detections::get_pixel(bSurface, bx, by);

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