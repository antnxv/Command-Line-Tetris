#include "tetris.h"
#include "board.h"
#include "player.h"
#include "display.h"

#define INPUT_DELAY 5000000

int status = 1;
int delay = 999999999;
pthread_mutex_t io_mutex;
pthread_mutex_t board_mutex;

int top = 0;
int score = 0;
int level = 0;
int lines = 0;

int next_type;
tetromino_t player;
board_t board = {};

int maps[7][4][4] = 
{
    { /* I */
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, { /* L */
        {0, 0, 0, 0},
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0} 
    }, { /* J */
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    }, { /* T */
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    }, { /* O */
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }, { /* S */
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    }, { /* Z */
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }
};

void *move_down_passive(void *a){
    struct timespec t0 = {0, delay};

    while (status){
        nanosleep(&t0, NULL);

        pthread_mutex_lock(&board_mutex);
            move_down();
            print_board();
        pthread_mutex_unlock(&board_mutex);
    }
    return NULL;
}

// // used to test i/o race conditions
// void *refresh_debug(void *a){
//     while (status){
//         pthread_mutex_lock(&io_mutex);
//             refresh();
//         pthread_mutex_unlock(&io_mutex);
//     }
//     return NULL;
// }

int main(){
    int q;
    pthread_t down_id, ref_debug_id;
    struct timespec ti = {0, INPUT_DELAY};

    srand(time(0));
    
    initscr();
    clear();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    init_colors();
    
    top = get_top();
    next_type = choose_piece();
    next_piece();

    print_base();
    print_board();
    print_controls();

    q = getch();

    nodelay(stdscr, TRUE); // non-blocking getch(), returns ERR if no input
    pthread_mutex_init(&io_mutex, NULL);
    pthread_mutex_init(&board_mutex, NULL);
    pthread_create(&down_id, NULL, move_down_passive, NULL);
    // pthread_create(&ref_debug_id, NULL, refresh_debug, NULL);
    
    while (q != 'q' && q != 'Q'){
        if (status) {
            q =  move_tetromino(q);
        } else {
            pthread_mutex_lock(&io_mutex);
                q = getch();
            pthread_mutex_unlock(&io_mutex);
        }

        while (q == ERR) {
            nanosleep(&ti, NULL);
            pthread_mutex_lock(&io_mutex);
                q = getch();
            pthread_mutex_unlock(&io_mutex);
        }
    }
    
    top = set_top();
    endwin();
    exit(0);
}
