#pragma once

#include "Card.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

SDL_FRect getCardSourceRect(const Card& card);
void renderCard(SDL_Renderer* renderer, SDL_Texture* texture, const Card& card,
                float x, float y);
void renderHand(SDL_Renderer* renderer, const Hand& hand, float y,
                SDL_Texture* heartsTexture, SDL_Texture* clubsTexture,
                SDL_Texture* spadesTexture, SDL_Texture* diamondsTexture);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
                float x, float y, SDL_Color color);