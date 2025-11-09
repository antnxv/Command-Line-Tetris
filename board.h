#ifndef BOARD_H
#define BOARD_H

int choose_piece();
int no_collision_check();
int save_piece();
int get_top();
int set_top();
int update_stats(int tiles);
int clear_lines();
int next_piece();
int end_game();

typedef struct board {
    char tiles[20][10];
} board_t;

#endif // BOARD_H