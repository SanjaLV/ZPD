#pragma once
#include <string>

class Agent {
	public :
		virtual long long getMove ( long long current_state ) = 0;
		virtual void Clear ( bool isWhite );
		virtual std::string getName() = 0;
};

void Agent::Clear( bool isWhite ) {
}
