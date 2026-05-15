#include "Renderer.h"

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
                float x, float y)
{
    SDL_FRect cardRect = getCardSourceRect(card);
    SDL_FRect destRect = {x, y, 88, 124};
    SDL_RenderTexture(renderer, texture, &cardRect, &destRect);
}

void renderHand(SDL_Renderer* renderer, const Hand& hand, float y,
                SDL_Texture* heartsTexture, SDL_Texture* clubsTexture,
                SDL_Texture* spadesTexture, SDL_Texture* diamondsTexture)
{
    for (int i = 0; i < hand.cards.size(); i++)
    {

        switch (hand.cards[i].suit)
        {
        case Suit::HEARTS:
            renderCard(renderer, heartsTexture, hand.cards[i], 200 + (i * 100),
                       y);
            break;

        case Suit::CLUBS:
            renderCard(renderer, clubsTexture, hand.cards[i], 200 + (i * 100),
                       y);
            break;

        case Suit::SPADES:
            renderCard(renderer, spadesTexture, hand.cards[i], 200 + (i * 100),
                       y);
            break;
        case Suit::DIAMONDS:
            renderCard(renderer, diamondsTexture, hand.cards[i],
                       200 + (i * 100), y);
            break;

        default:
            break;
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
