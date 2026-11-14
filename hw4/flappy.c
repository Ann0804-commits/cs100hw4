#include "flappy.h"

void advancePipes(Game *game, float dt) {
    for (int i = 0; i < PIPE_COUNT; i++) {
        game->m_pipes[i].m_x -= PIPE_SPEED * dt;
    }
    for (int j = 0; j < PIPE_COUNT; j++) {
        float rightEdge = game->m_pipes[j].m_x + PIPE_WIDTH;
        /*回收*/
        if (rightEdge <= 0) {
            recyclePipe(game, j);
        }
    }
}

bool birdHitsObstacle(const Game *game) {
    Box bird = gameBirdBox(game);
    if (bird.m_y < 0 || (bird.m_y + bird.m_height) > FLOOR_Y) {
        return true;
    }
    for (int i = 0; i < PIPE_COUNT; i++) {
        /*画管道*/
        Box lowerPipe = {game->m_pipes[i].m_x, game->m_pipes[i].m_gap_y + PIPE_GAP_HEIGHT,
                         PIPE_WIDTH, FLOOR_Y - (game->m_pipes[i].m_gap_y + PIPE_GAP_HEIGHT)};
        Box upperPipe = {game->m_pipes[i].m_x, 0, PIPE_WIDTH, game->m_pipes[i].m_gap_y};
        if (boxesOverlap(bird, upperPipe) || boxesOverlap(bird, lowerPipe)) {
            /*边角碰撞不管*/
            return true;
        }
    }
    return false;
}

int scorePassedPipes(Game *game) {
    int score = 0;
    for (int i = 0; i < PIPE_COUNT; i++) {
        if (!game->m_pipes[i].m_scored && (game->m_pipes[i].m_x + PIPE_WIDTH) < BIRD_X) {
            game->m_pipes[i].m_scored = true;
            score += 1;
        }
    }
    game->m_score += score;
    return score;
}

unsigned int nextRandom(unsigned int seed) {
    static unsigned int state = RANDOM_INITIAL_SEED;
    if (seed > 0) {
        state = seed % RANDOM_MODULUS;
    }
    state = (RANDOM_MULTIPLIER * state + RANDOM_INCREMENT) % RANDOM_MODULUS;
    return state;
}
