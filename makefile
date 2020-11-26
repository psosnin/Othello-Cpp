#paths to library code
CXX = g++ -std=c++20 `pkg-config gtkmm-3.0 --cflags --libs`

CXXFLAGS = -Wall -g 

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o bitboard_ops.o move_ops.o game.o gui.o
	$(CXX) $(CXXFLAGS) -o main main.o move_ops.o bitboard_ops.o game.o gui.o

# The main.o target can be written more simply

main.o: main.cpp bitboard_ops.h move_ops.h game.h gui.h
	$(CXX) $(CXXFLAGS) -c main.cpp

gui.o: gui.h

move_ops.o: move_ops.h

game.o: game.h

bitboard_ops.o: bitboard_ops.h 

# Standard C++ Makefile rules:
clean:
	rm -rf $(EXE) $(TEST) $(OBJS_DIR) $(CLEAN_RM) *.o *.d main

tidy: clean
	rm -rf doc

.PHONY: all tidy clean
