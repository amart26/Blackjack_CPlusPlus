#pragma once

#include "Card.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

SDL_FRect getCardSourceRect(const Card& card);
void renderCard(SDL_Renderer* renderer, SDL_Texture* texture, const Card& card,
                float x, float y, float scale);
void renderCardBack(SDL_Renderer* renderer, SDL_Texture* texture, float x,
                    float y, float scale);
void renderHand(SDL_Renderer* renderer, const Hand& hand, float x, float y,
                float scale, SDL_Texture* heartsTexture,
                SDL_Texture* clubsTexture, SDL_Texture* spadesTexture,
                SDL_Texture* diamondsTexture, SDL_Texture* cardBackTexture,
                bool isCardHidden);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
                float x, float y, SDL_Color color);

void renderButton(SDL_Renderer* renderer, float x, float y, float width,
                  float height, SDL_Color buttColor,
                  const std::string& buttonText, TTF_Font* font,
                  SDL_Color textColor);
