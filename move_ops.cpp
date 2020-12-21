#include <iostream>
#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <bit>
#include "bitboard_ops.h"


//a function that returns a key into the flip_map given a black row, a white row and a move
unsigned long flip_map_key(int player, int opponent, int move) {
        return (player&0xff) << 16 | (opponent&0xff) << 8 | (move&0xff);
}

//a structure holding the precomputed move data. An array holds the move information and a map holds the flipped pieces for every move
struct MoveTable {
    char moves[256*256];
    std::unordered_map<int, char> flip_map;
};

//precomputes the possible valid moves and their corresponding flips for any row state given by player and opponent
void compute_moves(unsigned int player, unsigned int opponent, struct MoveTable* move_table) {
    //black to move
    std::bitset<8> pl = player;
    std::bitset<8> op = opponent;
    std::bitset<8> move;
    std::bitset<8> flipped = 0;

    for (int i = 0; i < 8; i++) {
        if (pl[i] == 1 and pl[i+1] == 0 and op[i+1] == 1) {
            flipped.reset();
            for (int j = i + 1; j < 8; j++) {
                flipped[j] = 1;
                if (op[j] == 0) {
                    flipped[j] = 0;
                    move[j] = 1;
                    move_table->flip_map[flip_map_key(pl.to_ulong(), op.to_ulong(), std::pow(2,j))] |= flipped.to_ulong();
                    break;
                }
            }
            
        }
    }
    for (int i = 7; i > 0; i--) {
        if (pl[i] == 1 and pl[i-1] == 0 and op[i-1] == 1) {
            flipped.reset();
            for (int j = i - 1; j >= 0; j--) {
                flipped[j] = 1;
                if (op[j] == 0) {
                    flipped[j] = 0;
                    move[j] = 1;
                    move_table->flip_map[flip_map_key(pl.to_ulong(), op.to_ulong(), std::pow(2,j))] |= flipped.to_ulong();
                    break;
                }
            }
            
        }
    }
    move &= ~(player | opponent);
    move_table->moves[(player<<8) | opponent] = (char)move.to_ulong();
}

//precomputes the move table for all moves for every possible configuration of white and black counters on a row. The table is indexed by a 16 bit number -> the first 8 bits are the player's row state, the last 8 are the opponent's row state
struct MoveTable* generate_move_table() {
    struct MoveTable* move_table = new struct MoveTable;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            compute_moves(i,j,move_table);
        }
    }
    return move_table;
}

//generates an index into the move table
int move_array_index(int player, int opponent){return (player << 8) | opponent ;}

//gets all possible moves on the board in the hoirizontal direction
unsigned long get_H_moves(unsigned long player, unsigned long opponent, char* move_table){
    //generate move for player given the board states stored in player and opponent
    unsigned long moves = 0;
    for (int i = 0; i < 8; i++) {
        moves |= ((unsigned long)move_table[move_array_index(((player >> 8*i) & 0xff), ((opponent >> 8*i) & 0xff))] & 0xff) << i*8;
    }
    return moves & (~(player | opponent));
}

//gets all possible moves on the board in the vertical direction
unsigned long get_V_moves(unsigned long player, unsigned long opponent, char* move_table){
    return rotAC90(get_H_moves(rotC90(player), rotC90(opponent), move_table));
}

//gets all possible moves on the board in the direction of the leading diagonal
unsigned long get_D_moves(unsigned long player, unsigned long opponent, char* move_table) {
    unsigned long rplayer = rotAC45(player);
    unsigned long ropponent = rotAC45(opponent); 
    unsigned int shift[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
    unsigned int mask[11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};
    unsigned long moves = 0;
    for (int i = 0; i < 11; i++ ) {
            unsigned int m = mask[i];
            unsigned int s = shift[i];
            moves |= (((unsigned long)move_table[move_array_index((rplayer >> s) & m, (ropponent >> s)   & m)] & m) << s);
            }
    return rotC45(moves) & (~(player | opponent));
}

//gets all the possible moves on the board in the direction of the anti-diagonal
unsigned long get_AD_moves(unsigned long player, unsigned long opponent, char* move_table) {
    return flipV(get_D_moves(flipV(player), flipV(opponent), move_table));
}

//gets all the possible moves on the board
unsigned long get_move_board(unsigned long player, unsigned long opponent, char* move_table) {
    return  get_V_moves(player, opponent, move_table) | get_H_moves(player, opponent, move_table) | get_D_moves(player, opponent, move_table) | get_AD_moves(player, opponent, move_table);
}

//a function that decomposes a move board into a set of possible moves given by the log of there position in the bitboard
std::set<int> get_move_list(unsigned long move_board) {
    std::set<int> move_indices;
     while (move_board != 0){
        unsigned long current_move = std::bit_floor(move_board);
        //std::cout << bin(current_move) << std::endl;
        move_board = move_board - current_move;
        move_indices.insert(std::log2(current_move));
    } 
    return move_indices;
}

//a function that takes the bit of a bitboard and returns the x,y index MSB (0,0) 
std::pair<int,int> get_xy_index(int bit) {
    return std::pair<int,int>((63-bit)%8, (63-bit)/8);
}

//a function that takes an (x,y) pair and returns a pair of (diagonal, antidiagonal) indices.
std::pair<int,int> get_diagonal_index(std::pair<int, int> xy) {
    return std::pair<int,int> ((5 + xy.second - xy.first), (-2 + xy.second + xy.first));
}


//a function that returns a bitboard of opponents pieces that are flipped on a move. player and opponent are the current bitboards, move is an integer value representing the number of the bit that the move is on, flip mop is the map of all moves
unsigned long get_flip_board(unsigned long player, unsigned long opponent, int move, std::unordered_map<int, char>  flip_map){
    // get x, y, d and ad
    std::pair<int,int> xy = get_xy_index(move);
    std::pair<int,int> diag = get_diagonal_index(xy);
    unsigned long move_board = std::pow(2,move);
    //set row
    unsigned long flip_board  = set_row(0, flip_map[flip_map_key(get_row(player, xy.second), get_row(opponent, xy.second), get_row(move_board, xy.second))], xy.second)
                | set_column(0, flip_map[flip_map_key(get_column(player, xy.first), get_column(opponent, xy.first), get_column(move_board,xy.first))], xy.first)
                | set_diagonal(0, flip_map[flip_map_key(get_diagonal(player, diag.first), get_diagonal(opponent, diag.first), get_diagonal(move_board,diag.first))], diag.first)
                | set_anti_diagonal(0, flip_map[flip_map_key(get_anti_diagonal(player, diag.second), get_anti_diagonal(opponent, diag.second), get_anti_diagonal(move_board,diag.second))], diag.second);
    if (flip_board == 0) {
        return flip_board;
    }
    else {
        return flip_board | move_board;
    }
        
}