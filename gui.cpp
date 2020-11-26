#include "gtkmm/window.h"
#include "gtkmm/button.h"
#include "gtkmm/grid.h"
#include "gui.h"
#include "gtkmm/box.h"
#include "gtkmm/label.h"
#include "gtkmm/image.h"
#include "gtkmm/overlay.h"
#include "game.h"
#include <set>
#include <string>
#include <iostream>
#include <stdlib.h>

Othello::Othello() {
  //initialise window
  set_title("Othello");
  set_size_request(660, 660);
  set_border_width(10);
  //initialise labels
  label_top.set_label("Welcome to Othello");
  label_player_to_move.set_label("Black to move");
  label_score.set_label("Black: 2 White: 2");
  //pack labels into top box and conficgure reset button
  reset_button.set_label("Reset");
  reset_button.set_can_focus(false);
  reset_button.signal_clicked().connect(sigc::mem_fun(*this, &Othello::resetFunction));
  top_box.pack_start(reset_button);
  top_box.pack_start(label_top);
  top_box.pack_start(label_player_to_move);
  top_box.pack_start(label_score);
  //initialise grid
  board_grid.set_border_width(0);
  board_grid.set_row_homogeneous(true);
  board_grid.set_column_homogeneous(true);
  //initialise background
  background.set("board.png");
  //add background and grid to overlay
  overlay.add(background);
  overlay.add_overlay(board_grid);
  //add overlay to eventbox and configure event
  event_box.add(overlay);
  event_box.set_events(Gdk::BUTTON_PRESS_MASK);
  event_box.signal_button_press_event().connect(sigc::mem_fun(*this, &Othello::onClicked));
  //pack label and event box into box
  main_box.pack_start(top_box,Gtk::PACK_SHRINK);
  main_box.pack_start(event_box);
  //initialise images and add them to grid
  for (int j = 0; j < 8; j ++){
    for (int i = 0; i < 8; i ++) {
      board_grid.attach(board_squares[i + 8*j], i, j);
    }
  }
  //add box to window and show all
  add(main_box);
  show_all_children();
  Othello::place(27,-1);
  Othello::place(36,-1);
  Othello::place(28,1);
  Othello::place(35,1);
  updateBoard();
}

bool Othello::onClicked(GdkEventButton* button_event)
{
      if (!game.game_ended) {
        int x = (int)((button_event->x)/80);
        int y = (int)((button_event->y)/80);
        int mv = 63 - x - 8*y;
        label_top.set_label(game.playMove(mv));
        updateBoard();
        if (game.getPlayer() == 1) {
          label_player_to_move.set_label("Black to move");
        }
        else {
          label_player_to_move.set_label("White to move");
        }
      }
      else {
        std::pair<int, int> score = game.getScore();
        if (score.first > score.second) {label_player_to_move.set_label("Black wins!");}
        else if (score.first < score.second) {label_player_to_move.set_label("White wins!");}
        else if (score.first == score.second) {label_player_to_move.set_label("Draw!");}
      }
      
    return true;
}

// a function that draws a piece of a specific colour on a square on the board given by mv
void Othello::place(int mv, int colour) {
  //white = -1, black = 1, move = 0, clear = 2
  if (colour == 1) {
    board_squares[63 - mv].set("black.png");
  }
  else if (colour == -1) {
    board_squares[63 - mv].set("white.png");
  }
  else if (colour == 0) {
    board_squares[63-mv].set("move.png");
  }
}

//a function that updates the board gui after a move has been made
void Othello::updateBoard() {
  for (int i : drawn_moves) {
    board_squares[63-i].clear();
  }
  drawn_moves.clear();
  int player = game.getPlayer();
  std::set<int> changed  = game.getBoardChange(-1*player);
  for (int i : changed) {place(i, -1*player);}
  std::set<int> current_moves = game.getMoveList();
  for (int i : current_moves) {
    drawn_moves.insert(i);
    place(i, 0);
    }
  label_score.set_label("Black: " + std::to_string(game.getScore().first) + " White: " + std::to_string(game.getScore().second));
}

//a simple strategy that plays the first move in the valid_move set
int Othello::strategyFirst(std::set<int> valid_moves) {
  if (valid_moves.empty()) {
    return -1;
  }
  else {
    return *valid_moves.begin();
  }
}

//a function that resets the gui and board state when a move is made
void Othello::resetFunction() {
  for (int j = 0; j < 64; j++){
      board_squares[j].clear();
  }
  game = Game();
  updateBoard();
  label_top.set_label("Welcome to Othello");
  label_player_to_move.set_label("Black to move");
  label_score.set_label("Black: 2 White: 2");
  Othello::place(27,-1);
  Othello::place(36,-1);
  Othello::place(28,1);
  Othello::place(35,1);
  
}