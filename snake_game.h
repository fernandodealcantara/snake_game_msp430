/*
 * Módulo responsável por gerenciar o jogo
 */
#ifndef _SNAKE_GAME
#define _SNAKE_GAME

#include <stdint.h>

#define BOARD_ROWS 8
#define BOARD_COLS 8

#define SNAKE_WINNER_SIZE 16
#define LCD_TEXT_MAX_SIZE 16

typedef enum { False, True } Bool;
typedef enum { ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7 } Rows;
typedef enum { COL0, COL1, COL2, COL3, COL4, COL5, COL6, COL7 } Cols;
typedef enum { LEFT, RIGHT, UP, DOWN, HALTED, RESTART } ControlledCommand;
typedef enum { WON, LOSE, PLAYING } UncontrolledCommand;
typedef enum { EMPTY, SNAKE_BODY, FRUIT } CellState;

typedef struct {
    int row;
    int col;
} Coordinate;

typedef struct {
    Coordinate coord[BOARD_ROWS*BOARD_COLS];
    int quantity;
} Coordinates;

typedef struct sCell {
    CellState state;
    Coordinate coord;
    struct sCell *prev;
    struct sCell *next;
} Cell;

typedef struct sSnake {
    Cell *head;
    Cell *tail;
    int size;
} Snake;

Cell board[BOARD_ROWS][BOARD_COLS];     // matrix that contains the game cells
Snake snake;    // track the snake cells
ControlledCommand snake_cmd, prev_snake_cmd; // snake command
UncontrolledCommand game_cmd, lcd_text_type; // lcd_text_type is used to prevent "rerendering"
Bool snake_is_halted;

// selects a pseudo-random empty cell and sets it to 'FRUIT'
void generate_fruit(void);
// receive command
void snake_command(ControlledCommand);
// make a decision based on the game and snake cmd
void game_logic(void);
// matrix initial state -> snake initial position and fruit
void board_initial_state(void);
// maps the matrix state to the module that shows the game, in this case the module MAX7219 Matrix.
void update_display_board(void);
void process_board_to_display(uint8_t *);
// display the current game state in the lcd
void update_display_lcd(void);

#endif // _SNAKE_GAME
