#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;
using namespace sf;

// Global variables for the Whole Game
const int GRID_SIZE = 20;
const int WIDTH = 30, HEIGHT = 30;
int snake_x[1000], snake_y[1000], snake_length = 2;
int food_x, food_y, score = 0, score1 = 0, score2 = 0, high_score = 0, high_score1 = 0, direction = 0; // 0=Up, 1=Right, 2=Down, 3=Left
//score for Endless, score1 for Level mode, score2 for Custom 
//high_score for Endless, high_score1 for Level Mode
bool game_over = false;
bool game_started = false; // Flag to start moving the snake
int level = 0;  //1 for Level 1, 2 for Level 2, 3 for Level 3, 4 for Endless, 5 for Custom and 6 for infinity
const int MAX_WALLS = 1000; // Maximum number of wall segments
int wall_x[MAX_WALLS], wall_y[MAX_WALLS]; // Arrays for wall positions
int wall_count = 0; // Total number of wall segments
// Menu-specific variables
const int MenuSize = 8;  // Endless, Level, Custom,HighScores, Load Game, ColorSelection, Infinity Mode and Quit
Text menuOptions[MenuSize];
int selectedOption = 0;
Color color_selected = Color::Green;//Default Color of The Snake
// Function declarations for Endless and Level Modes
void initializeGame();
void generateFood();
void moveSnake();
bool checkCollision();
void updateGame();
void renderGame(RenderWindow&, Font&);
void handleInput(Keyboard::Key);
void level1Walls();
void level2Walls();
void level3Walls();
void startLevel3(RenderWindow&, Font&);
void startLevel1(RenderWindow&, Font&);
void startLevel2(RenderWindow&, Font&);
bool checkWallCollision();
void customMode(RenderWindow& window);
void startCustom(RenderWindow&, Font&);
bool loadGame();
void saveGame();
void loadlevel1(RenderWindow&, Font&);
void loadlevel2(RenderWindow&, Font&);
void loadlevel3(RenderWindow&, Font&);
void loadEndless(RenderWindow&, Font&);
Text createText(const string& , Font& , int , const Vector2f&);
void displayMenu(RenderWindow&, Font&);
bool displayMainMenu(RenderWindow&, Font&);
Color selectSnakeColor(RenderWindow&);
// Endless Mode logic
void startEndlessMode(RenderWindow&, Font&);
void saveHighScore(Font& font, RenderWindow& window);
void infinityMode(RenderWindow&, Font&);
void generateRandomWalls();
void initializeGame() {
    srand(time(0));
    snake_length = 2;
    snake_x[0] = rand() % WIDTH;
    snake_y[0] = rand() % HEIGHT;
    bool snake_on_wall = false;

    // Check if the snake head position is on a wall
    for (int i = 0; i < wall_count && snake_on_wall == false; i++) {
        if (snake_x[0] == wall_x[i] && snake_y[0] == wall_y[i]) {
            snake_on_wall = true;
            snake_x[0] = rand() % WIDTH;
            snake_y[0] = rand() % HEIGHT;
        }
    }
    snake_x[1] = snake_x[0] - 1;
    snake_y[1] = snake_y[0];
    generateFood();
    score = 0;score1 = 0;score2 = 0;
    direction = 1;
    game_over = false;
    game_started = false; // Snake does not start moving until a key is pressed
}

void generateFood() {
    bool food_on_wall = true;
    while (food_on_wall) {
        
        food_x = rand() % WIDTH;
        food_y = rand() % HEIGHT;
        food_on_wall = false;
        // Check if the generated food position is on a wall
        for (int i = 0; i < wall_count && food_on_wall == false; i++) {
            if (food_x == wall_x[i] && food_y == wall_y[i]) {
                food_on_wall = true;
                food_x = rand() % WIDTH;
                food_y = rand() % HEIGHT;
            }
        }
    }
}

