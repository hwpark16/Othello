#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;
#include <vector>

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    vector<Move*> possibleMoves(Side side);
    int findScore(Side my_side, Side their_side, Move *possibleMove);
    Move* findBestMove(Side my_side, Side their_side, vector<Move*> possibleMoves);
};

#endif
