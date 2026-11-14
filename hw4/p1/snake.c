#include "snake.h"

bool snakeInit(Snake *snake, int rows, int cols, size_t length) {
    if (rows < MIN_BOARD_SIZE || cols < MIN_BOARD_SIZE) {
        *snake = (Snake){0};
        return false;
    }
    if (length < 1 || length > (size_t)(cols - 2 * BOARD_WALL_THICKNESS)) {
        *snake = (Snake){0};
        return false;
    }
    int headRow = rows / 2;
    int headCol = cols / 2;
    if (length > (size_t)headCol) {
        headCol = (int)length;
    }
    for (size_t i = 0; i < length; i++) {
        SnakeNode *node = malloc(sizeof(SnakeNode));
        if (node == NULL) {
            snakeDestroy(snake);
            return false;
        }
        node->m_pos.m_row = headRow;
        node->m_pos.m_col = headCol - (int)length + 1 + (int)i;
        if (snake->m_head == NULL) {
            node->m_next = node;
        } else {
            node->m_next = snake->m_head->m_next;
            snake->m_head->m_next = node;
        }
        snake->m_head = node;
        snake->m_length += 1;
    }
    snake->m_direction = DIR_RIGHT;
    snake->m_rows = rows;
    snake->m_cols = cols;
    return true;
}

Coord snakeNextPosition(const Snake *snake, Direction requested) {
    Direction direction = snake->m_direction;
    Coord nextPos = snake->m_head->m_pos;
    if (requested >= DIR_UP && requested < DIRECTION_COUNT) {
        if (snake->m_length == 1 || requested != (direction + 2) % DIRECTION_COUNT) {
            direction = requested;
        }
    }
    if (direction == DIR_UP) {
        nextPos.m_row -= 1;
    } else if (direction == DIR_DOWN) {
        nextPos.m_row += 1;
    } else if (direction == DIR_LEFT) {
        nextPos.m_col -= 1;
    } else if (direction == DIR_RIGHT) {
        nextPos.m_col += 1;
    }
    return nextPos;
}

bool snakeContains(const Snake *snake, Coord pos) {
    if (snake->m_head == NULL || snake->m_length == 0) {
        return false;
    }
    SnakeNode *current = snake->m_head;
    for (size_t i = 0; i < snake->m_length; i++) {
        if (current->m_pos.m_row == pos.m_row && current->m_pos.m_col == pos.m_col) {
            return true;
        }
        current = current->m_next;
    }
    return false;
}

StepResult snakeStep(Snake *snake, Direction requested, bool grow) {
    Coord nextPos = snakeNextPosition(snake, requested);
    if (nextPos.m_row < BOARD_WALL_THICKNESS ||
        nextPos.m_row >= snake->m_rows - BOARD_WALL_THICKNESS ||
        nextPos.m_col < BOARD_WALL_THICKNESS ||
        nextPos.m_col >= snake->m_cols - BOARD_WALL_THICKNESS) {
        return STEP_WALL;
    }
    SnakeNode *tail = snake->m_head->m_next;
    SnakeNode *current = snake->m_head;
    for (size_t i = 0; i < snake->m_length; i++) {
        if (current->m_pos.m_row == nextPos.m_row && current->m_pos.m_col == nextPos.m_col) {
            if (grow || current != tail) {
                return STEP_SELF;
            }
        }
        current = current->m_next;
    }
    Direction direction = snake->m_direction;
    if (requested >= DIR_UP && requested < DIRECTION_COUNT) {
        if (snake->m_length == 1 || requested != (direction + 2) % DIRECTION_COUNT) {
            direction = requested;
        }
    }
    if (!grow) {
        tail->m_pos = nextPos;
        snake->m_head = tail;
    } else {
        SnakeNode *node = malloc(sizeof(SnakeNode));
        if (node == NULL) {
            return STEP_NO_MEMORY;
        }
        node->m_pos = nextPos;
        node->m_next = tail;
        snake->m_head->m_next = node;
        snake->m_head = node;
        snake->m_length += 1;
    }
    snake->m_direction = direction;
    return STEP_OK;
}

void snakeDestroy(Snake *snake) {
    SnakeNode *current = snake->m_head;
    for (size_t i = 0; i < snake->m_length; i++) {
        SnakeNode *next = current->m_next;
        free(current);
        current = next;
    }
    *snake = (Snake){0};
}

unsigned int nextRandom(unsigned int seed) {
    static unsigned int state = RANDOM_INITIAL_SEED;
    if (seed > 0) {
        state = seed % RANDOM_MODULUS;
    }
    state = (RANDOM_MULTIPLIER * state + RANDOM_INCREMENT) % RANDOM_MODULUS;
    return state;
}