void moveSnake() {
    // Move the body
    for (int i = snake_length; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    if (direction == 0) 
        snake_y[0]--; // Up
    else if (direction == 1) 
        snake_x[0]++; // Right
    else if (direction == 2) 
        snake_y[0]++; // Down
    else if (direction == 3) 
        snake_x[0]--; // Left

    // Wrap the snake around the screen
    if (snake_x[0] < 0) 
        snake_x[0] = WIDTH - 1;
    if (snake_x[0] >= WIDTH)
        snake_x[0] = 0;
    if (snake_y[0] < 0) 
        snake_y[0] = HEIGHT - 1;
    if (snake_y[0] >= HEIGHT) 
        snake_y[0] = 0;
}
// Check if the snake runs into itself
bool checkCollision() {
    for (int i = 1; i < snake_length; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) 
            return true;
    }
    return false;
}

void updateGame() {
    if (game_over) return; // Stop game logic if the game is over

    // If the game hasn't started yet, don't move the snake
    if (!game_started) return;

    moveSnake(); // Move the snake
    if (checkCollision()) {
        game_over = true; // Trigger game over when collision occurs
    }

    // Check if the snake eats food
    if (level == 4) {
        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            snake_length++; // Snake grows
            score += 10; 
            if (score > high_score) {
                high_score = score;
            } // Update high score for Endless Mode
            generateFood(); // Generate new food
        }
    }
    else if (level == 1 || level == 2 || level == 3) {
        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            snake_length++; // Snake grows
            score1 += 10; 
            if (score1 > high_score1) {
                high_score1 = score1;
            } // Update high score for Level Mode
            generateFood(); // Generate new food at a random position
        }
    }
    if (level == 5|| level==6) {
        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            snake_length++; // Snake grows
            score2 += 10; // Increase score
            generateFood(); // Generate new food at a random position
        }
    }
}
void renderGame(RenderWindow& window, Font& font) {
    RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
    RectangleShape grid_line(Vector2f(GRID_SIZE, GRID_SIZE));

    // Draw grid
    grid_line.setOutlineThickness(1);
    grid_line.setOutlineColor(Color(50, 50, 50));
    grid_line.setFillColor(Color::Transparent);

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            grid_line.setPosition(i * GRID_SIZE, j * GRID_SIZE);
            window.draw(grid_line);
        }
    }

    // Draw snake
    for (int i = 0; i < snake_length; i++) {
        cell.setPosition(snake_x[i] * GRID_SIZE, snake_y[i] * GRID_SIZE);
        cell.setFillColor(color_selected);
        window.draw(cell);
    }

    // Draw food
    cell.setPosition(food_x * GRID_SIZE, food_y * GRID_SIZE);
    cell.setFillColor(Color::Red);
    window.draw(cell);

    // Draw score
    Text score_text;
    if (level == 4) {
        score_text = createText("Score: " + to_string(score), font, 20, { 5,5 });
        window.draw(score_text);
    }
    else if (level == 1 || level == 2 || level == 3) {
        score_text = createText("Score: " + to_string(score1), font, 20, { 5,5 });
        window.draw(score_text);
    }
    if (level == 5|| level ==6) {
        score_text = createText("Score: " + to_string(score2), font, 20, { 5,5 });
        window.draw(score_text);
    }
    // Game Over message
    if (game_over) {
        Text game_over_text;
        game_over_text = createText("Game Over! Press Escape\n to return to menu.", font, 25, { 175, (HEIGHT * GRID_SIZE) / 2 });
        game_over_text.setFillColor(Color::Red);
        window.draw(game_over_text);
    }
}

void handleInput(Keyboard::Key key) {
    // Start the game if any key is pressed
    if (!game_started && key != Keyboard::Escape) {
        game_started = true;
    }

    if ((key == Keyboard::Up || key == Keyboard::W) && direction != 2) {
        direction = 0;
    }
    if ((key == Keyboard::Right || key == Keyboard::D) && direction != 3) {
        direction = 1;
    }
    if ((key == Keyboard::Down || key == Keyboard::S) && direction != 0) {
        direction = 2;
    }
    if ((key == Keyboard::Left || key == Keyboard::A) && direction != 1) {
        direction = 3;
    }
}

