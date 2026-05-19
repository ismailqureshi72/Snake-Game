# Hungry Snake Game

A classic Snake Game built in C++ using the **SFML (Simple and Fast Multimedia Library)**. This project offers a variety of engaging game modes, customizable settings, and a save/load feature!

## Features

- **Endless Mode**: Play until you hit a wall or yourself. Compete for the highest score!
- **Level Mode**: Progress through increasingly difficult levels (Level 1, 2, and 3) with custom wall layouts.
- **Custom Mode**: Play with a custom game setup.
- **Infinity Mode**: A mode where walls are dynamically generated as your score increases.
- **Save & Load Game**: Pause and save your game progress, then come back and resume right where you left off.
- **High Scores**: Keep track of your best runs across different game modes.
- **Color Selection**: Customize the color of your snake.

## Prerequisites

To compile and play this game, you will need:
- **C++ Compiler** (e.g., MSVC, GCC, or Clang)
- **SFML (Simple and Fast Multimedia Library)** installed and configured in your environment.
- Visual Studio (recommended for Windows, using the included `.sln` and `.vcxproj` files).

## How to Play

1. **Controls:**
   - Use the **Arrow Keys** (`Up`, `Down`, `Left`, `Right`) or **W, A, S, D** to control the direction of the snake.
   - Press **Escape** to pause/save the game and return to the main menu.
2. **Objective:**
   - Eat the red food block to grow your snake and increase your score.
   - Avoid colliding with your own tail or the blue walls!

## Project Structure

- `snake game.cpp`: The main source code containing all the game logic, rendering, and menu handling.
- Visual Studio project files (`.sln`, `.vcxproj`) for easy setup on Windows.
- Font files (`.ttf`) for rendering text in the game menu and score overlays.
- Image assets (`background.jpg`, `menu.jpg`) for the UI.

## Credits

Developed by **24L-0811** & **24L-3028**.
