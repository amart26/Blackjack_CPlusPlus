#include "Betting.h"
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
    // SCREEN
    int screenWidth = 1920;
    int screenHeight = 1080;

    // BUTTONS
    int buttonWidth = 120;
    int buttonHeight = 50;
    int buttonGap = 10;

    float dealButtonX = 210;
    float dealButtonY = 575;

    float standButtonX = (screenWidth / 2) + (buttonGap / 2);
    float standButtonY = 1000 + (80 / 2) - (buttonHeight / 2);

    float hitButtonX =
        ((float(screenWidth / 2) - buttonWidth - (buttonGap / 2)));
    float hitButtonY = 1000 + (80 / 2) - (buttonHeight / 2);

    float offset = 55;
    float playAgainButtonX = float(screenWidth / 2) - offset;
    float playerAgainButtonY = float(screenHeight / 2);

    std::string dealButtonText = "DEAL";
    std::string hitButtonText = "HIT";
    std::string standButtonText = "STAND";
    std::string playAgainButtonText = "PLAY AGAIN";

    // DRAG STATE
    int dragChipValue = 0;
    float dragX = 0;
    float dragY = 0;
    float draggedChipCol = 0;
    float draggedChipRow = 0;

    // BET ZONE
    float betChipsX = 200;
    float betChipsY = 700;
    float betChipRenderX = 238;
    float betChipRenderY = 675;
    float betChipsDropZoneOffset = 300;

    // CHIP PROPERTIES
    float chipWidth = 64;
    float chipHeight = 72;

    std::vector<Chip> chips = {{1, 800, 920, chipWidth, chipHeight, 0, 0},
                               {5, 880, 920, chipWidth, chipHeight, 1, 0},
                               {25, 960, 920, 64, 72, 2, 0},
                               {50, 1040, 920, 64, 72, 3, 0},
                               {100, 1120, 920, 64, 72, 4, 1}};

    std::vector<Chip> betChips = {};

    // GAME STATE
    GameState gameState = GameState::BETTING;

    // PLAYER & DEALER STATE
    int playerScore = 0;
    int dealerScore = 0;
    int playerChips = 1000;
    int currentBet = 0;
    std::string playerChipsText;

    // MOUSE
    float mouseX, mouseY;

    // BOOLS
    bool running = true;
    bool isDealerDone = false;
    bool isPlayerBusted = false;
    bool isDealerBusted = false;
    bool isGameOverHandled = false;
    bool isCardHidden = false;
    bool isDragging = false;
    bool isHovered;

    // COLORS
    SDL_Color green = {170, 255, 0, SDL_ALPHA_OPAQUE};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color gold = {255, 215, 0, 255};

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Log("SDL initialized successfully");
    SDL_Window* window =
        SDL_CreateWindow("Blackjack", screenWidth, screenHeight, 0);
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

    // LOADING ASSETS
    SDL_Texture* tableTexture =
        IMG_LoadTexture(renderer, "assets/images/table.png");
    if (tableTexture == nullptr)
    {
        SDL_Log("Failed to load tables: %s", SDL_GetError());
    }

    SDL_Texture* cardBackTexture =
        IMG_LoadTexture(renderer, "assets/images/CardBack.png");
    if (cardBackTexture == nullptr)
    {
        SDL_Log("Failed to load tables: %s", SDL_GetError());
    }

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

    SDL_Texture* chipsTexture =
        IMG_LoadTexture(renderer, "assets/images/Chips.png");
    if (chipsTexture == nullptr)
    {
        SDL_Log("Failed to load spades: %s", SDL_GetError());
    }

    SDL_Texture* chipsOutlinedTexture =
        IMG_LoadTexture(renderer, "assets/images/ChipsOutlined.png");
    if (chipsOutlinedTexture == nullptr)
    {
        SDL_Log("Failed to load spades: %s", SDL_GetError());
    }

    while (running)
    {

        // **RENDERING** //
        SDL_Event event;
        SDL_PollEvent(&event);

        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_SetRenderDrawColorFloat(renderer, 0.13f, 0.33f, 0.13f, 1.0f);
        SDL_RenderClear(renderer);

        SDL_FRect tableRect = {0, 0, 1920, 1080};
        SDL_RenderTexture(renderer, tableTexture, nullptr, &tableRect);

        SDL_FRect buttonBarRect = {0, 1000, 1920, 80};
        SDL_SetRenderDrawColor(renderer, 14, 17, 17, 255);
        SDL_RenderFillRect(renderer, &buttonBarRect);

        // RENDER CHIPS
        for (int i = 0; i < chips.size(); i++)
        {
            isHovered = isMouseOverChip(chips[i], mouseX, mouseY);
            if (!isHovered)
            {
                renderChip(renderer, chipsTexture, chips[i]);
            }
            else if (isHovered)
            {
                renderChip(renderer, chipsOutlinedTexture, chips[i]);
            }
        }

        for (int i = 0; i < betChips.size(); i++)
        {
            renderChip(renderer, chipsTexture, betChips[i]);
        }

        // DEAL BUTTON
        if (gameState == GameState::BETTING)
        {
            renderButton(renderer, dealButtonX, dealButtonY, buttonWidth,
                         buttonHeight, gold, dealButtonText, font, black);
        }

        // HIT BUTTON

        renderButton(renderer, hitButtonX, hitButtonY, buttonWidth,
                     buttonHeight, green, hitButtonText, font, black);

        // STAND BUTTON

        renderButton(renderer, standButtonX, standButtonY, buttonWidth,
                     buttonHeight, red, standButtonText, font, black);

        // PLAY AGAIN BUTTON

        if (gameState == GameState::GAME_OVER)
        {

            renderButton(renderer, playAgainButtonX, playerAgainButtonY,
                         buttonWidth, buttonHeight, red, playAgainButtonText,
                         smallFont, black);
        }

        // RENDER PLAYER CARD
        renderHand(renderer, playerHand, 175, 805, 1.0, heartsTexture,
                   clubsTexture, spadesTexture, diamondsTexture,
                   cardBackTexture, isCardHidden);

        // RENDER DEALER CARDS
        if (gameState != GameState::DEALER_TURN &&
            gameState != GameState::GAME_OVER)
        {
            renderHand(renderer, dealerHand, 714, 227, 0.8, heartsTexture,
                       clubsTexture, spadesTexture, diamondsTexture,
                       cardBackTexture, !isCardHidden);
        }
        else
        {
            renderHand(renderer, dealerHand, 714, 227, 0.8, heartsTexture,
                       clubsTexture, spadesTexture, diamondsTexture,
                       cardBackTexture, isCardHidden);
        }

        if (isDragging)
        {
            dragX = mouseX;
            dragY = mouseY;
            Chip draggedChip = {dragChipValue,
                                dragX - chipWidth / 2,
                                dragY - chipHeight / 2,
                                chipWidth,
                                chipHeight,
                                draggedChipCol,
                                draggedChipRow};
            renderChip(renderer, chipsTexture, draggedChip);
        }
        playerChipsText = "Player Chips: " + std::to_string(playerChips);
        renderText(renderer, font, playerChipsText, 100, 1025, white);

        SDL_RenderPresent(renderer);

        // EVENT HANDLING

        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {

            // BETTING CHIPS
            for (int i = 0; i < chips.size(); i++)
            {
                isHovered = isMouseOverChip(chips[i], mouseX, mouseY);
                if (isHovered && gameState == GameState::BETTING)
                {
                    isDragging = true;

                    dragChipValue = chips[i].value;
                    draggedChipCol = chips[i].col;
                    draggedChipRow = chips[i].row;
                }
            }

            // DEAL BUTTON CLICK DECETECTION
            if (currentBet > 0 && mouseX >= dealButtonX &&
                mouseX <= dealButtonX + buttonWidth && mouseY >= dealButtonY &&
                mouseY <= dealButtonY + buttonHeight)
            {
                dealCard(deck, dealerHand);
                dealCard(deck, dealerHand);

                dealCard(deck, playerHand);
                dealCard(deck, playerHand);

                playerScore = calculateScore(playerHand);
                dealerScore = calculateScore(dealerHand);

                gameState = GameState::PLAYER_TURN;
            }

            // HIT BUTTON CLICK DETECTION
            if (mouseX >= hitButtonX && mouseX <= hitButtonX + buttonWidth &&
                mouseY >= hitButtonY && mouseY <= hitButtonY + buttonHeight &&
                gameState != GameState::GAME_OVER &&
                gameState != GameState::BETTING)
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

            // STAND BUTTON CLICK DETECTION
            if (mouseX >= standButtonX &&
                mouseX <= standButtonX + buttonWidth &&
                mouseY >= standButtonY &&
                mouseY <= standButtonY + buttonHeight &&
                gameState != GameState::GAME_OVER &&
                gameState != GameState::BETTING)
            {
                std::cout << "STAND!, DEALERS TURN!\n";
                gameState = GameState::DEALER_TURN;
            }

            // PLAY AGAIN BUTTON DETECTION
            if (gameState == GameState::GAME_OVER &&
                mouseX >= playAgainButtonX &&
                mouseX <= playAgainButtonX + buttonWidth &&
                mouseY >= playerAgainButtonY &&
                mouseY <= playerAgainButtonY + buttonHeight)
            {
                playerHand.cards.clear();
                dealerHand.cards.clear();
                deck.cards.clear();
                betChips.clear();
                currentBet = 0;

                fillDeck(deck);
                shuffleDeck(deck);

                isDealerDone = false;
                isPlayerBusted = false;
                isDealerBusted = false;
                isGameOverHandled = false;

                gameState = GameState::BETTING;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            if (isDragging && mouseX >= betChipsX &&
                mouseX <= betChipsX + betChipsDropZoneOffset &&
                mouseY >= betChipsY &&
                mouseY <= betChipsY + betChipsDropZoneOffset)
            {
                currentBet += dragChipValue;
                playerChips -= dragChipValue;
                std::cout << "Current bet: " << currentBet << "\n";
                std::cout << "Player chips: " << playerChips << "\n";
                Chip newBetChip = {
                    dragChipValue,
                    betChipRenderX + ((float)betChips.size() * 5),
                    betChipRenderY - ((float)betChips.size() * 5),
                    chipWidth,
                    chipHeight,
                    draggedChipCol,
                    draggedChipRow};

                betChips.push_back(newBetChip);
                isDragging = false;
            }
            else
            {
                isDragging = false;
            }
        }

        if (gameState == GameState::PLAYER_TURN)
        {
        }
        else if (gameState == GameState::DEALER_TURN)
        {

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
                    playerChips += currentBet * 2;
                    std::cout << "PLAYERS WIN - Dealers Busted\n";
                }
                else if (playerScore == 21 && playerScore != dealerScore)
                {
                    playerChips += currentBet + (int)(currentBet * 1.5);
                }
                else if (playerScore > dealerScore && !isPlayerBusted)
                {
                    playerChips += currentBet * 2;
                    std::cout << "PLAYER WINS\n";
                }
                else if (dealerScore > playerScore && !isDealerBusted)
                {
                    std::cout << "DEALER WINS\n";
                }
                else if (playerScore == dealerScore)
                {
                    playerChips += currentBet;
                    std::cout << "PUSH!\n";
                }
            }
        }
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}
