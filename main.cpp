#include <array>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cassert>
#define TO_HUMAN(x) (((char) (x[0] + 'a')) + to_string(8 - x[1]))
using namespace std;
vector<array<int, 2>> HUNTER_MOVES = {{ -1, -1 }, { 1, -1 }};
vector<array<int, 2>> RABBIT_MOVES = {{ -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }};
typedef array<array<int, 2>, 5> Board;
string gen_key(Board board, bool hunter_turns) {
    int i, j;
    string key;
    sort(board.begin() + 1, board.end());
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 2; j++)
            key += to_string(board[i][j]) + " ";
    }
    if(hunter_turns)
        key += "1";
    else
        key += "0";
    return key;
}
bool can_step(int nx, int ny, const Board &board) {
    int i;
    if(!(0 <= nx && nx < 8 && 0 <= ny && ny < 8))
        return false;
    for(i = 0; i < 5; i++) {
        if(board[i][0] == nx && board[i][1] == ny)
            return false;
    }
    return true;
}
vector<Board> find_legal_moves(const Board &board, bool hunter_turns) {
    int i, nx, ny;
    vector<Board> lm;
    for(i = (hunter_turns ? 1 : 0); i < (hunter_turns ? 5 : 1); i++) {
        for(const auto &x : (hunter_turns ? HUNTER_MOVES : RABBIT_MOVES)) {
            nx = board[i][0] + x[0];
            ny = board[i][1] + x[1];
            if(!can_step(nx, ny, board))
                continue;
            lm.push_back(board);
            lm.back()[i][0] = nx;
            lm.back()[i][1] = ny;
        }
    }
    return lm;
}
bool engine(const Board &board, bool hunter_turns, unordered_map<string, bool> &dp) {
    bool ans;
    string key = gen_key(board, hunter_turns);
    auto found = dp.find(key);
    if(found == dp.end()) {
        if(board[0][1] < 7) {
            auto lm = find_legal_moves(board, hunter_turns);
            if(hunter_turns) {
                ans = false;
                for(const auto &x : lm) {
                    if(engine(x, false, dp)) {
                        ans = true;
                        break;
                    }
                }
            }
            else {
                ans = true;
                for(const auto &x : lm) {
                    if(!engine(x, true, dp)) {
                        ans = false;
                        break;
                    }
                }
            }
        }
        else
            ans = false;
        dp[key] = ans;
    }
    else
        ans = found->second;
    return ans;
}
Board find_best_move(const Board &board, unordered_map<string, bool> &dp) {
    auto lhm = find_legal_moves(board, true);
    for(const auto &x : lhm) {
        if(engine(x, false, dp))
            return x;
    }
    assert(false);
}
string find_board_delta(const Board &b1, const Board &b2) {
    int i;
    for(i = 0; i < 5; i++) {
        if(b1[i] != b2[i])
            return (TO_HUMAN(b1[i]) + " => " + TO_HUMAN(b2[i]));
    }
    return "SAME";
}
int main() {
    int rdx, rdy;
    unordered_map<string, bool> dp;
    Board board;
    board[0] = { 3, 0 };
    board[1] = { 0, 7 };
    board[2] = { 2, 7 };
    board[3] = { 4, 7 };
    board[4] = { 6, 7 };
    while(true) {
        auto lrm = find_legal_moves(board, false);
        if(lrm.empty()) {
            cout << "Game over, the hunters won.\n";
            break;
        }
        do {
            cout << "Where does the rabbit move?\n";
            cin >> rdx >> rdy;
        }
        while(all_of(lrm.begin(), lrm.end(), [&](const auto &a) { return !(a[0][0] == board[0][0] + rdx && a[0][1] == board[0][1] + rdy); }));
        board[0][0] += rdx;
        board[0][1] += rdy;
        cout << "Thinking...\n";
        auto best_move = find_best_move(board, dp);
        cout << find_board_delta(board, best_move) << '\n';
        board = best_move;
    }
    return 0;
}

