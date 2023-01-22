#define GRID 8
#define SPEED 150

#define START 0
#define PLAY 1
#define PAUSA 2

#define TECLAS 0
#define JOYSTICK 1

#define D_ALL (int[4]){1,1,1,1}
#define DIGIT(i) (i==0)? (int[4]){1,0,0,0} :\
				 (i==1)? (int[4]){0,1,0,0} :\
				 (i==2)? (int[4]){0,0,1,0} :\
				         (int[4]){0,0,0,1}

#define NUMBER(i) (i==0)? (int[7]){0,0,0,0,0,0,1} :\
				  (i==1)? (int[7]){1,0,0,1,1,1,1} :\
				  (i==2)? (int[7]){0,0,1,0,0,1,0} :\
				  (i==3)? (int[7]){0,0,0,0,1,1,0} :\
				  (i==4)? (int[7]){1,0,0,1,1,0,0} :\
				  (i==5)? (int[7]){0,1,0,0,1,0,0} :\
				  (i==6)? (int[7]){0,1,0,0,0,0,0} :\
				  (i==7)? (int[7]){0,0,0,1,1,1,1} :\
				  (i==8)? (int[7]){0,0,0,0,0,0,0} :\
				          (int[7]){0,0,0,0,1,0,0}
#define GUION (int[7]){1,1,1,1,1,1,0}

#define R 0
#define L 1
#define U 2
#define D 3

typedef struct Coord {
	int x;
	int y;
} coord_t;

typedef struct Snake {
	coord_t body[GRID*GRID];
	int length;
	int die;
} snake_t;

typedef struct Food {
	coord_t pos;
	int eaten;
} food_t;

extern int board[GRID][GRID];

//snake_t snake;
//int die;
//
//coord_t food;
//int eaten;
//
//int dir; // 0 der, 1 izq, 2 arr, 3 abj

void leds_on (int x, int y);
void leds_off(int x, int y);
void row_on(int i, int on);
void col_on(int i, int on);
void select_digit(int arr[4]);
void print_n(int arr[7]);
int* get_digits(int n);
void print_score(int state, int score);
void change_direction(int * dir, int new_dir);

void startup_board();
void print_board();
void update_board(snake_t * snake, food_t * food);
int rand_in_range(int min, int max);
snake_t spawn_snake();
void move_snake(snake_t * snake, int dir);
int is_in_snake(coord_t * c, snake_t * snake);
void eat(snake_t * snake, food_t * food);
void spawn_food (food_t * food, snake_t * snake);
