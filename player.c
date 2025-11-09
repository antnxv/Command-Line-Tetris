#include "tetris.h"
#include "board.h"
#include "player.h"
#include "display.h"
#include "pieces.h"

extern int delay;
extern pthread_mutex_t io_mutex;
extern pthread_mutex_t board_mutex;

extern int next_type;
extern tetromino_t player;

int get_left() {
    int x, y;
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            if (player.map[y][x] == 1) {
                return x;
            }
        }
    }
    return -1;
}

int get_right() {
    int x, y;
    for (x = 3; x >= 0; x--) {
        for (y = 0; y < 4; y++) {
            if (player.map[y][x] == 1) {
                return x;
            }
        }
    }
    return -1;
}

int get_x_bottom(int x) {
    int y;
    for (y = 3; y >= 0; y--) {
        if (player.map[y][x] == 1) {
            return y;
        }
    }
    return -1;
}

int rotate_cw(int c){
    int old[4][4];
    for (int i = 0; i < 4; i ++){
        for (int j = 0; j < 4; j ++){
            old[i][j] = player.map[i][j];
        }
    }

    if (player.type == I_PIECE || player.type == O_PIECE){
        // rotation about centre of 4x4 map
        for (int y = 0; y < 4; y ++){
            for (int x = 0; x < 4; x ++){
                player.map[y][x] = old[3-x][y];
            }
        }
    }else{
        // rotation about centre of 3x3 inner map
        for (int y = 1; y < 4; y ++){
            for (int x = 0; x < 3; x ++){
                player.map[y][x] = old[3-x][y-1];
            }
        }
    }

    if (!no_collision_check()){
        if (c < 4){
            return rotate_cw(c+1);
        }
        return 0;
    }
    return 1;
}

int move_left(){
    player.x -= 1;
    if (!no_collision_check()){
        player.x += 1;
        return 0;
    }
    return 1;
}

int move_right(){
    player.x += 1;
    if (!no_collision_check()){
        player.x -= 1;
        return 0;
    }
    return 1;
}

int move_down(){
    player.y -= 1;
    if (!no_collision_check()){
        player.y += 1;
        save_piece();
        clear_lines();
        if (!next_piece()){
            player.y += 2;
            next_type = player.type;
            end_game();
        }
        return 0;
    }
    return 1;
}

// down up left right
char move_tetromino(int input){
    char q;

    pthread_mutex_lock(&board_mutex);

        switch (input) {
            case KEY_DOWN:
            case 's':
            case 'S':
                move_down();
                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
                move_left();
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                move_right();
                break;
            case KEY_UP:
            case 'w':
            case 'W':
                rotate_cw(1);
                break;
            case ' ':
                while(move_down()){}
                break;
            default:
                pthread_mutex_unlock(&board_mutex);
                pthread_mutex_lock(&io_mutex);
                    q = getch();
                pthread_mutex_unlock(&io_mutex);
                return q; // skip print_board if rejected input
        }
    
        print_board();
    pthread_mutex_unlock(&board_mutex);
    
    pthread_mutex_lock(&io_mutex);
        q = getch();
    pthread_mutex_unlock(&io_mutex);

    return q;
}