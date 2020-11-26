#include "bitboard_ops.h"
#include "move_ops.h"
#include "game.h"
#include <string>
#include <bitset>
#include <set>
#include <utility>
#include <iostream>
#include <cmath>

//initialises the starting game state
Game::Game() {
        player1 = 0x0000000810000000; //starting board for black
        player2 = 0x0000001008000000; //starting board for white
        past1 = 0x0000000810000000; //starting board for black
        past2 = 0x0000001008000000; //starting board for white
        move_count = 0;
        move_table = generate_move_table();
        updateMoveBoard();
        game_ended = false;
    }

//carries out the logic when the move passed in is requested and returns the results - ended, pass turn, made move and not a valid move
std::string Game::playMove(int move) {
    if (game_ended) {
        return "Game has ended";
    }
    else if (current_moves == 0) {
        move_count++;
        updateMoveBoard();
        return "Passing turn";
    }
    else if (((long)std::pow(2, move) & current_moves) != 0) {
        flip(move);
        move_count++;
        updateMoveBoard();
        return "Made move: " + std::to_string(move);
    }
    else {
        return "Not a valid move";
    }
}

//updates the current valid moves given the current board states
void Game::updateMoveBoard() {
    if (move_count % 2 == 0) {
        current_moves = get_move_board(player1, player2, move_table->moves);
        if (current_moves == 0 and get_move_board(player2, player1, move_table->moves) == 0) {
            game_ended = true;
        }
    }
    else {
        current_moves = get_move_board(player2, player1, move_table->moves);
        if (current_moves == 0 and get_move_board(player1, player2, move_table->moves) == 0) {
            game_ended = true;
        }
    }
    
}

//gets a list of all the possible moves in the current board state
std::set<int> Game::getMoveList(bool t) {
    if (t) {updateMoveBoard();}
    return get_move_list(current_moves);
}

//flips all of the pieces neccessary when a move is taken
void Game::flip(int move) {
    past1 = player1;
    past2 = player2;
    if (move_count % 2 == 0) {
        unsigned long flips = get_flip_board(player1, player2, move, move_table->flip_map);
        player2 &= ~flips;
        player1 |= flips;
    }
    else {
        unsigned long flips = get_flip_board(player2, player1, move, move_table->flip_map);
        player1 &= ~flips;
        player2 |= flips;
    }
}

//prints the board state to the command line
void Game::print() {
    pprint_board(player1, player2, current_moves);
}

//returns the player whos move it is 
int Game::getPlayer() {
    //0 for black, 1 for white
    if (move_count % 2 == 0) {
        return 1;
    }
    else {
        return -1;
    }
}

//returns a set of all the pieces that were flippled or placed on the previous turn
std::set<int> Game::getBoardChange(int player) {
    if (player == 1) {
        return get_move_list(player1 & ~past1);
    }
    else {
        return get_move_list(player2 & ~past2);
    }
}

//returns the current number of pieces on the board in a pair (black, white)
std::pair<int, int> Game::getScore() {
    int black = (std::bitset<64> (player1)).count();
    int white = (std::bitset<64> (player2)).count();
    return std::pair<int, int> (black, white);
}
