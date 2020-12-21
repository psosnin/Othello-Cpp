
#pragma once
#include <set>
#include <string>
#include <utility>
class Game
{ 
    // Access specifier 
    public: 
        Game();
        void print();
        void updateMoveBoard();
        std::set<int> getMoveList(bool t = false);
        std::string playMove(int m);
        bool game_ended;
        int returnWinner();
        int getPlayer();
        std::set<int> getBoardChange(int player);
        std::pair<int, int> getScore();
    private:
        int move_count;
        unsigned long current_moves;
        unsigned long player1;
        unsigned long player2; 
        unsigned long past1;
        unsigned long past2;
        struct MoveTable* move_table;
        void flip(int move);
    };
