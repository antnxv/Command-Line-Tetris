#include "tetris.h"
#include "board.h"
#include "player.h"
#include "display.h"
#include "pieces.h"

extern int status;

extern int top;
extern int score;
extern int level;
extern int lines;

extern int next_type;
extern tetromino_t player;
extern board_t board;

extern int maps[7][4][4];

int init_colors(){
    start_color();
    init_pair(I_PIECE, COLOR_CYAN, COLOR_CYAN);
    init_pair(L_PIECE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(J_PIECE, COLOR_BLUE, COLOR_BLUE);
    init_pair(T_PIECE, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(O_PIECE, COLOR_WHITE, COLOR_WHITE);
    init_pair(S_PIECE, COLOR_GREEN, COLOR_GREEN);
    init_pair(Z_PIECE, COLOR_RED, COLOR_RED);
    return 1;
}

void print_tile(int y, int x, int type){
    attron(COLOR_PAIR(type));
    mvprintw(y, x, "  ");
    attroff(COLOR_PAIR(type));
}

int print_stats(){
    char str[10] = "";
    int x, y;

    y = -1;
    sprintf(str, "%d", top);
    mvprintw(4+y, 4, "Top:    %-9.9s", str);
    strcpy(str, "");

    y++;
    sprintf(str, "%d", score);
    mvprintw(4+y, 4, "Score:  %-9.9s   ", str);
    strcpy(str, "");

    y ++;
    sprintf(str, "%d", level);
    mvprintw(4+y, 4, "Level:  %-9.9s   ", str);
    strcpy(str, "");

    y ++;
    sprintf(str, "%d", lines);
    mvprintw(4+y, 4, "Lines:  %-9.9s   ", str);
    return 1;
}

int print_next(){
    mvprintw(3, 26, "Next");
    for (int y = 0; y < 3; y ++){
        for (int x = 0; x < 4; x ++){
            if (maps[next_type-1][y][x] == 1){
                print_tile(4+y, 24 + 2*x, next_type);
            }else{
                
                mvprintw(4+y, 24 + 2*x, "  ");
            }
        }
    }
    return 1;
}

int print_base(){
    erase();
    mvprintw(1, 9, "Command Line Tetris\n\n");

    print_stats();
    print_next();

    int x, y, i;
    for (y = 19; y >= 0; y--){
        mvprintw(BOARD_Y+19 - y, 6, "%02d", y);

        for (x = 0; x < 10; x++){
            mvprintw(BOARD_Y+19 - y, BOARD_X + 2*x, ". ");
         }
    }
    mvprintw(BOARD_Y+21, 11, "0 1 2 3 4 5 6 7 8 9");
    return 1;
}

int print_controls(){
    int y = BOARD_Y + 4;
    int x = BOARD_X + 24;
    if (status){
        mvprintw(y++, x+7, "Controls\n");
        y++;
        mvprintw(y++, x+1, "Left or A: Move Left\n");
        mvprintw(y++, x+0, "Right or D: Move Right\n");
        mvprintw(y++, x+2, "Up or W: Rotate CW\n");
        y++;
        mvprintw(y++, x+1, "Down or S: Soft Drop\n");
        mvprintw(y++, x+3, "Space: Hard Drop\n");
        y++;
        mvprintw(y, x+5, "Q: Quit Game\n");
    }
    return 1;
}

int print_end() {
    mvprintw(BOARD_Y + 8, BOARD_X + 32, "Game over!\n");
    mvprintw(BOARD_Y + 9, BOARD_X + 29, "Press Q to quit.\n");
}

int print_board(){
    int y, x, dy, dx, type;
    int left, right, left_bottom, right_bottom;

    left = get_left();
    right = get_right();
    left_bottom = get_x_bottom(left);
    right_bottom = get_x_bottom(right);

    for (y = 19; y >= 0; y--){
        for (x = 0; x < 10; x++){
            dx = x - player.x;
            dy = player.y - y;

            // search player piece for something at coords
            if (dx >= 0 && dx < 4 &&
                dy >= 0 && dy < 4 &&
                player.map[dy][dx] == 1){
                print_tile(BOARD_Y+19 - y, BOARD_X + 2*x, player.type);
            }
            
            else {
                // board tile
                if ((type = board.tiles[y][x])) {
                    print_tile(BOARD_Y+19 - y, BOARD_X + 2*x, type);
                }
                
                else if (status && x == player.x + left && player.y - y > left_bottom) {  
                    // at player left and right (vertical I)
                    if (x == player.x + right) {
                        mvprintw(BOARD_Y+19 - y, BOARD_X + 2*x, "||");
                    }
                    // at player left
                    else {
                        mvprintw(BOARD_Y+19 - y, BOARD_X + 2*x, "| ");
                    }
                }
                
                // at player right
                else if (status && x == player.x + right && player.y - y > right_bottom) {
                    mvprintw(BOARD_Y+19 - y, BOARD_X + 2*x, " |");
                }
                
                // normal empty tile
                else {
                    mvprintw(BOARD_Y+19 - y, BOARD_X + 2*x, ". ");
                }
            }
        }
    }
    print_stats();
    return 1;
}