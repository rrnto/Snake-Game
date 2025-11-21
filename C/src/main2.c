#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define TILE_COUNT 30
#define TILE_SIZE (WINDOW_WIDTH / TILE_COUNT - 2)
#define MAX_SNAKE 100

typedef enum { STR, UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x, y;
} Point;

void exitWithoutError(const char *message);
void clearScreen(SDL_Renderer *renderer);
void drawSnake(SDL_Rect snakeRect, SDL_Renderer *renderer);
void drawFood(SDL_Rect foodRect, SDL_Renderer *renderer);
int checkMouseClick(int mouseX, int mouseY, SDL_Rect button);

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        exitWithoutError("SDL Initialization failed");

    window = SDL_CreateWindow("Snake Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
        exitWithoutError("SDL Window creation failed");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer)
        exitWithoutError("SDL Renderer creation failed");

    // --- Game Variables ---
    Direction dir;
    int snakeLength;
    Point snake[MAX_SNAKE];
    Point food;
    int gameOver;
    Uint32 lastTick;
    const int speed = 150; // ms per move

    // Restart function
    void restartGame() {
        snakeLength = 3;
        snake[0].x = 10; snake[0].y = 10;
        snake[1].x = 9; snake[1].y = 10;
        snake[2].x = 8; snake[2].y = 10;
        dir = STR;
        food.x = rand() % TILE_COUNT;
        food.y = rand() % TILE_COUNT;
        gameOver = 0;
        lastTick = SDL_GetTicks();
    }

    restartGame();

    SDL_bool play = SDL_TRUE;
    while (play) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) play = SDL_FALSE;

            if (event.type == SDL_KEYDOWN && !gameOver) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if(dir != DOWN) dir = UP; break;
                    case SDLK_DOWN: if(dir != UP) dir = DOWN; break;
                    case SDLK_LEFT: if(dir != RIGHT) dir = LEFT; break;
                    case SDLK_RIGHT: if(dir != LEFT) dir = RIGHT; break;
                    default: break;
                }
            }

            // Handle mouse click for restart
            if (event.type == SDL_MOUSEBUTTONDOWN && gameOver) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Rect restartButton = {WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 25, 100, 50};
                if (checkMouseClick(mouseX, mouseY, restartButton)) {
                    restartGame();
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (!gameOver && now - lastTick >= speed) {
            lastTick = now;

            if (dir != STR) {
                // Move body
                for (int i = snakeLength - 1; i > 0; i--)
                    snake[i] = snake[i - 1];

                // Move head
                switch (dir) {
                    case UP: snake[0].y--; break;
                    case DOWN: snake[0].y++; break;
                    case LEFT: snake[0].x--; break;
                    case RIGHT: snake[0].x++; break;
                    default: break;
                }

                // Wall collision ends the game
                if (snake[0].x < 0 || snake[0].x >= TILE_COUNT ||
                    snake[0].y < 0 || snake[0].y >= TILE_COUNT) {
                    gameOver = 1;
                }

                // Self collision
                for (int i = 1; i < snakeLength; i++) {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                        gameOver = 1;
                    }
                }

                // Food collision
                if (snake[0].x == food.x && snake[0].y == food.y) {
                    if (snakeLength < MAX_SNAKE) snakeLength++;
                    food.x = rand() % TILE_COUNT;
                    food.y = rand() % TILE_COUNT;
                }
            }
        }

        // --- Rendering ---
        clearScreen(renderer);

        SDL_Rect foodRect = {food.x * (TILE_SIZE + 2), food.y * (TILE_SIZE + 2),
                             TILE_SIZE, TILE_SIZE};
        drawFood(foodRect, renderer);

        for (int i = 0; i < snakeLength; i++) {
            SDL_Rect snakeRect = {snake[i].x * (TILE_SIZE + 2), snake[i].y * (TILE_SIZE + 2),
                                  TILE_SIZE, TILE_SIZE};
            drawSnake(snakeRect, renderer);
        }

        // Draw restart button if game over
        if (gameOver) {
            SDL_Rect restartButton = {WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 25, 100, 50};
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &restartButton);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// --- Functions ---
void exitWithoutError(const char *message) {
    SDL_Log("%s - %s", SDL_GetError(), message);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void clearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void drawSnake(SDL_Rect snakeRect, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &snakeRect);
}

void drawFood(SDL_Rect foodRect, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &foodRect);
}

int checkMouseClick(int mouseX, int mouseY, SDL_Rect button) {
    return mouseX >= button.x && mouseX <= button.x + button.w &&
           mouseY >= button.y && mouseY <= button.y + button.h;
}
