#include "board/board.h"

/*
    Notes:
        - This is a N queen problem where the board is n x n, since we've got 8 queens
        the board will be 8 x 8.

        - The goal of the 8-queens problem is to place eight queens on a chessboard such that no queens
        attack any other (A queen attacks any piece in the same row, column, or diagonal).
*/

int main() {
    board eight_queens(16);
    eight_queens.initialization();
    eight_queens.search();
    return 0;
}

