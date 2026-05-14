#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

enum class GameState
{
    PLAYER_TURN,
    DEALER_TURN,
    GAME_OVER
};

enum class Suit
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum class Rank
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

struct Card
{
    Suit suit;
    Rank rank;
};

struct Deck
{
    std::vector<Card> cards;
};

struct Hand
{
    std::vector<Card> cards;
};

void fillDeck(Deck& deck)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            deck.cards.push_back(
                Card{static_cast<Suit>(i), static_cast<Rank>(j)});
        }
    }
}

void shuffleDeck(Deck& deck)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(deck.cards.begin(), deck.cards.end(), gen);
}

void dealCard(Deck& deck, Hand& hand)
{
    hand.cards.push_back(deck.cards.back());
    deck.cards.pop_back();
}

int calculateScore(const Hand& hand)
{
    int sum = 0;
    int aceCount = 0;
    for (int i = 0; i < hand.cards.size(); i++)
    {
        switch (hand.cards[i].rank)
        {
        case Rank::JACK:
            sum += 10;
            break;
        case Rank::QUEEN:
            sum += 10;
            break;
        case Rank::KING:
            sum += 10;
            break;
        case Rank::ACE:
            aceCount++;
            sum += 11;
            break;
        default:
            sum += static_cast<int>(hand.cards[i].rank) + 1;
            break;
        }
    }

    while (sum > 21 && aceCount != 0)
    {
        aceCount--;
        sum -= 10;
    }
    return sum;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Log("SDL initialized successfully");
    SDL_Window* window = SDL_CreateWindow("Blackjack", 800, 600, 0);
    SDL_Log("Window created successfully");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);

    TTF_Font* font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 24);
    TTF_Font* smallFont = TTF_OpenFont("assets/fonts/OpenSans.ttf", 16);
    if (font == nullptr)
    {
        SDL_Log("Font failed to laod: %s", SDL_GetError());
    }

    Deck deck;
    Hand playerHand;
    Hand dealerHand;

    fillDeck(deck);
    shuffleDeck(deck);
    dealCard(deck, dealerHand);
    dealCard(deck, dealerHand);

    dealCard(deck, playerHand);
    dealCard(deck, playerHand);

    GameState gameState = GameState::PLAYER_TURN;
    int playerScore = calculateScore(playerHand);
    int dealerScore = calculateScore(dealerHand);

    std::cout << "Players score: " << playerScore << "\n";
    std::cout << "Dealers score " << dealerScore << "\n";
    bool running = true;
    bool isDealerDone = false;
    bool isPlayerBusted = false;
    bool isDealerBusted = false;
    bool isGameOverHandled = false;
    while (running)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            float mouseX = event.button.x;
            float mouseY = event.button.y;

            if (mouseX >= 300 && mouseX <= 380 && mouseY >= 500 &&
                mouseY <= 540)
            {
                std::cout << "HIT!\n";
                dealCard(deck, playerHand);
                playerScore = calculateScore(playerHand);
                std::cout << playerScore << "\n";

                if (playerScore > 21)
                {
                    isPlayerBusted = true;
                    gameState = GameState::GAME_OVER;
                    std::cout << "BUST! GAME OVER!\n";
                }

                if (playerScore == 21)
                {
                    gameState = GameState::GAME_OVER;
                    std::cout << "BlACKJACK!\n";
                }
            }

            if (mouseX >= 420 && mouseX <= 500 && mouseY >= 500 &&
                mouseY <= 540)
            {
                std::cout << "STAND!, DEALERS TURN!\n";
                gameState = GameState::DEALER_TURN;
            }

            if (gameState == GameState::GAME_OVER && mouseX >= 360 &&
                mouseX <= 420 && mouseY >= 260 && mouseY <= 300)
            {
                playerHand.cards.clear();
                dealerHand.cards.clear();
                deck.cards.clear();
                fillDeck(deck);
                shuffleDeck(deck);

                dealCard(deck, playerHand);
                dealCard(deck, dealerHand);

                dealCard(deck, playerHand);
                dealCard(deck, dealerHand);

                playerScore = calculateScore(playerHand);
                dealerScore = calculateScore(dealerHand);

                isDealerDone = false;
                isPlayerBusted = false;
                isDealerBusted = false;
                isGameOverHandled = false;

                gameState = GameState::PLAYER_TURN;
            }
        }

        SDL_SetRenderDrawColorFloat(renderer, 0.13f, 0.33f, 0.13f, 1.0f);
        SDL_RenderClear(renderer);

        // BUTTONS
        SDL_FRect hitButton = {300, 500, 80, 40};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &hitButton);

        SDL_FRect standButton = {420, 500, 80, 40};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &standButton);

        if (gameState == GameState::GAME_OVER)
        {
            SDL_FRect playAgainButton = {340, 260, 120, 40};
            SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
            SDL_RenderFillRect(renderer, &playAgainButton);
        }

        // GLOBAL TEXT
        float textW, textH;
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 255};

        // PLAYER SCORE TEXT
        std::string playerText = "Player " + std::to_string(playerScore);
        SDL_Surface* playerSurface =
            TTF_RenderText_Blended(font, playerText.c_str(), 0, white);
        SDL_Texture* playerTexture =
            SDL_CreateTextureFromSurface(renderer, playerSurface);

        SDL_GetTextureSize(playerTexture, &textW, &textH);
        SDL_FRect playerTextRect = {50, 400, textW, textH};
        SDL_RenderTexture(renderer, playerTexture, nullptr, &playerTextRect);
        SDL_DestroySurface(playerSurface);
        SDL_DestroyTexture(playerTexture);

        // DEALER SCORE TEXT
        std::string dealerText = "Dealer: " + std::to_string(dealerScore);
        SDL_Surface* dealerSurface =
            TTF_RenderText_Blended(font, dealerText.c_str(), 0, white);
        SDL_Texture* dealerTexture =
            SDL_CreateTextureFromSurface(renderer, dealerSurface);
        SDL_GetTextureSize(dealerTexture, &textW, &textH);
        SDL_FRect dealerTextRect = {50, 100, textW, textH};
        SDL_RenderTexture(renderer, dealerTexture, nullptr, &dealerTextRect);
        SDL_DestroySurface(dealerSurface);
        SDL_DestroyTexture(dealerTexture);

        // HIT BUTTON TEXT
        std::string hitButtonText = "HIT";
        SDL_Surface* hitButtonSurface =
            TTF_RenderText_Blended(smallFont, hitButtonText.c_str(), 0, black);
        SDL_Texture* hitButtonTexture =
            SDL_CreateTextureFromSurface(renderer, hitButtonSurface);
        SDL_GetTextureSize(hitButtonTexture, &textW, &textH);
        SDL_FRect hitButtonRect = {340 - (textW / 2), 520 - (textH / 2), textW,
                                   textH};
        SDL_RenderTexture(renderer, hitButtonTexture, nullptr, &hitButtonRect);
        SDL_DestroySurface(hitButtonSurface);
        SDL_DestroyTexture(hitButtonTexture);

        // STAND BUTTON TEXT
        std::string standButtonText = "STAND";
        SDL_Surface* standButtonSurface = TTF_RenderText_Blended(
            smallFont, standButtonText.c_str(), 0, black);
        SDL_Texture* standButtonTexture =
            SDL_CreateTextureFromSurface(renderer, standButtonSurface);
        SDL_GetTextureSize(standButtonTexture, &textW, &textH);
        SDL_FRect standButtonRect = {460 - (textW / 2), 520 - (textH / 2),
                                     textW, textH};
        SDL_RenderTexture(renderer, standButtonTexture, nullptr,
                          &standButtonRect);
        SDL_DestroySurface(standButtonSurface);
        SDL_DestroyTexture(standButtonTexture);

        // PLAY AGAIN BUTTON TEXT
        std::string playAgainButtonText = "PLAY AGAIN";
        if (gameState == GameState::GAME_OVER)
        {
            SDL_Surface* playAgainTextSurface = TTF_RenderText_Blended(
                smallFont, playAgainButtonText.c_str(), 0, black);
            SDL_Texture* playAgainTextTexture =
                SDL_CreateTextureFromSurface(renderer, playAgainTextSurface);
            SDL_GetTextureSize(playAgainTextTexture, &textW, &textH);
            SDL_FRect playAgainTextRect = {400 - (textW / 2), 280 - (textH / 2),
                                           textW, textH};
            SDL_RenderTexture(renderer, playAgainTextTexture, nullptr,
                              &playAgainTextRect);
            SDL_DestroySurface(playAgainTextSurface);
            SDL_DestroyTexture(playAgainTextTexture);
        }

        SDL_RenderPresent(renderer);

        if (gameState == GameState::PLAYER_TURN)
        {
            // player logic will go here
        }
        else if (gameState == GameState::DEALER_TURN)
        {
            // dealer logic will go here
            if (!isDealerDone)
            {
                dealerScore = calculateScore(dealerHand);
                if (dealerScore > 21)
                {
                    isDealerBusted = true;
                    gameState = GameState::GAME_OVER;
                    std::cout << "GAME OVER! DEALER BUSTS!\n";
                }
                else if (dealerScore >= 17)
                {
                    isDealerDone = true;
                    gameState = GameState::GAME_OVER;
                    std::cout << dealerScore << "\n";
                }
                else if (dealerScore < 17)
                {
                    dealCard(deck, dealerHand);
                    dealerScore = calculateScore(dealerHand);
                }
            }
        }
        else if (gameState == GameState::GAME_OVER)
        {
            if (!isGameOverHandled)
            {
                isGameOverHandled = true;
                // show winner will go here
                if (isPlayerBusted)
                {
                    std::cout << "DEALERS WIN - Player Busted\n";
                }
                else if (isDealerBusted)
                {
                    std::cout << "PLAYERS WIN - Dealers Busted\n";
                }
                else if (playerScore > dealerScore && !isPlayerBusted)
                {
                    std::cout << "PLAYER WINS\n";
                }
                else if (dealerScore > playerScore && !isDealerBusted)
                {
                    std::cout << "DEALER WINS\n";
                }
                else if (playerScore == dealerScore)
                {
                    std::cout << "PUSH!\n";
                }
            }
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
