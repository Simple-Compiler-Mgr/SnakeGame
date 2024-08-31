#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SNAKE_BLOCK_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void run();

private:
    void processInput();
    void update();
    void render();
    void reset();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    Direction dir;
    std::vector<SnakeSegment> snake;
    SnakeSegment food;
};

SnakeGame::SnakeGame() : isRunning(true), dir(RIGHT) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    reset();
}

SnakeGame::~SnakeGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SnakeGame::reset() {
    snake.clear();
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    food = { rand() % (SCREEN_WIDTH / SNAKE_BLOCK_SIZE) * SNAKE_BLOCK_SIZE,
             rand() % (SCREEN_HEIGHT / SNAKE_BLOCK_SIZE) * SNAKE_BLOCK_SIZE };
}

void SnakeGame::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: if (dir != DOWN) dir = UP; break;
                case SDLK_DOWN: if (dir != UP) dir = DOWN; break;
                case SDLK_LEFT: if (dir != RIGHT) dir = LEFT; break;
                case SDLK_RIGHT: if (dir != LEFT) dir = RIGHT; break;
            }
        }
    }
}

void SnakeGame::update() {
    SnakeSegment newHead = snake.front();

    switch (dir) {
        case UP: newHead.y -= SNAKE_BLOCK_SIZE; break;
        case DOWN: newHead.y += SNAKE_BLOCK_SIZE; break;
        case LEFT: newHead.x -= SNAKE_BLOCK_SIZE; break;
        case RIGHT: newHead.x += SNAKE_BLOCK_SIZE; break;
    }

    // 检查是否吃到食物
    if (newHead.x == food.x && newHead.y == food.y) {
        snake.push_back({0, 0});
        food = { rand() % (SCREEN_WIDTH / SNAKE_BLOCK_SIZE) * SNAKE_BLOCK_SIZE,
                 rand() % (SCREEN_HEIGHT / SNAKE_BLOCK_SIZE) * SNAKE_BLOCK_SIZE };
    }

    // 移动蛇
    for (size_t i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;

    // 检查边界或自我碰撞
    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        isRunning = false;  // 游戏结束
    }
    for (size_t i = 1; i < snake.size(); ++i) {
        if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
            isRunning = false;  // 游戏结束
        }
    }
}

void SnakeGame::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 绘制蛇
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& segment : snake) {
        SDL_Rect rect = { segment.x, segment.y, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }

    // 绘制食物
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = { food.x, food.y, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE };
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_RenderPresent(renderer);
}

void SnakeGame::run() {
    while (isRunning) {
        processInput();
        update();
        render();
        SDL_Delay(100);
    }
}

int main() {
    SnakeGame game;
    game.run();
    return 0;
}