// Function to start Endless Mode
void startEndlessMode(RenderWindow& window, Font& font) {
    initializeGame(); // Initialize the game when Endless Mode starts

    // Game loop for Endless Mode
    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) 
                window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        // Update the game every 100 ms
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update the snake and check for collisions
            clock.restart();
        }

        window.clear();
        renderGame(window, font); // Render the game elements (snake, food, score)
        window.display();
    }
}
void loadEndless(RenderWindow& window, Font& font) {
    //Game already loaded before calling this function
    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) 
                window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        // Update the game every 100 ms
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame();
            clock.restart();
        }

        window.clear();
        renderGame(window, font); // Render the game elements (snake, food, score)
        window.display();
    }
}
// To avoid repitation of code
Text createText(const string& str, Font& font, int size, const Vector2f& position) {
    Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(Color::White); // Default color is white
    text.setPosition(position);
    return text;
}

void saveGame() {
    ofstream fout("saveGame.txt");
    if (!fout.is_open()) {
        cout << "Error: Could not open file to save game!" << endl;
        return;
    }
    //Save which level the user played last time
    fout << level << endl;
    // Save the snake's length
    fout << snake_length << endl;

    // Save the snake's position
    for (int i = 0; i < snake_length; i++) {
        fout << snake_x[i] << " " << snake_y[i] << endl;
    }

    // Save the food position, score, and direction
    fout << food_x << " " << food_y << endl;
    if (level == 4) {
        fout << score << endl;
    }
    else {
        fout << score1 << endl;
    }
    fout << direction << endl;
    fout.close();
    cout << "Game saved successfully!" << endl;
}

bool loadGame() {
    game_over = false;
    ifstream fin("savegame.txt");
    if (!fin.is_open()) {
        cout << "Error: Could not open file to load game!" << endl;
        return false;
    }
    //Read which level the user played last time
    fin >> level ;
    // Load the snake's length
    fin >> snake_length;

    // Load the snake's position
    for (int i = 0; i < snake_length; i++) {
        fin >> snake_x[i] >> snake_y[i];
    }

    // Load the food position, score, and direction
    fin >> food_x >> food_y;
    if (level == 4) {
        fin>>score;
    }
    else {
        fin>> score1;
    }
    fin >> direction;

    fin.close();
    cout << "Game loaded successfully!" << endl;
    return true;
}

