#pragma once

#include <SDL3/SDL.h>

struct Chip
{
    int value;
    float x, y;
    float width, height;
    float col, row;
};

void renderChip(SDL_Renderer* renderer, SDL_Texture* texture, const Chip& chip);

bool isMouseOverChip(const Chip& chip, float mouseX, float mouseY);
