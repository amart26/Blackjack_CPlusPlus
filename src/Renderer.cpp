#include "Renderer.h"

const float CARD_WIDTH = 88;
const float CARD_HEIGHT = 124;
const float CARD_GAP = 13;

SDL_FRect getCardSourceRect(const Card& card)
{
    int rankIndex = static_cast<int>(card.rank);
    int column = rankIndex % 5;
    int row = rankIndex / 5;
    float x = column * 88;
    float y = row * 124;
    return SDL_FRect{x, y, 88, 124};
}

void renderCard(SDL_Renderer* renderer, SDL_Texture* texture, const Card& card,
                float x, float y, float scale)
{
    SDL_FRect cardRect = getCardSourceRect(card);
    SDL_FRect destRect = {x, y, CARD_WIDTH * scale, CARD_HEIGHT * scale};
    SDL_RenderTexture(renderer, texture, &cardRect, &destRect);
}

void renderCardBack(SDL_Renderer* renderer, SDL_Texture* texture, float x,
                    float y, float scale)
{
    SDL_FRect cardBackRect = {0, 0, 88, 124};
    SDL_FRect cardBackDestRect = {x, y, CARD_WIDTH * scale,
                                  CARD_HEIGHT * scale};
    SDL_RenderTexture(renderer, texture, &cardBackRect, &cardBackDestRect);
}

void renderHand(SDL_Renderer* renderer, const Hand& hand, float x, float y,
                float scale, SDL_Texture* heartsTexture,
                SDL_Texture* clubsTexture, SDL_Texture* spadesTexture,
                SDL_Texture* diamondsTexture, SDL_Texture* cardBackTexture,
                bool isCardHidden)
{
    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (isCardHidden && i == hand.cards.size() - 1)
        {
            SDL_Log("RENDERING card back");
            renderCardBack(renderer, cardBackTexture,
                           x + (i * (CARD_WIDTH + CARD_GAP) * scale), y, scale);
        }
        else
        {
            switch (hand.cards[i].suit)
            {
            case Suit::HEARTS:
                renderCard(renderer, heartsTexture, hand.cards[i],
                           x + (i * (CARD_WIDTH + CARD_GAP) * scale), y, scale);
                break;

            case Suit::CLUBS:
                renderCard(renderer, clubsTexture, hand.cards[i],
                           x + (i * (CARD_WIDTH + CARD_GAP) * scale), y, scale);
                break;

            case Suit::SPADES:
                renderCard(renderer, spadesTexture, hand.cards[i],
                           x + (i * (CARD_WIDTH + CARD_GAP) * scale), y, scale);
                break;
            case Suit::DIAMONDS:
                renderCard(renderer, diamondsTexture, hand.cards[i],
                           x + (i * (CARD_WIDTH + CARD_GAP) * scale), y, scale);
                break;

            default:
                break;
            }
        }
    }
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
                float x, float y, SDL_Color color)
{
    float textW, textH;
    SDL_Surface* textSurface =
        TTF_RenderText_Blended(font, text.c_str(), 0, color);
    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_GetTextureSize(textTexture, &textW, &textH);
    SDL_FRect playerTextRect = {x, y, textW, textH};
    SDL_RenderTexture(renderer, textTexture, nullptr, &playerTextRect);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void renderButton(SDL_Renderer* renderer, float x, float y, float width,
                  float height, SDL_Color buttColor,
                  const std::string& buttonText, TTF_Font* font,
                  SDL_Color textColor)
{
    SDL_SetRenderDrawColor(renderer, buttColor.r, buttColor.g, buttColor.b,
                           buttColor.a);
    SDL_FRect buttonRect = {
        x,
        y,
        width,
        height,
    };
    SDL_RenderFillRect(renderer, &buttonRect);

    SDL_Surface* buttonTextSurface =
        TTF_RenderText_Blended(font, buttonText.c_str(), 0, textColor);
    SDL_Texture* buttonTextTexture =
        SDL_CreateTextureFromSurface(renderer, buttonTextSurface);

    float textW, textH;
    SDL_GetTextureSize(buttonTextTexture, &textW, &textH);

    float centerX = x + (width / 2);
    float centerY = y + (height / 2);
    float textRectX = centerX - (textW / 2);
    float textRectY = centerY - (textH / 2);

    SDL_FRect textRect = {textRectX, textRectY, textW, textH};
    SDL_RenderTexture(renderer, buttonTextTexture, nullptr, &textRect);

    SDL_DestroySurface(buttonTextSurface);
    SDL_DestroyTexture(buttonTextTexture);
}
