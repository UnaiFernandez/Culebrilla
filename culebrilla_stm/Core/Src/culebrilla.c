#include <stdio.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "main.h"
#include "../Inc/culebrilla.h"

int board[GRID][GRID];

void leds_on(int x, int y) {
	board[y][x] = 1;
}
void leds_off(int x, int y) {
	board[y][x] = 0;
}

void row_on(int i, int on) {
	switch (i) {
	case 0:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, on);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, on);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, on);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, on);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, on);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, on);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, on);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, on);
		break;
	}
}
/*Cristobal*/
void col_on(int i, int on) {
	// COLUMNAS
	switch (i) {
	case 0:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, on);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, on);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, on);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, on);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, on);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, on);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, on);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, on);
		break;
	}
}

void select_digit(int arr[4]) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, arr[0]);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, arr[1]);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, arr[2]);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, arr[3]);
}

//PRINT NUMBERS

void print_n(int arr[7]) // writing 0
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, arr[0]);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, arr[1]);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, arr[2]);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, arr[3]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, arr[4]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, arr[5]);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, arr[6]);
	// digitalWrite(DOT, LOW);
}

int* get_digits(int n) {
	int *dig = malloc(4 * sizeof(int));
	int i;
	for (i = 3; i >= 0; i--) {
		dig[i] = n % 10;
		n /= 10;
	}
	return dig;
}

void print_score(int state, int score) {
	int i, n;
	int dig[4];
	switch (state) {
	case START:
		select_digit(D_ALL);
		print_n(GUION);
		break;
	case PLAY:
	case PAUSA:
		n = score;
		for (i = 3; i >= 0; i--) {
			dig[i] = n % 10;
			n /= 10;
		}
		//dig = get_digits(score);
		for (i = 0; i < 4; i++) {
			select_digit(DIGIT(i));
			print_n(NUMBER(dig[i]));
			HAL_Delay(1);
		}
		//free(dig);
		break;
	}
}

void change_direction(int *dir, int new_dir) { //OK
	switch (new_dir) {
	case U:
		if (*dir != D)
			*dir = U;
		break;
	case L:
		if (*dir != R)
			*dir = L;
		break;
	case D:
		if (*dir != U)
			*dir = D;
		break;
	case R:
		if (*dir != L)
			*dir = R;
		break;
	default:
		break;
	}
}

void startup_board() { //OK
	int i, j;
	for (i = 0; i < GRID; i++) {
		for (j = 0; j < GRID; j++) {
			leds_off(i, j);
		}
	}

}

void print_board() { //ok
	int i, j;
	for (i = 0; i < GRID; i++) {
		row_on(i, 1);
		//for (j=GRID-1; j>=0; j--) {
		for (j = 0; j < GRID; j++) {
			int b = board[i][j];
			if (b)
				col_on(j, 0);  //Encender
			else
				col_on(j, 1);  //Apagar
		}
		HAL_Delay(5);
		row_on(i, 0);
	}
}

void update_board(snake_t *snake, food_t *food) {  ///OK
	int i;
	coord_t s_pos;

	for (i = 0; i < snake->length; i++) {
		s_pos = snake->body[i];
		leds_on(s_pos.x, s_pos.y);
	}

	leds_on(food->pos.x, food->pos.y);
}

int rand_in_range(int min, int max) {  //OK
	return (rand() % (max - min + 1)) + min;
}

snake_t spawn_snake() {  //OK
	snake_t snake;
	coord_t head;
	head.x = rand_in_range(0, GRID - 1);
	head.y = rand_in_range(0, GRID - 1);

	snake.body[0] = head;
	snake.length = 1;
	snake.die = 0;
///Aqui crea el cuerpo de la serpiente y inicializa la variable de culebrilla muerta a 0
	return snake;
}

void move_snake(snake_t *snake, int dir) {  //OK
	int i;
	coord_t head_next = snake->body[0];

	switch (dir) {
	case R:
		//if (head_next.x == GRID-1)
		//head_next.x = 0;
		//else
		head_next.x = (head_next.x + 1) % GRID;
		break;
	case L:
		if (head_next.x == 0)
			head_next.x = GRID - 1;
		else
			head_next.x--;
		break;
	case D:
		head_next.y = (head_next.y + 1) % GRID;
		break;
	case U:
		if (head_next.y == 0)
			head_next.y = GRID - 1;
		else
			head_next.y--;
		break;
	}

	for (i = snake->length - 1; i > 0; i--) {
		snake->body[i] = snake->body[i - 1];
	}

	// check if is is dead
	if (is_in_snake(&head_next, snake)) {
		snake->die = 1;
	}
	snake->body[0] = head_next;

}

int is_in_snake(coord_t *c, snake_t *snake) {			//OK
	int i;
	for (i = 0; i < snake->length; i++) {
		if (snake->body[i].x == c->x && snake->body[i].y == c->y) {
			return 1;
		}
	}
	return 0;
}

void eat(snake_t *snake, food_t *food) {			///mas menos entendido
	if (snake->body[0].x == food->pos.x && snake->body[0].y == food->pos.y) {
		food->eaten = 1;
		snake->body[snake->length] = snake->body[0];
		snake->length++;
	}
}

void spawn_food(food_t *food, snake_t *snake) {			//OK
	while (is_in_snake(&(food->pos), snake)) {
		food->pos.x = rand_in_range(0, GRID - 1);
		food->pos.y = rand_in_range(0, GRID - 1);
	}

	food->eaten = 0;
}
