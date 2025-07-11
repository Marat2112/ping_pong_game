#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3

// Игровые переменные
int ball_x, ball_y;
int paddle_left_y, paddle_right_y;
int dx, dy;
int score_left = 0;
int score_right = 0;

void init_game() {
    srand(time(NULL));
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    paddle_left_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle_right_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    dx = (rand() % 2) ? 1 : -1;
    dy = (rand() % 2) ? 1 : -1;
}

void draw_field() {
    // Очистка экрана
    printf("\x1B[2J\x1B[H");
    
    // Верхняя граница
    for (int i = 0; i < WIDTH; i++) putchar('-');
    putchar('\n');
    
    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) {
                putchar('|');
                continue;
            }
            
            if (x == ball_x && y == ball_y) {
                putchar('O');
                continue;
            }
            
            if (x == 0 && y >= paddle_left_y && y < paddle_left_y + PADDLE_HEIGHT) {
                putchar('|');
                continue;
            }
            
            if (x == WIDTH - 1 && y >= paddle_right_y && y < paddle_right_y + PADDLE_HEIGHT) {
                putchar('|');
                continue;
            }
            
            putchar(' ');
        }
        putchar('\n');
    }
    
    // Нижняя граница
    for (int i = 0; i < WIDTH; i++) putchar('-');
    putchar('\n');
    
    // Счет и управление
    printf("Счет: %d - %d\n", score_left, score_right);
    printf("Управление: A/Z (левая) K/M (правая), Пробел - шаг\n");
    printf("Команды: ");
}

void process_input() {
    char input;
    while (1) {
        input = getchar();
        if (input == ' ') break;
        
        if (input == 'a' && paddle_left_y > 0) paddle_left_y--;
        if (input == 'z' && paddle_left_y < HEIGHT - PADDLE_HEIGHT) paddle_left_y++;
        if (input == 'k' && paddle_right_y > 0) paddle_right_y--;
        if (input == 'm' && paddle_right_y < HEIGHT - PADDLE_HEIGHT) paddle_right_y++;
    }
}

void update_game() {
    ball_x += dx;
    ball_y += dy;
    
    if (ball_y <= 0 || ball_y >= HEIGHT - 1) dy = -dy;
    
    if (ball_x == 1 && ball_y >= paddle_left_y && ball_y < paddle_left_y + PADDLE_HEIGHT) {
        dx = -dx;
        if (ball_y == paddle_left_y) dy = -1;
        else if (ball_y == paddle_left_y + PADDLE_HEIGHT - 1) dy = 1;
    }
    
    if (ball_x == WIDTH - 2 && ball_y >= paddle_right_y && ball_y < paddle_right_y + PADDLE_HEIGHT) {
        dx = -dx;
        if (ball_y == paddle_right_y) dy = -1;
        else if (ball_y == paddle_right_y + PADDLE_HEIGHT - 1) dy = 1;
    }
    
    if (ball_x < 0) {
        score_right++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        dx = 1;
        dy = (rand() % 2) ? 1 : -1;
    }
    
    if (ball_x >= WIDTH) {
        score_left++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        dx = -1;
        dy = (rand() % 2) ? 1 : -1;
    }
}

int main() {
    init_game();
    
    while (1) {
        draw_field();
        process_input();
        update_game();
        
        if (score_left >= 21) {
            printf("\nЛевый игрок победил %d:%d!\n", score_left, score_right);
            break;
        }
        if (score_right >= 21) {
            printf("\nПравый игрок победил %d:%d!\n", score_left, score_right);
            break;
        }
    }
    
    return 0;
}