//Funtions to Handle Menus
void displayMenu(RenderWindow& window, Font& font) {
    // Main menu options
    menuOptions[0] = createText("Load Game", font, 30, { 210, 75 });
    menuOptions[1] = createText("Endless Mode", font, 30, { 210, 125 });
    menuOptions[2] = createText("Level Mode", font, 30, { 210, 175 });
    menuOptions[3] = createText("Custom Mode", font, 30, { 210, 225 });
    menuOptions[4] = createText("High Scores", font, 30, { 210, 275 });
    menuOptions[5] = createText("Color Selection", font, 30, { 210, 325 });
    menuOptions[6] = createText("Infinity Mode", font, 30, { 210,375 });
    menuOptions[7] = createText("Quit", font, 30, { 210,425 });

    // Level menu options
    const int levelMenuSize = 4; // Level 1, Level 2, Level 3, Back
    Text levelMenuOptions[levelMenuSize];
    levelMenuOptions[0] = createText("Level 1", font, 30, { 225, 200 });
    levelMenuOptions[1] = createText("Level 2", font, 30, { 225, 250 });
    levelMenuOptions[2] = createText("Level 3", font, 30, { 225, 300 });
    levelMenuOptions[3] = createText("Back", font, 30, { 225, 350 });

    selectedOption = 0;       // Index of the currently selected menu option
    bool showLevelMenu = false;   // Whether the level menu is currently displayed
    bool game_loaded = false;
    Texture texture;
    texture.loadFromFile("menu.jpg");
    if (!texture.loadFromFile("menu.jpg"))
        cout << "Picture Failed to Load" << endl;
    Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                    selectedOption--;
                    if (selectedOption < 0) {
                        selectedOption = (showLevelMenu ? levelMenuSize : MenuSize) - 1;
                    }
                }
                else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                    selectedOption++;
                    if (selectedOption >= (showLevelMenu ? levelMenuSize : MenuSize)) {
                        selectedOption = 0;
                    }
                }
                else if (event.key.code == Keyboard::Enter) {
                    if (!showLevelMenu) {
                        if (selectedOption == 0) {
                            game_loaded = loadGame();//Load the saved Game 
                            if (level == 1) loadlevel1(window, font);
                            if (level == 2) loadlevel2(window, font);
                            if (level == 3) loadlevel3(window, font);
                            if (level == 4) loadEndless(window, font);
                        }
                        else if (selectedOption == 1) {
                            level = 4;
                            startEndlessMode(window, font);
                            // Start Endless Mode
                        }
                        else if (selectedOption == 2) {
                            showLevelMenu = true; // Enter level menu
                            selectedOption = 0;  // Reset selection for level menu
                        }
                        else if (selectedOption == 3) {
                            level = 5;
                            customMode(window);
                            startCustom(window, font);
                            // Custom Mode selected
                        }
                        else if (selectedOption == 4) {
                            saveHighScore(font, window);// Show High Score
                        }

                        else if (selectedOption == 5) {
                            Color color_selected = selectSnakeColor(window); // To Select Color of the Snake
                        }
                        else if (selectedOption == 6) {
                            level = 6;
                            infinityMode(window,font); // Infinity Mode
                        }
                        else if (selectedOption == 7) {
                            window.close(); // Quit
                        }
                    }
                    else {
                        if (selectedOption == 0) {
                            level = 1;
                            startLevel1(window, font);
                            // Start Level 1
                        }
                        else if (selectedOption == 1) {
                            level = 2;
                            startLevel2(window, font);
                            // Start Level 2
                        }
                        else if (selectedOption == 2) {
                            level = 3;
                            startLevel3(window, font);
                            // Start Level 3
                        }
                        else if (selectedOption == 3) {
                            showLevelMenu = false; // Back to main menu
                            selectedOption = 0;   // Reset selection for main menu
                        }
                    }
                }
            }
        }

        // Render the menu based on the current state
        window.clear();
        window.draw(sprite);
        if (showLevelMenu) {
            // Render Level Menu
            for (int i = 0; i < levelMenuSize; i++) {
                levelMenuOptions[i].setFillColor(i == selectedOption ? Color::Black : Color::White);
                window.draw(levelMenuOptions[i]);
            }
        }
        else {
            // Render Main Menu
            for (int i = 0; i < MenuSize; i++) {
                menuOptions[i].setFillColor(i == selectedOption ? Color::Black : Color::White);
                window.draw(menuOptions[i]);
            }
        }

        window.display();
    }

}
bool displayMainMenu(RenderWindow& window, Font& font) {
    Text title, playButton, names;
   
    names = createText("Credits: 24L-0811 & 24L-3028", font, 20, { 25,555 });// Title: Hungry Snake Game
    title = createText("HUNGRY SNAKE\n       GAME", font, 40, { 100,200 });// Play Button
    playButton=createText("Play", font, 30, { 210,325 });
    playButton.setFillColor(Color::Black);
    Texture texture;
    texture.loadFromFile("background.jpg");
    if (!texture.loadFromFile("background.jpg"))
        cout << "Picture Failed to Load" << endl;
    Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                return 1; // Exit the main screen to proceed to the main menu
            }
        }

        window.clear();
        window.draw(sprite);
        window.draw(names);
        window.draw(title);
        window.draw(playButton);
        window.display();
    }
}

