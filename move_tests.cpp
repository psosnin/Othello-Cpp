#include <iostream>
#include <bitset>

int main() {
    unsigned long white = 0b00010010;
    unsigned long black = 0b11100100;
    std::cout << "Black:            " << std::bitset<8>(black) << std::endl;
    std::cout << "White:            " << std::bitset<8>(white) << std::endl;
    std::cout << "Black >> 1:       " << std::bitset<8>(black >> 1) << std::endl;
    std::cout << "White Neighbours: " << std::bitset<8>((black >> 1) & white) << std::endl;
    std::cout << "Possible moves:   " << std::bitset<8>(((black >> 1) & white)>>1) << std::endl;
    std::cout << "Moves not blocked:" << std::bitset<8>((((black >> 1) & white)>>1)&(~(white | black))) << std::endl;
}
