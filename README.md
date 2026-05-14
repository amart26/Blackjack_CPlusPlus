# Blackjack

A 2D Blackjack game built with C++ and SDL3, developed as a portfolio project to demonstrate systems programming, game architecture, and graphics programming fundamentals.

![C++](https://img.shields.io/badge/C++-17-blue) ![SDL3](https://img.shields.io/badge/SDL-3.0-green) ![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS-lightgrey)

## Features

- Full 52-card deck with Fisher-Yates shuffle
- Blackjack scoring with proper Ace logic (1 or 11)
- Dealer AI following standard casino rules (hits on 16, stands on 17)
- Game state machine managing player turn, dealer turn, and game over
- SDL3 2D rendering with live score display
- Clickable Hit, Stand, and Play Again buttons
- Bust and Blackjack detection

## Tech Stack

- **Language:** C++17
- **Graphics:** SDL3
- **Font Rendering:** SDL3_ttf
- **Build:** g++ (MinGW on Windows, Clang on macOS)

## Getting Started

### Prerequisites

**Windows:**
- [MinGW-w64](https://winlibs.com)
- [SDL3](https://github.com/libsdl-org/SDL/releases)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf/releases)

**macOS:**
```bash
brew install sdl3 sdl3_ttf
```

### Building

**Windows:**
```bash
g++ src/main.cpp -o blackjack -I"C:\SDL3\x86_64-w64-mingw32\include" -I"C:\SDL3_ttf\x86_64-w64-mingw32\include" -L"C:\SDL3\x86_64-w64-mingw32\lib" -L"C:\SDL3_ttf\x86_64-w64-mingw32\lib" -lSDL3 -lSDL3_ttf
```

**macOS:**
```bash
g++ src/main.cpp -o blackjack -I/opt/homebrew/opt/sdl3/include -I/opt/homebrew/opt/sdl3_ttf/include -L/opt/homebrew/opt/sdl3/lib -L/opt/homebrew/opt/sdl3_ttf/lib -lSDL3 -lSDL3_ttf
```

### Running

**Windows:**
```bash
.\blackjack.exe
```

**macOS:**
```bash
./blackjack
```

## How to Play

- **Hit** — Draw another card
- **Stand** — End your turn and let the dealer play
- **Play Again** — Reset and start a new round (appears after game ends)

## Project Structure

```
Blackjack/
├── src/
│   └── main.cpp
├── assets/
│   └── fonts/
│       └── OpenSans.ttf
├── .vscode/
│   └── c_cpp_properties.json
├── .clang-format
├── .gitignore
└── README.md
```

## Concepts Demonstrated

- Object-oriented design with structs and enums
- Dynamic memory management with vectors
- SDL3 game loop architecture
- Event-driven input handling
- State machine pattern for game flow
- Cross-platform C++ development

## Roadmap

- [ ] Card rendering with suit and rank visuals
- [ ] Chip and betting system
- [ ] Win/loss streak tracking
- [ ] Sound effects

## License

MIT