void generateRandomWalls() {
    wall_count = 0; // Reset wall count
    srand(time(0)); // Seed random number generator

    int numWalls = 10 + score2 / 30; // Number of walls increases with score

    while (wall_count < numWalls ) {
        // Generate random position
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        bool validPosition = true;

        // Check if the position overlaps with the snake
        for (int i = 0; i < snake_length; i++) {
            if (snake_x[i] == x && snake_y[i] == y) {
                validPosition = false;
            }
        }

        // Check if the position overlaps with the food
        if (validPosition) {
            if (x == food_x && y == food_y) {
                validPosition = false;
            }
        }

        // Check if the position overlaps with existing walls
        if (validPosition) {
            for (int i = 0; i < wall_count; i++) {
                if (wall_x[i] == x && wall_y[i] == y) {
                    validPosition = false;
                }
            }
        }

        // Add the wall if the position is valid
        if (validPosition) {
            wall_x[wall_count] = x;
            wall_y[wall_count] = y;
            wall_count++;
        }
    }
}
void infinityMode(RenderWindow& window, Font& font) {
    initializeGame(); // Initialize the game state
    int lastWallUpdateScore = -1; // Track the last score at which walls were created

    Clock clock;

    while (window.isOpen()) {
        // Generate walls only once when score2 is 0 or a multiple of 50
        if (score2 % 30 == 0 && score2 != lastWallUpdateScore) {
            generateRandomWalls();
            lastWallUpdateScore = score2;
        }

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape) {
                    displayMenu(window, font); // Return to menu
                    return;
                }
            }
        }

        // Update the game state every 100ms
        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update snake movement and check for collisions

            if (checkWallCollision()) {
                game_over = true; // Trigger game over if wall collision occurs
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render the snake, food, and score
        window.display();
    }
}

void level3Walls() {
    // Reset wall data
    wall_count = 0;
    for (int i = 0; i < MAX_WALLS;i++) {
        wall_x[i] = 0;
        wall_y[i] = 0;
    }
    // Base Level 1 Walls (Outer boundary)
    for (int i = 0; i < WIDTH; i++) {
        wall_x[wall_count] = i; wall_y[wall_count] = 0;wall_count++;        // Top boundary
        wall_x[wall_count] = i; wall_y[wall_count] = HEIGHT - 1;wall_count++; // Bottom boundary
    }
    for (int i = 0; i < HEIGHT; i++) {
        wall_x[wall_count] = 0; wall_y[wall_count] = i;wall_count++;        // Left boundary
        wall_x[wall_count] = WIDTH - 1; wall_y[wall_count] = i;wall_count++; // Right boundary
    }

    // Horizontal Walls
    for (int x = 5; x <= 25; x++) {
        wall_x[wall_count] = x; wall_y[wall_count] = 5;wall_count++;        // (5,5) to (25,5)
    }
    for (int x = 10; x <= 20; x++) {
        wall_x[wall_count] = x; wall_y[wall_count] = 15;wall_count++;       // (10,15) to (20,15)
    }
    for (int x = 3; x <= 27; x++) {
        wall_x[wall_count] = x; wall_y[wall_count] = 25; wall_count++;      // (3,25) to (27,25)
    }

    // Vertical Walls
    for (int y = 5; y <= 20; y++) {
        wall_x[wall_count] = 5; wall_y[wall_count] = y;  wall_count++;      // (5,5) to (5,20)
    }
    for (int y = 5; y <= 20; y++) {
        wall_x[wall_count] = 25; wall_y[wall_count] = y; wall_count++;      // (25,5) to (25,20)
    }
    for (int y = 10; y <= 25; y++) {
        wall_x[wall_count] = 15; wall_y[wall_count] = y; wall_count++;      // (15,10) to (15,25)
    }
}
void startLevel3(RenderWindow& window, Font& font) {
    level3Walls(); // Create walls for Level 3
    initializeGame(); // Initialize the snake and food
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}
void loadlevel3(RenderWindow& window, Font& font) {
    //Game already loaded before calling this function
    level3Walls(); // Create walls for Level 3
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}

bool checkWallCollision() {
    for (int i = 0; i < wall_count; i++) {
        if (snake_x[0] == wall_x[i] && snake_y[0] == wall_y[i]) {
            return true; // Collision detected
        }
    }
    return false;
}

