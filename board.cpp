#include "board.hpp"
using namespace std;
#include <iostream>

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);

}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

vector<Move*> Board::possibleMoves(Side side)
{
    vector<Move*> possibleMoves;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i,j);
            if (checkMove(move, side))
            {
                possibleMoves.push_back(move);
            }
        }
    }
    return possibleMoves;
}

int Board::findScore(Side my_side, Side their_side, Move *possibleMove)
{
    copy()->doMove(possibleMove, my_side);

    int score = count(my_side) - count(their_side);

    int X = possibleMove->getX();
    int Y = possibleMove->getY();

    // Corners
    if (X == 0 && Y == 0)
    {
        score *= 3;
    }
    else if (X == 0 && Y == 7)
    {
        score *= 3;
    }
    else if (X == 7 && Y == 0)
    {
        score *= 3;
    }
    else if (X == 7 && Y == 7)
    {
        score *= 3;
    }

    // Edges
    else if (X == 0 && Y == 2)
    {
        score *= 1.5;
    }
    else if (X == 0 && Y == 3)
    {
        score *= 1.5;
    }
    else if (X == 0 && Y == 4)
    {
        score *= 1.5;
    }
    else if (X == 0 && Y == 5)
    {
        score *= 1.5;
    }

    else if (X == 2 && Y == 0)
    {
        score *= 1.5;
    }
    else if (X == 3 && Y == 0)
    {
        score *= 1.5;
    }
    else if (X == 4 && Y == 0)
    {
        score *= 1.5;
    }
    else if (X == 5 && Y == 0)
    {
        score *= 1.5;
    }

    else if (X == 2 && Y == 7)
    {
        score *= 1.5;
    }
    else if (X == 3 && Y == 7)
    {
        score *= 1.5;
    }
    else if (X == 4 && Y == 7)
    {
        score *= 1.5;
    }
    else if (X == 5 && Y == 7)
    {
        score *= 1.5;
    }

    else if (X == 7 && Y == 2)
    {
        score *= 1.5;
    }
    else if (X == 7 && Y == 3)
    {
        score *= 1.5;
    }
    else if (X == 7 && Y == 4)
    {
        score *= 1.5;
    }
    else if (X == 7 && Y == 5)
    {
        score *= 1.5;
    }
    
    // Adjacent to corners
    else if (X == 0 && Y == 1)
    {
        score *= -3;
    }
    else if (X == 1 && Y == 0)
    {
        score *= -3;
    }
    else if (X == 1 && Y == 1)
    {
        score *= -3;
    }
    
    else if (X == 0 && Y == 6)
    {
        score *= -3;
    }
    else if (X == 1 && Y == 6)
    {
        score *= -3;
    }
    else if (X == 1 && Y == 7)
    {
        score *= -3;
    }
    
    else if (X == 6 && Y == 0)
    {
        score *= -3;
    }
    else if (X == 6 && Y == 1)
    {
        score *= -3;
    }
    else if (X == 7 && Y == 1)
    {
        score *= -3;
    }

    else if (X == 6 && Y == 6)
    {
        score *= -3;
    }
    else if (X == 6 && Y == 7)
    {
        score *= -3;
    }
    else if (X == 7 && Y == 6)
    {
        score *= -3;
    }

    return score;

}

Move* Board::findBestMove(Side my_side, Side their_side, vector<Move*> possibleMoves)
{
    int max_score = -1000000000;
    Move *best_move;

    for (unsigned int i = 0; i < possibleMoves.size(); i++)
    {
        int new_score = findScore(my_side, their_side, possibleMoves[i]);

        if (new_score > max_score)
        {
            max_score = new_score;
            best_move = possibleMoves[i];
        }
    }
    cerr << best_move << endl;
    return best_move;
}