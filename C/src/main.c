#include <stdio.h>
#include <SDL.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define tileCount 30
#define tileSize (WINDOW_WIDTH/tileCount - 2)

enum direction 
{
    STR,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void exitWithoutError(const char *message);
void clearScreen(SDL_Renderer *renderer);
void drawFood(SDL_Rect food, SDL_Renderer *renderer);
void drawSnake(SDL_Rect snake, SDL_Renderer * renderer);
void changePos(SDL_Rect *rct);

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect snakeHead;
    SDL_Rect food;
    int dir = 0;
    int fps = 33;
    int now=0, ex=0, dt=0;

    snakeHead.x = 10;
    snakeHead.y = 10;

    food.x = 10;
    food.y = 15;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        exitWithoutError("Initiallisation error \n");

    window = SDL_CreateWindow("Snake game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL)
        exitWithoutError("Impossible de creer la fenetre \n");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        exitWithoutError("impossible de creer le rendu \n");

    //------------------------------------------------------------------------------------------------------------------------------------

    SDL_bool play = SDL_TRUE;

    while(play)
    {
        SDL_Event event;

        now = SDL_GetTicks();
        dt = now - ex;
        

        if (dt > fps)
        {
            // printf("%d\n", now);
            
            while(SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        play = SDL_FALSE;
                        break;

                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_UP:
                            //dir = UP;
                            changePos(&snakeHead);
                            // printf("Mode nrm sx: %d\n", snakeHead.y);
                            // snakeHead.y++;
                            break;

                        case SDLK_DOWN:
                            snakeHead.y = snakeHead.y + 1;
                            break;

                        case SDLK_LEFT:
                            snakeHead.x--;
                            break;

                        case SDLK_RIGHT:
                            snakeHead.x++;
                            break;
                        
                        default:
                            break;
                        }

                    default:
                        break;
                }

                switch (dir)
                {
                    case STR:
                        /* code */
                        break;
                    
                    default:
                        break;
                }


                clearScreen(renderer);
                drawFood(food, renderer);
                drawSnake(snakeHead, renderer);
                //changePos(snakeHead, dir);

                

                SDL_RenderPresent(renderer);

                ex = now;
            }
        }

        else
            SDL_Delay(fps - dt);
    }

    

    //------------------------------------------------------------------------------------------------------------------------------------

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //------------------------------------------------------------------------------------------------------------------------------------

    return EXIT_SUCCESS;
}

void exitWithoutError(const char *message)
{
    SDL_Log("%s -+ %s", SDL_GetError(), message);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void clearScreen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void drawSnake(SDL_Rect snake, SDL_Renderer * renderer)
{
    snake.x = snake.x * tileCount;
    snake.y = snake.y * tileCount;
    snake.w = tileSize;
    snake.h = tileSize;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &snake);
}

void drawFood(SDL_Rect food, SDL_Renderer *renderer)
{
    food.x = food.x * tileCount;
    food.y = food.y * tileCount;
    food.w = tileSize;
    food.h = tileSize;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &food);
}

void changePos(SDL_Rect *rct)
{
        (*rct).y--;
        // printf("Mode fct sx: %d\n", (*rct).y);
        // (*rct).y--;
        // (*rct).y--;
        // (*rct).y--;
}
