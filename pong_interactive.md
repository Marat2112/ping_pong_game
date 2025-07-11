# ping_pong_game

# Подробный разбор интерактивной версии Pong (pong_interactive.c)

## 1. Заголовочные файлы

#include <ncurses.h>  // Библиотека для работы с терминалом в реальном времени

#include <stdlib.h>   // Стандартная библиотека (rand, srand)

#include <time.h>     // Для работы со временем (time)

#include <unistd.h>   // Для функции usleep (задержка)


### ncurses.h - ключевая библиотека, которая позволяет:

  Очищать экран без мерцания

  Обрабатывать нажатия клавиш без ожидания Enter

  Позиционировать курсор для вывода в любом месте экрана

## 2. Константы игры

#define WIDTH 80         // Ширина поля (80 символов)

#define HEIGHT 25        // Высота поля (25 строк)

#define PADDLE_HEIGHT 3  // Высота ракетки (3 символа)

#define BALL_DELAY 50000 // Задержка между кадрами (в микросекундах)


### BALL_DELAY контролирует скорость игры - чем меньше значение, тем быстрее движется мяч.

## 3. Функция game_init()

void game_init(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy, 
              int *left_paddle, int *right_paddle, 
              int *score_left, int *score_right) {
    srand(time(NULL));  // Инициализация генератора случайных чисел
    
    *ball_x = WIDTH / 2;   // Стартовая позиция X мяча (центр)
    *ball_y = HEIGHT / 2;  // Стартовая позиция Y мяча (центр)
    
    // Случайное начальное направление мяча:
    *ball_dx = (rand() % 2) ? 1 : -1;  // По горизонтали (1 вправо, -1 влево)
    *ball_dy = (rand() % 2) ? 1 : -1;  // По вертикали (1 вниз, -1 вверх)
    
    // Центрирование ракеток:
    *left_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    *right_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    
    // Обнуление счета:
    *score_left = 0;
    *score_right = 0;
}
### Ключевые моменты:

  Все параметры передаются через указатели для модификации

  rand() % 2 дает 0 или 1, тернарный оператор преобразует в -1/1

  Ракетки центрируются по вертикали

## 4. Функция game_draw()

void game_draw(int ball_x, int ball_y, 
              int left_paddle, int right_paddle,
              int score_left, int score_right) {
    clear();  // Очистка экрана (ncurses)
    
    // Рисуем верхнюю и нижнюю границы:
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, '-');       // Верхняя граница
        mvaddch(HEIGHT - 1, x, '-'); // Нижняя граница
    }
    
    // Рисуем мяч:
    mvaddch(ball_y, ball_x, 'O');  // 'O' в позиции (ball_x, ball_y)
    
    // Рисуем левую ракетку:
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        mvaddch(left_paddle + i, 0, '|');  // Вертикальная линия
    }
    
    // Рисуем правую ракетку:
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        mvaddch(right_paddle + i, WIDTH - 1, '|');  // Вертикальная линия
    }
    
    // Выводим счет и управление:
    mvprintw(HEIGHT, 0, "Score: %d - %d", score_left, score_right);
    mvprintw(HEIGHT + 1, 0, "Controls: A/Z (left) K/M (right)");
    
    refresh();  // Обновляем экран (выводим все изменения)
}
### Особенности:

  mvaddch(y, x, c) - выводит символ c в позицию (x,y)

  mvprintw() - аналог printf с позиционированием

  refresh() - применяет все изменения на экране

## 5. Функция process_input()

void process_input(int *left_paddle, int *right_paddle) {
    int ch = getch();  // Получаем нажатую клавишу
    
    switch (ch) {
        case 'a':  // Левая ракетка вверх
            if (*left_paddle > 1) (*left_paddle)--;
            break;
        case 'z':  // Левая ракетка вниз
            if (*left_paddle < HEIGHT - PADDLE_HEIGHT - 1) (*left_paddle)++;
            break;
        case 'k':  // Правая ракетка вверх
            if (*right_paddle > 1) (*right_paddle)--;
            break;
        case 'm':  // Правая ракетка вниз
            if (*right_paddle < HEIGHT - PADDLE_HEIGHT - 1) (*right_paddle)++;
            break;
    }
}
### Важные детали:

  getch() не блокирует выполнение (благодаря nodelay)

  Проверки > 1 и < HEIGHT - PADDLE_HEIGHT - 1 не дают ракеткам выйти за границы

## 6. Функция game_update()

