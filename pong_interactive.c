#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define BALL_DELAY 50000

void game_init(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy, 
              int *left_paddle, int *right_paddle, 
              int *score_left, int *score_right) {
    srand(time(NULL));
    *ball_x = WIDTH / 2;
    *ball_y = HEIGHT / 2;
    *ball_dx = (rand() % 2) ? 1 : -1;
    *ball_dy = (rand() % 2) ? 1 : -1;
    *left_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    *right_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    *score_left = 0;
    *score_right = 0;
}

void game_draw(int ball_x, int ball_y, 
              int left_paddle, int right_paddle,
              int score_left, int score_right) {
    clear();
    
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, '-');
        mvaddch(HEIGHT - 1, x, '-');
    }
    
    mvaddch(ball_y, ball_x, 'O');
    
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        mvaddch(left_paddle + i, 0, '|');
        mvaddch(right_paddle + i, WIDTH - 1, '|');
    }
    
    mvprintw(HEIGHT, 0, "Score: %d - %d", score_left, score_right);
    mvprintw(HEIGHT + 1, 0, "Controls: A/Z (left) K/M (right)");
    
    refresh();
}

void process_input(int *left_paddle, int *right_paddle) {
    int ch = getch();
    
    switch (ch) {
        case 'a': if (*left_paddle > 1) (*left_paddle)--; break;
        case 'z': if (*left_paddle < HEIGHT - PADDLE_HEIGHT - 1) (*left_paddle)++; break;
        case 'k': if (*right_paddle > 1) (*right_paddle)--; break;
        case 'm': if (*right_paddle < HEIGHT - PADDLE_HEIGHT - 1) (*right_paddle)++; break;
    }
}

void game_update(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                int left_paddle, int right_paddle,
                int *score_left, int *score_right) {
    *ball_x += *ball_dx;
    *ball_y += *ball_dy;
    
    if (*ball_y <= 1 || *ball_y >= HEIGHT - 2) *ball_dy = -(*ball_dy);
    
    if (*ball_x == 1 && *ball_y >= left_paddle && *ball_y < left_paddle + PADDLE_HEIGHT) {
        *ball_dx = -(*ball_dx);
        if (*ball_y == left_paddle) *ball_dy = -1;
        else if (*ball_y == left_paddle + PADDLE_HEIGHT - 1) *ball_dy = 1;
    }
    
    if (*ball_x == WIDTH - 2 && *ball_y >= right_paddle && *ball_y < right_paddle + PADDLE_HEIGHT) {
        *ball_dx = -(*ball_dx);
        if (*ball_y == right_paddle) *ball_dy = -1;
        else if (*ball_y == right_paddle + PADDLE_HEIGHT - 1) *ball_dy = 1;
    }
    
    if (*ball_x < 0) {
        (*score_right)++;
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *ball_dx = 1;
        *ball_dy = (rand() % 2) ? 1 : -1;
    }
    
    if (*ball_x >= WIDTH) {
        (*score_left)++;
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *ball_dx = -1;
        *ball_dy = (rand() % 2) ? 1 : -1;
    }
}

int main() {
    int ball_x, ball_y, ball_dx, ball_dy;
    int left_paddle, right_paddle;
    int score_left = 0, score_right = 0;
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    game_init(&ball_x, &ball_y, &ball_dx, &ball_dy, 
             &left_paddle, &right_paddle,
             &score_left, &score_right);
    
    while (1) {
        process_input(&left_paddle, &right_paddle);
        game_update(&ball_x, &ball_y, &ball_dx, &ball_dy,
                   left_paddle, right_paddle,
                   &score_left, &score_right);
        game_draw(ball_x, ball_y, left_paddle, right_paddle,
                 score_left, score_right);
        
        if (score_left >= 21 || score_right >= 21) break;
        
        usleep(BALL_DELAY);
    }
    
    clear();
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Game over! Score: %d - %d", 
             score_left, score_right);
    if (score_left > score_right) {
        mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Left player wins!");
    } else {
        mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Right player wins!");
    }
    refresh();
    getch();
    endwin();
    
    return 0;
}