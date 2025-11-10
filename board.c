#include "tetris.h"
#include "board.h"
#include "player.h"
#include "display.h"
#include "pieces.h"

extern int status;
extern int delay;
extern pthread_mutex_t io_mutex;
extern pthread_mutex_t board_mutex;

extern int top;
extern int score;
extern int level;
extern int lines;

extern int next_type;
extern tetromino_t player;
extern board_t board;

int bag_idx = 0;
int bag[7] = {I_PIECE, L_PIECE, J_PIECE, T_PIECE, O_PIECE, S_PIECE, Z_PIECE};

extern int maps[7][4][4];

int choose_piece(){
    if (bag_idx == 0){
        for (int i = 0; i < 7; i++){
            int swap = rand() % 7;
            int temp = bag[i];
            bag[i] = bag[swap];
            bag[swap] = temp;
        }
    }
    
    bag_idx ++;
    bag_idx %= 7;
    return bag[bag_idx];
}

int no_collision_check(){
    for (int y = 0; y < 4; y ++){
        for (int x = 0; x < 4; x ++){
            if (player.map[y][x] == 1){
                
                // collision with wall
                if (player.x + x < 0 || player.x + x > 9 ||
                    player.y - y > 19 || player.y - y < 0){
                    return 0;
                }

                // collision with board tiles
                if (board.tiles[player.y - y][player.x + x] != 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int save_piece(){
    for (int y = 0; y < 4; y ++){
        for (int x = 0; x < 4; x ++){
            if (player.map[y][x] == 1){
                board.tiles[player.y - y][player.x + x] = player.type;
            }
        }
    }
    return 0;
}

int get_top(){
    int n = 0;

    FILE *f;
    char str[10] = "";
    f = fopen("user.dat", "r");
    if (f){
        char c = fgetc(f);
        while (c != EOF) {
            n = n*10 + atoi(&c);
            c = fgetc(f);
        }
    }
    
    return n;
}

int set_top(){
    if (top > get_top()){
        FILE *f;
        char str[10] = "";
        sprintf(str, "%d", top);
        f = fopen("user.dat", "w");
        fputs(str, f);
        // fputs("\n", f);
    }

    return 1;
}

int update_stats(int cleared){
    lines += cleared;
    switch (cleared){
        case 1:
            score += 40 * (level + 1);
            break;
        case 2:
            score += 100 * (level + 1);
            break;
        case 3:
            score += 300 * (level + 1);
            break;
        case 4:
            score += 1200 * (level + 1);
            break;
    }
    if (score > top){
        top = score;
    }

    if (lines/10 > level){
        level++;
        delay *= 0.8;
    }
    return 1;
}

int clear_lines() {
    int y, x, new_y;

    // map old lines to new lines
    new_y = 0;
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 10; x++) {
            if (board.tiles[y][x] == 0) {
                // line not complete, carry over to new board
                for (x = 0; x < 10; x++) {
                    board.tiles[new_y][x] = board.tiles[y][x];
                }
                new_y++;

                continue;
            }
        }
    }

    // new clear lines at top
    for (y = new_y; y < 20; y++) {
        for (x = 0; x < 10; x++) {
            board.tiles[y][x] = 0;
        }
    }

    return update_stats(y - new_y);

}

int next_piece(){
    player.type = next_type;
    player.x = 3;
    player.y = 20;
    memcpy(player.map, maps[player.type-1], sizeof(int[4][4]));

    next_type = choose_piece();

    print_next();

    if (!no_collision_check()){
        return 0;
    }

    return 1;
}

int end_game(){
    status = 0;
    set_top();
    print_base();
    print_end();

    pthread_mutex_lock(&board_mutex);
        print_board();
    pthread_mutex_unlock(&board_mutex);

    return 0;
}
