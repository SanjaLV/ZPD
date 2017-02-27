#include "Pylos.h"
#include "Agent.h"
#include <vector>



class RandomAgent : public Agent {
	public :
		virtual long long getMove ( long long current_state );
		virtual std::string getName();
};


long long RandomAgent::getMove ( long long current_state ) {
	std::vector<long long> moves = pylos::getValidMoves(current_state);
	return moves[ rand() % moves.size() ];
}

std::string RandomAgent::getName ( ) {
	return ( "RandomAgent" );
}
