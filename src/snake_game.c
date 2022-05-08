#include <msp430.h>
#include <stdint.h>
#include "snake_game.h"
#include "functions.h"  // spi_send(), print(), rand()
#include "config.h"     // MAX7219 registers

Cell board[BOARD_ROWS][BOARD_COLS];
Snake snake;
ControlledCommand snake_cmd, prev_snake_cmd;
UncontrolledCommand game_cmd, lcd_text_type;
Bool snake_is_halted;
char win_text[LCD_TEXT_MAX_SIZE] = {'Y','O','U',' ','W','O','N','!'};
char lose_text[LCD_TEXT_MAX_SIZE] = {'Y','O','U',' ','L','O','S','E','!'};
char playing_text[LCD_TEXT_MAX_SIZE] = {'P','L','A','Y','I','N','G'};

void generate_fruit(){
    Coordinates empty_cells;
    Coordinate new_fruit_pos;
    unsigned int row, col, counter=0;
    for (row = 0; row < BOARD_ROWS; row++) {
        for (col = 0; col < BOARD_COLS; col++) {
            if (board[row][col].state == EMPTY) {
                empty_cells.coord[counter].col = col;
                empty_cells.coord[counter].row = row;
                empty_cells.quantity = ++counter;
            }
        }
    }

    new_fruit_pos = empty_cells.coord[rand() % empty_cells.quantity];
    board[new_fruit_pos.row][new_fruit_pos.col].state = FRUIT;
}

void snake_command(ControlledCommand new_command){
    // used to prevent address collision
    if (snake_cmd == UP || snake_cmd == DOWN || snake_cmd == LEFT || snake_cmd == RIGHT)
        prev_snake_cmd = snake_cmd;

    switch(new_command){
        case HALTED:
            snake_cmd = HALTED;
            return;
        case RESTART:
            snake_cmd = RESTART;
            return;
        // snake can't move into itself, keeps the previous direction
        case LEFT:
            snake_cmd = snake_cmd != RIGHT ? LEFT : RIGHT;
            return;
        case RIGHT:
            snake_cmd = snake_cmd != LEFT ? RIGHT : LEFT;
            return;
        case UP:
            snake_cmd = snake_cmd != DOWN ? UP : DOWN;
            return;
        case DOWN:
            snake_cmd = snake_cmd != UP ? DOWN : UP;
            return;
        default:
            return;
    }
}

void game_logic(){
    Cell *prev_head = snake.head, *prev_tail = snake.tail, *cell;
    unsigned int new_col = snake.head->coord.col, new_row = snake.head->coord.row;

    // exec uncontrolled state -> won, lose or is playing
    switch(game_cmd){
        case WON:
        case LOSE:
            if (snake_cmd == RESTART) board_initial_state();
            return;
        case PLAYING:
            // exec controlled command -> pause, restart or snake direction
            switch(snake_cmd){
                case HALTED:
                    snake_is_halted = True;
                    return; // if paused, do nothing
                case RESTART:
                    board_initial_state();
                    return;
                case LEFT:
                    new_col = (snake.head->coord.col - 1) >= COL0 ? snake.head->coord.col - 1 : COL7;
                    break;
                case RIGHT:
                    new_col = (snake.head->coord.col + 1) < BOARD_COLS ? snake.head->coord.col + 1 : COL0;
                    break;
                case UP:
                    new_row = (snake.head->coord.row - 1) >= ROW0 ? snake.head->coord.row - 1 : ROW7;
                    break;
                case DOWN:
                    new_row = (snake.head->coord.row + 1) < BOARD_ROWS ? snake.head->coord.row + 1 : ROW0;
                    break;
                default:
                    break;
            }
            cell = &board[new_row][new_col];

            // snake can't move into itself (address collision), keeps the previous direction
            if (cell == snake.head->prev && snake_is_halted == False) {
                snake_cmd = prev_snake_cmd;
                return;
            } else if (cell == snake.head->prev && snake_is_halted == True) {
                snake_cmd = HALTED;
                return;
            } else if (cell != snake.head->prev && snake_is_halted == True) {
                snake_is_halted = False;
            }

            break;
    }

    // verify cell state (is_body (lose), is_fruit (grow) or is empty (continues))
    switch(cell->state){
        case FRUIT:
            cell->state = SNAKE_BODY;
            cell->prev = prev_head;
            prev_head->next = cell;
            snake.head = cell;
            snake.size++;
            generate_fruit();
            if (snake.size >= SNAKE_WINNER_SIZE) game_cmd = WON;
            break;
        case SNAKE_BODY:
            game_cmd = LOSE;
            break;
        default:
            cell->prev = prev_head;
            cell->state = SNAKE_BODY;
            prev_head->next = cell;
            prev_tail->state = EMPTY;
            snake.head = cell;
            snake.tail = prev_tail->next;
            break;
    }
}

void board_initial_state(void){
    unsigned int row, col;

    for (row = 0; row < BOARD_ROWS; row++){
        for (col = 0; col < BOARD_COLS; col++){
            board[row][col].state = EMPTY;
            board[row][col].coord.row = row;
            board[row][col].coord.col = col;
        }
    }

    game_cmd = PLAYING;
    snake_is_halted = True;
    snake_command(HALTED);

    snake.head = &board[ROW3][COL2];
    snake.head->state = SNAKE_BODY;
    snake.head->prev = &board[ROW3][COL1];

    snake.tail = &board[ROW3][COL0];
    snake.tail->state = SNAKE_BODY;
    snake.tail->next = &board[ROW3][COL1];

    board[ROW3][COL1].state = SNAKE_BODY;
    board[ROW3][COL1].next = snake.head;
    board[ROW3][COL1].prev = snake.tail;

    snake.size = 3;

    generate_fruit();
}

void update_display_board(){
    uint8_t display_board[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    process_board_to_display(display_board);

    spi_send(MAX_DIGIT0, display_board[ROW0]);
    spi_send(MAX_DIGIT1, display_board[ROW1]);
    spi_send(MAX_DIGIT2, display_board[ROW2]);
    spi_send(MAX_DIGIT3, display_board[ROW3]);
    spi_send(MAX_DIGIT4, display_board[ROW4]);
    spi_send(MAX_DIGIT5, display_board[ROW5]);
    spi_send(MAX_DIGIT6, display_board[ROW6]);
    spi_send(MAX_DIGIT7, display_board[ROW7]);
}

void process_board_to_display(uint8_t *display_board){
    uint8_t byte;
    unsigned int row, col;

    for (row = 0; row < BOARD_ROWS; row++){
        byte = 0x0;
        for (col = 0; col < BOARD_COLS; col++){
            if (board[row][col].state == SNAKE_BODY || board[row][col].state == FRUIT)
                byte |= BIT0 << col;
            else
                byte &= ~(BIT0 << col);
        }
        display_board[row] = reverse_byte(byte);
    }
}

void update_display_lcd(){
    switch(game_cmd){
        case WON:
            if (lcd_text_type != WON) print(win_text);
            break;
        case LOSE:
            if (lcd_text_type != LOSE) print(lose_text);
            break;
        case PLAYING:
            if (lcd_text_type != PLAYING) print(playing_text);
            break;
        default:
            break;
    }
    lcd_text_type = game_cmd;
}


