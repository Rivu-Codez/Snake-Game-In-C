#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>

#define HEIGHT 20
#define WIDTH 60
#define MAX_LEN (WIDTH*HEIGHT)

typedef struct{
    int x, y;
}Point;

typedef struct{
    Point body[MAX_LEN]; //body[0] is the head of the snake, an array of points.
    int length, dx, dy;
}Snake;

void draw_board(Snake *s, Point *food);
void init_snake(Snake *s);
void update_snake(Snake *s);
void reset_cursor(void);
void hide_cursor(void);
void handle_input(Snake *s, int *running);
int check_collision(Snake *s);
void spawn_food(Point *food, Snake *s);

int main(){
    Snake snake;
    Point food;
    int running = 1;
    int score = 0;

    srand(time(NULL));              
    init_snake(&snake);
    spawn_food(&food, &snake);
    hide_cursor();

    while(running){
        
        handle_input(&snake, &running);

        Point old_tail = snake.body[snake.length - 1];  
        update_snake(&snake);

        if(check_collision(&snake)){
            running = 0;
            break;
        }

        if(snake.body[0].x == food.x && snake.body[0].y == food.y){
            if(snake.length < MAX_LEN){
                snake.body[snake.length] = old_tail;   
                snake.length++;
            }
            score++;
            spawn_food(&food, &snake);
        }
        reset_cursor();
        draw_board(&snake, &food);
        printf("Score: %d\n", score);
        Sleep(100);
    }

    printf("\nGame Over! Final score: %d\n", score);
    _getch();
    return 0;
}

void draw_board(Snake *s, Point *food){
    for(int y = 0; y<HEIGHT; y++){
        for(int x = 0; x<WIDTH; x++){
            int flag = 0;
            for(int i = 1; i<s->length; i++){
                if(x == s->body[i].x && y == s-> body[i].y){
                    flag = 1;
                    break;
                }
            }
            if(y == 0 || y == HEIGHT-1 || x == 0 || x == WIDTH-1){
                printf("#");
            }else if(x == s->body[0].x && y == s->body[0].y){
                printf("@");
            }else if(flag == 1){
                printf("O");
            }else if(x == food->x && y == food->y){
                printf("*");
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void init_snake(Snake *s){
    s->length = 4;
    s->dx = 1;
    s->dy = 0;

    s->body[0].x = WIDTH/2;
    s->body[0].y = HEIGHT/2;

    for(int i = 1; i< s->length; i++){
        s->body[i].x = s->body[i-1].x - 1;
        s->body[i].y = s->body[i-1].y;
    }
}

void update_snake(Snake *s){
    for(int i = s->length-1 ; i>0; i--){
        s->body[i].x = s->body[i-1].x;
        s->body[i].y = s->body[i-1].y;
    }
    s->body[0].x += s->dx;
    s->body[0].y += s->dy;
}

void reset_cursor(void){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(h, pos);
}

void hide_cursor(void){


    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = 0;
    SetConsoleCursorInfo(h, &info);
}

void handle_input(Snake *s, int *running){
    if(!_kbhit()){
        return;                     // no key pressed, keep current direction
    }

    int key = _getch();             // read the key without waiting for Enter

    switch(key){
        case 'w': case 'W':
            if(s->dy != 1){ s->dx = 0; s->dy = -1; }
            break;
        case 's': case 'S':
            if(s->dy != -1){ s->dx = 0; s->dy = 1; }
            break;
        case 'a': case 'A':
            if(s->dx != 1){ s->dx = -1; s->dy = 0; }
            break;
        case 'd': case 'D':
            if(s->dx != -1){ s->dx = 1; s->dy = 0; }
            break;
        case 'q': case 'Q':
            *running = 0;           // tells the game loop to stop
            break;
    }
}

int check_collision(Snake *s){
    int hx = s->body[0].x;
    int hy = s->body[0].y;

    // wall check: same cells as the border in draw_board
    if(hx <= 0 || hx >= WIDTH - 1 || hy <= 0 || hy >= HEIGHT - 1){
        return 1;
    }

    // self check: head against body segments 1..length-1
    for(int i = 1; i < s->length; i++){
        if(hx == s->body[i].x && hy == s->body[i].y){
            return 1;
        }
    }

    return 0;
}

void spawn_food(Point *food, Snake *s){
    int free_cell;
    do{
        food->x = 1 + rand() % (WIDTH - 2);    // 1 .. WIDTH-2
        food->y = 1 + rand() % (HEIGHT - 2);   // 1 .. HEIGHT-2

        free_cell = 1;
        for(int i = 0; i < s->length; i++){
            if(food->x == s->body[i].x && food->y == s->body[i].y){
                free_cell = 0;                  // landed on the snake, pick again
                break;
            }
        }
    }while(!free_cell);
}


