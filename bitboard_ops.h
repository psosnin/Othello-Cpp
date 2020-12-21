#pragma once

void print(unsigned long b);

void pprint(unsigned long b, char c = 'o');

void pprint_board(unsigned long black, unsigned long white, unsigned long moves = 0);

unsigned long flipV(unsigned long b); 

unsigned long flipH(unsigned long b);

unsigned long flipD(unsigned long b);

unsigned long flipAD(unsigned long b);

unsigned long place(unsigned long b, int x, int y);

unsigned long rotC90(unsigned long b);

unsigned long rotAC90(unsigned long b);

unsigned long rotC45(unsigned long b);

unsigned long rotAC45(unsigned long b);

unsigned int get_diagonal(unsigned long b, unsigned int x);

unsigned int get_anti_diagonal(unsigned long b, unsigned int x);

unsigned long set_diagonal(unsigned long b, unsigned long d, unsigned int x);

unsigned long set_anti_diagonal(unsigned long b, unsigned long d, unsigned int x);

unsigned int get_row(unsigned long b, unsigned int x);

unsigned int get_column(unsigned long b, unsigned int x);

unsigned long set_row(unsigned long b, unsigned long d, unsigned int x);

unsigned long set_column(unsigned long b, unsigned long d, unsigned int x);