//Kinam Park's Commit for points
#include "player.hpp"
using namespace std;
#include <vector> 

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    board = new Board();
    my_side = side;

    if (my_side == BLACK)
    {
    	their_side = WHITE;
    }

    else if (my_side == WHITE)
    {
    	their_side = BLACK;
    }

}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    board->doMove(opponentsMove, their_side);

    if (board->hasMoves(my_side))
    {
    	vector<Move*> moves = board->possibleMoves(my_side);

    	Move *best_move = board->findBestMove(my_side, their_side, moves);
	    board->doMove(best_move, my_side);

	    return best_move;
	}

	return nullptr;
}
