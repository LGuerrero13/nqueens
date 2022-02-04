#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <algorithm>
#include <memory>
#include <random>
#include <chrono>
#include <functional>
#include <vector>
#include "../queen/queen.h"

using namespace std;

class board {
private:
    int board_size;
    vector<vector<shared_ptr<queen>>> chess_board;
    vector<shared_ptr<queen>> queens;
    unsigned int total_conflicts = 0;

    /* Generates a random row number */
    int get_random_int(int size);

    /* Returns row with the minimum amount of queens hit */
    int get_min_conflicting_row(int column);

    /* Change piece position and move us on the board aswell */
    void move_board_queen_to_row(shared_ptr<queen>& queen_to_set, int row);

public:

    /* Check if the current state of the board is a solution */
    bool is_final_state();

    /* Custom constructor */
    board(unsigned int size);

    /*
        This will setup our initial state.

        NOTE: The problem wants us to first place the queen of the leftmost column at a
        random row. Then we will place the next queen on a row in the leftmost empty column such that in
        the chosen row it has the minimum number of conflicts.
    */
    void initialization();

    /* Print the board to the user in a nice layout */
    void print_board();

    /* Returns the amount of queens hit for a specific position */
    int conflicts_found_for_position(int diag_col, int diag_row);

    /*
        1. Put queens that are in conflict in the queens_in_conflict vector.
        1a. If there are no more elements in the array, set is_final_state to true, and return true.
        2. Picks a random conflicting queen from the array
        3. Moves that queen to the min conflict row that isn't the current one.
        4. Repeats.
    */
    void search();

    /* Returns a vector of all queens that are currently in conflic */
    vector<shared_ptr<queen>> get_conflicting_queens();
};

#endif