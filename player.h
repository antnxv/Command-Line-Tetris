#ifndef PLAYER_H
#define PLAYER_H

int get_left();
int get_right();
int get_x_bottom(int x);
int rotate_cw(int c);
int move_left();
int move_right();
int move_down();
char move_tetromino(int input);

typedef struct tetromino {
    int map[4][4];
    int type;
    int x;
    int y;
} tetromino_t;

#endif // PLAYER_H