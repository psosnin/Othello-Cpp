#pragma once
#include "gtkmm/window.h"
#include "gtkmm/button.h"
#include "gtkmm/grid.h"
#include "gtkmm/box.h"
#include "gtkmm/label.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/image.h"
#include "gtkmm/overlay.h"
#include "game.h"
#include <set>


class Othello : public Gtk::Window {
    public:
        Othello();
        void place(int mv, int colour);
        Game game = Game();
        void updateBoard();

    protected:
        void resetFunction();
        int strategyFirst(std::set<int> valid_moves);
        std::set<int> drawn_moves;
        bool onClicked(GdkEventButton* button_event);
        Gtk::Image board_squares[64];
        Gtk::Image background;
        Gtk::EventBox event_box;
        Gtk::Button reset_button;
        Gtk::Label label_score;
        Gtk::Label label_top; 
        Gtk::Label label_player_to_move; 
        Gtk::HBox top_box;
        Gtk::Grid board_grid;
        Gtk::VBox main_box;
        Gtk::Overlay overlay;
};