void level2Walls() {
    wall_count = 0;
    for (int i = 0; i < MAX_WALLS;i++) {
        wall_x[i] = 0;
        wall_y[i] = 0;
    }
    // Level 1 walls (outside border)
    for (int i = 0; i < WIDTH; i++) {
        wall_x[wall_count] = i;
        wall_y[wall_count] = 0;
        wall_count++;
        wall_x[wall_count] = i;
        wall_y[wall_count] = HEIGHT - 1;
        wall_count++;
    }
    for (int i = 0; i < HEIGHT; i++) {
        wall_x[wall_count] = 0;
        wall_y[wall_count] = i;
        wall_count++;
        wall_x[wall_count] = WIDTH - 1;
        wall_y[wall_count] = i;
        wall_count++;
    }

    // New walls for Level 2
    for (int i = 0; i <= 23; i++) {
        wall_x[wall_count] = i; wall_y[wall_count] = 6; wall_count++;
    }
    for (int i = 6; i <= 13; i++) {
        wall_x[wall_count] = 23; wall_y[wall_count] = i; wall_count++;
    }
    for (int i = 5; i <= 23; i++) {
        wall_x[wall_count] = i; wall_y[wall_count] = 13; wall_count++;
    }
    for (int i = 16; i <= 23; i++) {
        wall_x[wall_count] = 5; wall_y[wall_count] = i; wall_count++;
    }
}
void startLevel2(RenderWindow& window, Font& font) {
    level2Walls(); // Create walls for Level 2
    initializeGame(); // Initialize the snake and food
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game logic

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}
void loadlevel2(RenderWindow& window, Font& font) {
    //Game already loaded before calling this function

    level2Walls(); // Create walls for Level 2
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game logic

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}

void level1Walls() {
    wall_count = 0; // Reset wall count
    for (int i = 0; i < MAX_WALLS;i++) {
        wall_x[i] = 0;
        wall_y[i] = 0;
    }
    // Outer walls: top, bottom, left, and right borders
    for (int i = 0; i < WIDTH; i++) { // Top and bottom borders (row 0 and row 29)
        // Top wall (0,0) to (29,0)
        wall_x[wall_count] = i;  // x coordinate
        wall_y[wall_count] = 0;  
        wall_count++;

        // Bottom wall (0,29) to (29,29)
        wall_x[wall_count] = i;  
        wall_y[wall_count] = HEIGHT - 1; 
        wall_count++;
    }

    for (int i = 0; i < HEIGHT; i++) { 
        // Left wall (0,0) to (0,29)
        wall_x[wall_count] = 0;  // x = 0 (left side)
        wall_y[wall_count] = i;  // y coordinate
        wall_count++;

        // Right wall (29,0) to (29,29)
        wall_x[wall_count] = WIDTH - 1;  // x = 29 (right side)
        wall_y[wall_count] = i;  
        wall_count++;
    }
}
void startLevel1(RenderWindow& window, Font& font) {
    level1Walls(); // Create walls for Level1
    initializeGame(); // Initialize the snake and food
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game logic

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}
void loadlevel1(RenderWindow& window, Font& font) {
    //Game already loaded before calling this function
    level1Walls(); // Create walls for Level 1
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape && game_over != true) {
                    saveGame();
                    displayMenu(window, font); // Return to menu
                }
                else if (event.key.code == Keyboard::Escape)
                    displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100) {
            updateGame(); // Update game logic

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}

Color selectSnakeColor(RenderWindow& window) {
    // Display instructions
    Font font;
    if (!font.loadFromFile("arial.ttf")) { 
        cout << "Error loading font" << endl;
        return sf::Color::Green; // Default color
    }

    Text instructions("Press Number to Select the color of Snake\n 1-Yellow\n 2-Green\n 3-Cyan\n 4-Magenta\n 5-White", font, 30);
    instructions.setFillColor(Color::White);
    instructions.setPosition(10, 150);
    Texture texture;
    texture.loadFromFile("menu.jpg");
    if (!texture.loadFromFile("menu.jpg"))
        cout << "Picture Failed to Load" << endl;
    Sprite sprite;
    sprite.setTexture(texture);
    window.clear();
    window.draw(sprite);
    window.draw(instructions);
    window.display();

    bool colorSelected = false;

    while (colorSelected != true) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return Color::Green; // Default color if window is closed
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Num1) {
                    color_selected = Color::Yellow;
                    colorSelected = true;
                }
                if (event.key.code == Keyboard::Num2) {
                    color_selected = Color::Green;
                    colorSelected = true;
                }
                if (event.key.code == Keyboard::Num3) {
                    color_selected = Color::Cyan;
                    colorSelected = true;
                }
                if (event.key.code == Keyboard::Num4) {
                    color_selected = Color::Magenta;
                    colorSelected = true;
                }
                if (event.key.code == Keyboard::Num5) {
                    color_selected = Color::White;
                    colorSelected = true;
                }
            }
        }
    }
    return color_selected;
}

