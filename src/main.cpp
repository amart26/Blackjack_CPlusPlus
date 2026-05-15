#include "Card.h"
#include "GameState.h"
#include "Renderer.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <string>

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

    SDL_Texture* heartsTexture =
        IMG_LoadTexture(renderer, "assets/images/Hearts.png");
    if (heartsTexture == nullptr)
    {
        SDL_Log("Failed to load hearts: %s", SDL_GetError());
    }
    SDL_Texture* clubsTexture =
        IMG_LoadTexture(renderer, "assets/images/Clubs.png");
    if (clubsTexture == nullptr)
    {
        SDL_Log("Failed to load clubs: %s", SDL_GetError());
    }
    SDL_Texture* diamondsTexture =
        IMG_LoadTexture(renderer, "assets/images/Diamonds.png");
    if (diamondsTexture == nullptr)
    {
        SDL_Log("Failed to load diamonds: %s", SDL_GetError());
    }
    SDL_Texture* spadesTexture =
        IMG_LoadTexture(renderer, "assets/images/Spades.png");
    if (spadesTexture == nullptr)
    {
        SDL_Log("Failed to load spades: %s", SDL_GetError());
    }

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

        // CARDS
        renderHand(renderer, playerHand, 350, heartsTexture, clubsTexture,
                   spadesTexture, diamondsTexture);
        renderHand(renderer, dealerHand, 50, heartsTexture, clubsTexture,
                   spadesTexture, diamondsTexture);

        // GLOBAL TEXT
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 255};

        // PLAYER SCORE TEXT
        std::string playerText = "Player " + std::to_string(playerScore);
        renderText(renderer, font, playerText, 50, 400, white);

        // DEALER SCORE TEXT
        std::string dealerText = "Dealer: " + std::to_string(dealerScore);
        renderText(renderer, font, dealerText, 50, 100, white);

        // HIT BUTTON TEXT
        std::string hitButtonText = "HIT";
        renderText(renderer, font, hitButtonText, 320, 503, black);

        // STAND BUTTON TEXT
        std::string standButtonText = "STAND";
        renderText(renderer, smallFont, standButtonText, 435, 509, black);

        // PLAY AGAIN BUTTON TEXT
        std::string playAgainButtonText = "PLAY AGAIN";
        if (gameState == GameState::GAME_OVER)
        {
            renderText(renderer, smallFont, playAgainButtonText, 355, 268,
                       black);
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
