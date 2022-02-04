#include "board.h"

board::board(unsigned int size) :
    board_size(size),
    chess_board(size, vector<shared_ptr<queen>>(size)),
    queens(size) {
        
    if (board_size <= 3 && board_size != 1) throw invalid_argument("sizes lower than 4 and not equal to 1 are unsolvable!");
    
    for (auto& q : queens) {
        q = make_shared<queen>();
    }
}

int board::get_random_int(int size) {
    default_random_engine generator; // create instance of random engine
    generator.seed(chrono::system_clock::now().time_since_epoch().count()); // set a unique seed using chrono every call to get a more random #
    uniform_int_distribution<int> distribution(0, size - 1);  // get random # from 0 - size
    return distribution(generator);
}

int board::get_min_conflicting_row(int column)
{
    /* we set min_conflict_row_conflicts to the max, because we will find the lowest possible value */
    int min_conflict_board_row = 0, min_conflict_row_conflicts = INT_MAX;

    for (int board_row = 0; board_row < board_size; board_row++) {

        /* record how many conflicts were encountered for this possible position */
        int current_conflicts = conflicts_found_for_position(column, board_row);
        total_conflicts += current_conflicts;

        /* set min conflict board row */
        if (current_conflicts < min_conflict_row_conflicts) {
            min_conflict_board_row = board_row;
            min_conflict_row_conflicts = current_conflicts;
        }
    }
    return min_conflict_board_row;
}

void board::initialization() {

    /* loop through each column */
    for (int board_column = 0; board_column < board_size; board_column++) {

        /* First queen can always go in any row since there are no queens placed */
        if (board_column == 0) {

            /* get random row */
            int random_row = get_random_int(board_size);

            /* store their position */
            queens[board_column]->current_position = { board_column, random_row };
            chess_board[board_column][random_row] = queens[board_column];
        }
        else {

            int row = get_min_conflicting_row(board_column);

            /* store their position  */
            queens[board_column]->current_position = { board_column, row };
            chess_board[board_column][row] = queens[board_column]; // put us on the board
        }
    }
    printf("conflicts encountered in initial state: %d\n", total_conflicts);
    print_board(); // print initial state
}

void board::print_board() {
    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {

            if (chess_board[col][row] != nullptr)
                printf("\033[0;33mQ\033[0m  ");
            else
                printf("0  ");
        }
        printf("\n");
    }
    printf("\n");
}

int board::conflicts_found_for_position(int diag_col, int diag_row) {

    int result = 0;

    /*
        vector of lambda functions that will
        be used as the advancement to the next iteration
        so that we can check all the directions we need to check
    */
    vector<function<void(int&, int&)>> direction_list{
        [](int& col, int& row) { ++col; }, // right
        [](int& col, int& row) { --col; }, // left
        [](int& col, int& row) { ++row; }, // down
        [](int& col, int& row) { --row; }, // up

        /* diagonals */
        [](int& col, int& row) { ++col; --row; },  // up right
        [](int& col, int& row) { --col; ++row; },  // down left
        [](int& col, int& row) { --col; --row; },  // up left
        [](int& col, int& row) { ++col; ++row; }   // down right
    };

    for (auto direction : direction_list) {
        for (int col = diag_col, row = diag_row; (col < board_size && col > -1) && (row < board_size && row > -1); direction(col, row)) {
            if (col == diag_col && row == diag_row) continue;
            /* check if there is someone in the top right diag */
            if (col != diag_col && chess_board[col][row] != nullptr) ++result;
        }
    }
    return result;
}

void board::move_board_queen_to_row(shared_ptr<queen>& queen_to_set, int row) {
    chess_board[queen_to_set->current_position.column][queen_to_set->current_position.row] = nullptr; // remove us on the board
    queen_to_set->current_position.row = row;
    chess_board[queen_to_set->current_position.column][row] = queen_to_set; // put us on the board
}

void board::search() {

    int steps = 0;

    for (auto queens_in_conflict = get_conflicting_queens(); queens_in_conflict.size() > 0; queens_in_conflict = get_conflicting_queens()) {
        auto random_queen = queens_in_conflict[get_random_int(queens_in_conflict.size())]; // choose a random queen that is in conflic from the vector
        int min_row = get_min_conflicting_row(random_queen->current_position.column);

        /* 
            do not use same row that we're already on, so choose a random row,
            else use the new row that has the least coflicts
        */
        if (min_row == random_queen->current_position.row) {
            move_board_queen_to_row(random_queen, get_random_int(board_size));
        }
        else {
            move_board_queen_to_row(random_queen, min_row);
        }
        printf("moving queen #%d to row #%d!\n", random_queen->current_position.column + 1, random_queen->current_position.row + 1);
        print_board();
        ++steps; /* increament the steps we take */
    }
    printf("\033[0;32mIt took\033[0m \033[0;31m%d\033[0m \033[0;32msteps to find the solution!\033[0m\n", steps);
}

bool board::is_final_state() {
    int conflicts_found = 0;
    for (auto& q : queens) {
        conflicts_found += conflicts_found_for_position(q->current_position.column, q->current_position.row);
    }
    return conflicts_found == 0;
}

vector<shared_ptr<queen>> board::get_conflicting_queens() {

    vector<shared_ptr<queen>> queens_in_conflict;

    for (shared_ptr<queen>& q : queens) {
        if (conflicts_found_for_position(q->current_position.column, q->current_position.row) > 0) {
            queens_in_conflict.emplace_back(q);
        }
    }
    return queens_in_conflict;
}