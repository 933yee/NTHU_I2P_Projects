#include <bits/stdc++.h>
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
const int dfs_layer = 3;
int color_of_dics = WHITE;
int black_num, white_num, player;
std::pair<int, int> best_pos;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
const int dir[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
int alpha_beta[dfs_layer+1][2];
void init_alpha_beta(){
    for(int i=1; i<=dfs_layer; i++){
        alpha_beta[i][0] = INT_MIN;
        alpha_beta[i][1] = INT_MAX;
    }
}
void init_alpha_beta_two_to_last(){
    for(int i=2; i<=dfs_layer; i++){
        alpha_beta[i][0] = INT_MIN;
        alpha_beta[i][1] = INT_MAX;
    }
}
int color_flip(int cur_color){
    return ((cur_color-1)^1)+1;
}
std::pair<int, int> first_random(){
    while(true){
        bool good = true;
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        for(int i=0; i<8; i++){
            int try_x = x + dir[i][0]*2;
            int try_y = y + dir[i][1]*2;
            if(try_x < 0 || try_y < 0 || try_x >= SIZE || try_y >= SIZE){
                good = false;
                break;
            }
            if(!good) break;
        }
        if(good) return {x, y};
    }
}
std::pair<int, int> eval_tried_board_score(){
    int black_score = 0, white_score = 0;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(board[i][j] == BLACK){
                for(int k=0; k<8; k++){
                    int try_x = i , try_y = j, cnt = 0;
                    bool dead = false;
                    int first = 0;
                    while(try_x >= 0 && try_y >= 0 && try_x < SIZE && try_y < SIZE && board[try_x][try_y] == BLACK){
                        int disc_at_middle_x = try_x - dir[k][0]; 
                        int disc_at_middle_y = try_y - dir[k][1];
                        if(disc_at_middle_x >= 0 && disc_at_middle_y >= 0 && disc_at_middle_x < SIZE && disc_at_middle_y < SIZE){
                            if(board[disc_at_middle_x][disc_at_middle_y] == BLACK && first == 0) break;
                            else if(board[disc_at_middle_x][disc_at_middle_y] == WHITE) dead = true;
                            first++;
                        }else dead = true;
                        try_x += dir[k][0];
                        try_y += dir[k][1];
                        cnt++;
                        if(cnt == 5) break;
                    }
                    if(cnt == 0) continue;
                    if(cnt != 5 && try_x >= 0 && try_y >= 0 && try_x < SIZE && try_y < SIZE){
                        if(board[try_x][try_y] == WHITE){
                            if(dead) continue;
                            else dead = true;
                        }
                    }else if(cnt == 5){
                        black_score += 1000000;
                        if(color_of_dics == BLACK)
                            black_score += 1000000;
                        continue;
                    }else{
                        if(dead) continue;
                        else dead = true;
                    }
                    if(dead){
                        if(cnt == 2) black_score += 10;
                        else if(cnt == 3) black_score += 100;
                        else if(cnt == 4) black_score += 1000;
                    }else{
                        if(cnt == 1) black_score += 10;
                        else if(cnt == 2) black_score += 100;
                        else if(cnt == 3) black_score += 10000;
                        else if(cnt == 4) black_score += 100000;
                    }
                }
            }else if(board[i][j] == WHITE){
                for(int k=0; k<8; k++){
                    int try_x = i , try_y = j, cnt = 0;
                    bool dead = false;
                    int first = 0;
                    while(try_x >= 0 && try_y >= 0 && try_x < SIZE && try_y < SIZE && board[try_x][try_y] == WHITE){
                        int disc_at_middle_x = try_x - dir[k][0]; 
                        int disc_at_middle_y = try_y - dir[k][1];
                        if(disc_at_middle_x >= 0 && disc_at_middle_y >= 0 && disc_at_middle_x < SIZE && disc_at_middle_y < SIZE){
                            if(board[disc_at_middle_x][disc_at_middle_y] == WHITE && first == 0) break;
                            else if(board[disc_at_middle_x][disc_at_middle_y] == BLACK) dead = true;
                            first++;
                        }else dead = true;
                        try_x += dir[k][0];
                        try_y += dir[k][1];
                        cnt++;
                        if(cnt == 5) break;
                    }
                    if(cnt == 0) continue;
                    if(cnt != 5 && try_x >= 0 && try_y >= 0 && try_x < SIZE && try_y < SIZE){
                        if(board[try_x][try_y] == BLACK){
                            if(dead) continue;
                            else dead = true;
                        }
                    }else if(cnt == 5){
                        white_score += 1000000;
                            if(color_of_dics == WHITE)
                                white_score += 1000000;
                        continue;
                    }else{
                        if(dead) continue;
                        else dead = true;
                    }
                    if(dead){
                        if(cnt == 2) white_score += 10;
                        else if(cnt == 3) white_score += 100;
                        else if(cnt == 4) white_score += 1000;
                    }else{
                        if(cnt == 1) white_score += 10;
                        else if(cnt == 2) white_score += 100;
                        else if(cnt == 3) white_score += 10000;
                        else if(cnt == 4) white_score += 100000;
                    }
                }
            }
        }
    }
    return {black_score, white_score};
}
std::pair<int, std::pair<int, int>> dfs_try_pos(int depth, int parent_disc_color){
    int best_score = parent_disc_color == BLACK ? INT_MIN : INT_MAX;
    std::pair<int, int> best_pos;
    if(depth == dfs_layer){
        std::pair<int, int> cur_score = eval_tried_board_score();
        best_score = cur_score.first-cur_score.second;
        return {best_score, best_pos} ;
    }

    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(board[i][j] == EMPTY){
                board[i][j] = parent_disc_color;
                std::pair<int, std::pair<int, int>> next_try = dfs_try_pos(depth+1, color_flip(parent_disc_color));
                if(parent_disc_color == BLACK){
                    if(best_score < next_try.first){
                        best_score = next_try.first;
                        best_pos = {i, j};
                    }
                }else if(parent_disc_color == WHITE){
                    if(best_score > next_try.first){
                        best_score = next_try.first;                   
                        best_pos = {i, j};
                    }
                }
                board[i][j] = EMPTY;
            }
        }
    }
    return {best_score, best_pos};
}
std::pair<int, int> rec_first_black_disc;
void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if(board[i][j] == BLACK){
                black_num++;
                rec_first_black_disc.first = i;
                rec_first_black_disc.second = j;
            }
            if(board[i][j] == WHITE){
                white_num++;
            }
        }
    }
    if(black_num == white_num) 
        color_of_dics = BLACK;
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        if(black_num == 0){
            std::pair<int, int> first = first_random();
            x = first.first;
            y = first.second;
        }else if(white_num == 0){
            x = rec_first_black_disc.first;
            y = rec_first_black_disc.second;
            int t = (rand()%8);
            x += dir[t][0];
            y += dir[t][1];
        }else{
            std::pair<int, std::pair<int, int>> next_try = dfs_try_pos(1, color_of_dics);
            std::cout << "score:" <<next_try.first <<'\n';
            x = next_try.second.first;
            y = next_try.second.second;
            std:: cout << x <<' ' << y <<"\n";
            if(color_of_dics == BLACK){
                std::cout << "I'm BLACK!\n"; 
            }else{
                std::cout << "I'm WHITE!\n";
            }
        }
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
            break;
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    init_alpha_beta();
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
