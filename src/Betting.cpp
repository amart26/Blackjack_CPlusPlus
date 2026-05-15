#include "Betting.h"

void renderChip(SDL_Renderer* renderer, SDL_Texture* texture, const Chip& chip)
{
    SDL_FRect chipRect = {(float)chip.col * 64, (float)chip.row * 72, 64, 72};
    SDL_FRect chipDestRect = {chip.x, chip.y, chip.width, chip.height};
    SDL_RenderTexture(renderer, texture, &chipRect, &chipDestRect);
}

bool isMouseOverChip(const Chip& chip, float mouseX, float mouseY)
{
    if (mouseX >= chip.x && mouseX <= chip.x + chip.width && mouseY >= chip.y &&
        mouseY <= chip.y + chip.height)
    {
        return true;
    }
    else
    {
        return false;
    }
}
