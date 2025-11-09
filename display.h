#ifndef DISPLAY_H
#define DISPLAY_H

#define BOARD_X 11
#define BOARD_Y 8

int init_colors();
void print_tile(int y, int x, int type);
int print_stats();
int print_next();
int print_base();
int print_controls();
int print_end();
int print_board();

#endif // DISPLAY_H