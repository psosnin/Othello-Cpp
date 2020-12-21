#pragma once
#include  <unordered_map>
#include  <set>
#include  <string>

unsigned long flip_map_key(int player, int opponent, int move);

struct MoveTable {
    char moves[256*256];
    std::unordered_map<int, char> flip_map;
};

struct MoveTable* generate_move_table();

unsigned long get_move_board(unsigned long player, unsigned long opponent, char* move_table);

int move_array_index(int player, int opponent);

std::set<int> get_move_list(unsigned long move_board);

std::pair<int,int> get_xy_index(int bit);

std::pair<int,int> get_diagonal_index(std::pair<int, int> xy);

unsigned long get_flip_board(unsigned long player, unsigned long opponent, int move, std::unordered_map<int, char>  flip_map);