void game_update(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                int left_paddle, int right_paddle,
                int *score_left, int *score_right) {
    // Движение мяча:
    *ball_x += *ball_dx;
    *ball_y += *ball_dy;
    
    // Отскок от верхней и нижней стенок:
    if (*ball_y <= 1 || *ball_y >= HEIGHT - 2) {
        *ball_dy = -(*ball_dy);  // Меняем направление по Y
    }
    
    // Отскок от левой ракетки:
    if (*ball_x == 1 && *ball_y >= left_paddle && *ball_y < left_paddle + PADDLE_HEIGHT) {
        *ball_dx = -(*ball_dx);  // Меняем направление по X
        
        // Эффект "подкрутки":
        if (*ball_y == left_paddle) *ball_dy = -1;          // Верхний край
        else if (*ball_y == left_paddle + PADDLE_HEIGHT - 1) *ball_dy = 1; // Нижний край
    }
    
    // Отскок от правой ракетки (аналогично левой):
    if (*ball_x == WIDTH - 2 && *ball_y >= right_paddle && *ball_y < right_paddle + PADDLE_HEIGHT) {
        *ball_dx = -(*ball_dx);
        if (*ball_y == right_paddle) *ball_dy = -1;
        else if (*ball_y == right_paddle + PADDLE_HEIGHT - 1) *ball_dy = 1;
    }
    
    // Гол в левые ворота:
    if (*ball_x < 0) {
        (*score_right)++;  // Увеличиваем счет правого
        
        // Сброс мяча:
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *ball_dx = 1;      // Направляем вправо
        *ball_dy = (rand() % 2) ? 1 : -1;  // Случайное направление по Y
    }
    
    // Гол в правые ворота (аналогично):
    if (*ball_x >= WIDTH) {
        (*score_left)++;
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *ball_dx = -1;     // Направляем влево
        *ball_dy = (rand() % 2) ? 1 : -1;
    }
}
### Физика игры:

  При отскоке от ракетки мяч получает дополнительное вертикальное ускорение

  После гола мяч появляется в центре с направлением в сторону проигравшего

## 7. Главная функция main()

int main() {
    // Объявление переменных состояния:
    int ball_x, ball_y, ball_dx, ball_dy;
    int left_paddle, right_paddle;
    int score_left = 0, score_right = 0;
    
    // Инициализация ncurses:
    initscr();      // Инициализация экрана
    cbreak();       // Режим немедленного ввода (без буферизации)
    noecho();       // Не отображать нажатия клавиш
    keypad(stdscr, TRUE);  // Включить обработку специальных клавиш
    nodelay(stdscr, TRUE); // Неблокирующий ввод
    curs_set(0);    // Скрыть курсор
    
    // Инициализация игры:
    game_init(&ball_x, &ball_y, &ball_dx, &ball_dy, 
             &left_paddle, &right_paddle,
             &score_left, &score_right);
    
    // Основной игровой цикл:
    while (1) {
        process_input(&left_paddle, &right_paddle);  // Обработка ввода
        game_update(&ball_x, &ball_y, &ball_dx, &ball_dy, // Обновление состояния
                   left_paddle, right_paddle,
                   &score_left, &score_right);
        game_draw(ball_x, ball_y, left_paddle, right_paddle, // Отрисовка
                 score_left, score_right);
        
        // Проверка условия победы:
        if (score_left >= 21 || score_right >= 21) {
            break;
        }
        
        usleep(BALL_DELAY);  // Задержка для контроля скорости
    }
    
    // Вывод результатов:
    clear();
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Game over! Score: %d - %d", 
             score_left, score_right);
    if (score_left > score_right) {
        mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Left player wins!");
    } else {
        mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Right player wins!");
    }
    refresh();
    getch();  // Ожидание нажатия любой клавиши
    
    endwin(); // Завершение работы с ncurses
    
    return 0;
}
### Ключевые моменты:

  nodelay(stdscr, TRUE) делает ввод неблокирующим

  usleep(BALL_DELAY) контролирует скорость игры

  endwin() обязательно вызывать перед выходом для восстановления терминала

# Запуск и управление

## Компиляция:

### bash
### gcc src/pong_interactive.c -o pong -lncurses

## Запуск:

### bash
### ./pong

## Управление:

### Игрок 1 (левая сторона):

#### A - вверх

#### Z - вниз

### Игрок 2 (правая сторона):

#### K - вверх

#### M - вниз

## Игра автоматически обновляется в реальном времени, без необходимости нажимать пробел для каждого хода.