void startCustom(RenderWindow& window, Font& font) {
    initializeGame(); // Initialize the snake and food

    Clock clock;
    int customSpeed = 0;
    cout << "Enter 1-5 for Speed of snake: ";
    cin >> customSpeed;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::KeyPressed) {
                handleInput(event.key.code);
                if (event.key.code == Keyboard::Escape) displayMenu(window, font); // Return to menu
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > (250 / customSpeed)) {
            updateGame(); // Update game logic

            // Check collision with walls
            if (checkWallCollision()) {
                game_over = true; // End game if snake hits a wall
            }

            clock.restart();
        }

        // Rendering
        window.clear();

        // Draw walls
        RectangleShape cell(Vector2f(GRID_SIZE - 2, GRID_SIZE - 2));
        cell.setFillColor(Color::Blue);
        for (int i = 0; i < wall_count; i++) {
            cell.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(cell);
        }

        renderGame(window, font); // Render snake, food, and score
        window.display();
    }
}
void customMode(RenderWindow& window) {
    const int MAX_WALLS = 500;
    wall_count = 0;
    Font font;

    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font" << endl;
    }

    Text instructions;
    instructions = createText("Left Click: Add Wall\nRight Click: Remove Wall\nPress Enter to Start\n", font, 20, { 10,10 });

    bool startGame = false;

    while (!startGame && window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed) {
                int gridX = event.mouseButton.x / GRID_SIZE;
                int gridY = event.mouseButton.y / GRID_SIZE;

                if (event.mouseButton.button == Mouse::Left) {
                    bool exists = false;
                    // Check if the position already has a wall
                    for (int i = 0; i < wall_count; i++) {
                        if (wall_x[i] == gridX && wall_y[i] == gridY) {
                            exists = true;
                        }
                    }
                    if (!exists && wall_count < MAX_WALLS) {
                        wall_x[wall_count] = gridX;
                        wall_y[wall_count] = gridY;
                        wall_count++;
                    }
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    for (int i = 0; i < wall_count; i++) {
                        if (wall_x[i] == gridX && wall_y[i] == gridY) {
                            for (int j = i; j < wall_count - 1; j++) {
                                wall_x[j] = wall_x[j + 1];
                                wall_y[j] = wall_y[j + 1];
                            }
                            wall_count--;
                        }
                    }
                }
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                startGame = true;// Press Enter to start the game
            }
        }

        window.clear();
        window.draw(instructions);

        // Draw the walls
        for (int i = 0; i < wall_count; i++) {
            RectangleShape wall(Vector2f(GRID_SIZE, GRID_SIZE));
            wall.setFillColor(Color::Blue);
            wall.setPosition(wall_x[i] * GRID_SIZE, wall_y[i] * GRID_SIZE);
            window.draw(wall);
        }

        window.display();
    }
}

