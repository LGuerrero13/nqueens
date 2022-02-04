#pragma once
#ifndef QUEEN_H
#define QUEEN_H

struct position {
    int column, row;
};

class queen {
public:
    position current_position;
};

#endif