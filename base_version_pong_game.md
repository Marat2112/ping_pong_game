# Базовая версия (пошаговый режим) - src/pong.c

## 1. Заголовочные файлы и константы

#include <stdio.h>   // Для ввода/вывода (printf, getchar)

#include <stdlib.h>  // Для работы с памятью и случайными числами (rand, srand)

#include <time.h>    // Для работы со временем (time)

#define WIDTH 80      // Ширина игрового поля (80 символов)

#define HEIGHT 25     // Высота игрового поля (25 строк)

#define PADDLE_HEIGHT 3  // Высота ракетки (3 символа)

### Эти строки подключают необходимые библиотеки и определяют размеры игрового поля.

## 2. Глобальные переменные

int ball_x, ball_y;         // Позиция мяча (x - горизонталь, y - вертикаль)

int paddle_left_y;          // Позиция левой ракетки (верхний край)

int paddle_right_y;         // Позиция правой ракетки (верхний край)

int dx, dy;                 // Направление движения мяча (dx - по x, dy - по y)

int score_left = 0;         // Счет левого игрока

int score_right = 0;        // Счет правого игрока

### Эти переменные хранят текущее состояние игры.

## 3. Функция init_game()

void init_game()

{

    srand(time(NULL));       // Инициализация генератора случайных чисел
    
    ball_x = WIDTH / 2;     // Стартовая позиция мяча по x (центр)
    
    ball_y = HEIGHT / 2;    // Стартовая позиция мяча по y (центр)
    
    // Позиции ракеток по центру
    paddle_left_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle_right_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    // Случайное направление мяча (влево/вправо и вверх/вниз)
    dx = (rand() % 2) ? 1 : -1;
    dy = (rand() % 2) ? 1 : -1;
    
}
### Эта функция готовит игру к старту, устанавливая начальные позиции.

## 4. Функция draw_field()

void draw_field() {
    printf("\x1B[2J\x1B[H");  // Очистка экрана (специальные ANSI-коды)
    
    // Рисуем верхнюю границу поля
    for (int i = 0; i < WIDTH; i++) putchar('-');
    putchar('\n');
    
    // Рисуем основное поле (25 строк)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Боковые границы (левая и правая стенки)
            if (x == 0 || x == WIDTH - 1) {
                putchar('|');
                continue;
            }
            
            // Мяч
            if (x == ball_x && y == ball_y) {
                putchar('O');
                continue;
            }
            
            // Левая ракетка (3 символа)
            if (x == 0 && y >= paddle_left_y && y < paddle_left_y + PADDLE_HEIGHT) {
                putchar('|');
                continue;
            }
            
            // Правая ракетка (3 символа)
            if (x == WIDTH - 1 && y >= paddle_right_y && y < paddle_right_y + PADDLE_HEIGHT) {
                putchar('|');
                continue;
            }
            
            putchar(' ');  // Пустое пространство
        }
        putchar('\n');  // Новая строка
    }
    
    // Рисуем нижнюю границу поля
    for (int i = 0; i < WIDTH; i++) putchar('-');
    putchar('\n');
    
    // Выводим счет и подсказки по управлению
    printf("Счет: %d - %d\n", score_left, score_right);
    printf("Управление: A/Z (левая) K/M (правая), Пробел - шаг\n");
    printf("Команды: ");
}
### Эта функция рисует все игровое поле, ракетки, мяч и информацию о счете.

## 5. Функция process_input()

void process_input() {
    char input;
    while (1) {
        input = getchar();  // Ждем ввод от пользователя
        
        if (input == ' ') break;  // Пробел - завершить ввод
        
        // Управление левой ракеткой
        if (input == 'a' && paddle_left_y > 0) paddle_left_y--;  // Вверх
        if (input == 'z' && paddle_left_y < HEIGHT - PADDLE_HEIGHT) paddle_left_y++;  // Вниз
        
        // Управление правой ракеткой
        if (input == 'k' && paddle_right_y > 0) paddle_right_y--;  // Вверх
        if (input == 'm' && paddle_right_y < HEIGHT - PADDLE_HEIGHT) paddle_right_y++;  // Вниз
    }
}
### Обрабатывает нажатия клавиш для управления ракетками.

## 6. Функция update_game()

void update_game() {
    // Двигаем мяч
    ball_x += dx;
    ball_y += dy;
    
    // Отскок от верхней и нижней стенок
    if (ball_y <= 0 || ball_y >= HEIGHT - 1) dy = -dy;
    
    // Отскок от левой ракетки
    if (ball_x == 1 && ball_y >= paddle_left_y && ball_y < paddle_left_y + PADDLE_HEIGHT) {
        dx = -dx;  // Меняем направление
        // Эффект "подкрутки" при ударе по краю ракетки
        if (ball_y == paddle_left_y) dy = -1;
        else if (ball_y == paddle_left_y + PADDLE_HEIGHT - 1) dy = 1;
    }
    
    // Отскок от правой ракетки
    if (ball_x == WIDTH - 2 && ball_y >= paddle_right_y && ball_y < paddle_right_y + PADDLE_HEIGHT) {
        dx = -dx;
        if (ball_y == paddle_right_y) dy = -1;
        else if (ball_y == paddle_right_y + PADDLE_HEIGHT - 1) dy = 1;
    }
    
    // Гол в левые ворота
    if (ball_x < 0) {
        score_right++;  // Увеличиваем счет правого
        // Сбрасываем мяч в центр
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        dx = 1;  // Направляем вправо
        dy = (rand() % 2) ? 1 : -1;  // Случайное направление по вертикали
    }
    
    // Гол в правые ворота
    if (ball_x >= WIDTH) {
        score_left++;  // Увеличиваем счет левого
        // Сбрасываем мяч в центр
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
        dx = -1;  // Направляем влево
        dy = (rand() % 2) ? 1 : -1;  // Случайное направление по вертикали
    }
}
### Эта функция обновляет состояние игры: двигает мяч, обрабатывает столкновения и голы.

## 7. Функция main()

int main() {
    init_game();  // Подготовка игры
    
    while (1) {   // Основной игровой цикл
        draw_field();    // Рисуем поле
        process_input(); // Ждем ввод игроков
        update_game();   // Обновляем состояние
        
        // Проверяем условие победы (21 очко)
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
### Главная функция, которая управляет всем процессом игры.
