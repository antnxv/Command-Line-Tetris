#ifndef PLAYER
#define PLAYER

int get_left();
int get_right();
int get_x_bottom(int x);
int rotate_cw(int c);
int move_left();
int move_right();
int move_down();
int move_tetromino(int input);

#endif