void saveHighScore(Font& font, RenderWindow& window) {
    const string fileName = "EndlessModeHighScores.txt";
    const string fileName1 = "LevelModeHighScores.txt";

    const int MAX_SCORES = 10;
    int scores[MAX_SCORES] = { 0 }; // Array to store the scores
    int count = 0; // Current number of scores
    Texture texture;
    texture.loadFromFile("menu.jpg");
    if (!texture.loadFromFile("menu.jpg"))
        cout << "Picture Failed to Load" << endl;
    Sprite sprite;
    sprite.setTexture(texture);
    // Read existing scores from the file
    ifstream fin(fileName);
    if (fin.is_open()) {
        while (count < MAX_SCORES && fin >> scores[count]) {
            count++;
        }
        fin.close();
    }

    // Add the new score
    scores[count] = high_score;
    count++;

    // Sort the scores in descending order (manual sorting)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i] < scores[j]) {
                // Swap scores[i] and scores[j]
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Ensure only the top 10 scores are kept
    if (count > MAX_SCORES) {
        count = MAX_SCORES;
    }

    // Write the updated scores back to the file
    ofstream fout(fileName);
    if (fout.is_open()) {
        for (int i = 0; i < count; i++) {
            fout << scores[i] << endl;
        }
        fout.close();
    }
    // Display the top 3 scores on the screen
    Text title, scoreTexts[3];
    title = createText("Top 3 High Scores For Endless Mode", font, 30, { 50,50 });
    for (int i = 0; i < 3 && i < count; i++) {
        scoreTexts[i].setFont(font);
        scoreTexts[i].setString(to_string(i + 1) + ": " + to_string(scores[i]));
        scoreTexts[i].setCharacterSize(25);
        scoreTexts[i].setFillColor(Color::Black);
        scoreTexts[i].setPosition(200, 100 + i * 50);
    }
    const int MAX_SCORES1 = 10;
    int scores1[MAX_SCORES1] = { 0 }; // Array to store the scores
    int count1 = 0; // Current number of scores

    // Read existing scores from the file
    ifstream fin1(fileName1);
    if (fin1.is_open()) {
        while (count1 < MAX_SCORES1 && fin1 >> scores1[count1]) {
            count1++;
        }
        fin1.close();
    }

    // Add the new score
    scores1[count1] = high_score1;
    count1++;

    // Sort the scores in descending order (manual sorting)
    for (int i = 0; i < count1 - 1; i++) {
        for (int j = i + 1; j < count1; j++) {
            if (scores1[i] < scores1[j]) {
                // Swap scores[i] and scores[j]
                int temp1 = scores1[i];
                scores1[i] = scores1[j];
                scores1[j] = temp1;
            }
        }
    }

    // Ensure only the top 10 scores are kept
    if (count1 > MAX_SCORES1) {
        count1 = MAX_SCORES1;
    }

    // Write the updated scores back to the file
    ofstream fout1(fileName1);
    if (fout1.is_open()) {
        for (int i = 0; i < count; i++) {
            fout1 << scores1[i] << endl;
        }
        fout1.close();
    }

    // Display the top 3 scores on the screen
    Text title1, scoreTexts1[3];
  
    title1 = createText("Top 3 High Scores For Level Mode", font, 30, { 50,250 });
    for (int i = 0; i < 3 && i < count1; i++) {
        scoreTexts1[i].setFont(font);
        scoreTexts1[i].setString(to_string(i + 1) + ": " + to_string(scores1[i]));
        scoreTexts1[i].setCharacterSize(25);
        scoreTexts1[i].setFillColor(Color::Black);
        scoreTexts1[i].setPosition(200, 300 + i * 50);

    }

    // Render the top 3 scores
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    displayMenu(window, font); // Exit to the main menu when Escape is pressed
                }
            }
        }

        window.clear();
        window.draw(sprite);
        window.draw(title);
        for (int i = 0; i < 3 && i < count; i++) {
            window.draw(scoreTexts[i]);
        }
        window.draw(title1);
        for (int i = 0; i < 3 && i < count1; i++) {
            window.draw(scoreTexts1[i]);
        }

        window.display();
    }
}

int main() {
    RenderWindow window(VideoMode(WIDTH * GRID_SIZE, HEIGHT * GRID_SIZE), "Hungry Snake");

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error: Could not load font!" << endl;
    }
    bool is_displayed = displayMainMenu(window, font);
    while (window.isOpen()) {
        if (is_displayed) {
            displayMenu(window, font);//Displaying Options to Select From
        }
    }
}