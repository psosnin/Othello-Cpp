#include "bitboard_ops.h"
#include "move_ops.h"
#include "game.h"
#include <iostream>
#include <set>


int main() {
    Game game = Game();
    while (!game.game_ended) {
        game.print();

        std::set<int> valid_moves = game.getMoveList();
        for (int i : valid_moves) {std::cout << i << " ";}
        std::cout << std::endl;

        std::cout << "Black to move: "<< std::endl;

        int move;
        if(std::cin >> move) {
            game.playMove(move);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        

        
        game.print();

        valid_moves = game.getMoveList();
        for (int i : valid_moves) {std::cout << i << " ";}
        std::cout << std::endl;

        std::cout << "White to move: "<< std::endl;
        if(std::cin >> move) {
            game.playMove(move);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
    }
    

}