#include "Pylos.h"
#include "Agent.h"
#include <vector>

using namespace std;



namespace RandomPick { 
	long long getMove ( long long curent_state ) {
		vector<long long> moves = pylos::getValidMoves(curent_state);
		return moves[ rand() % moves.size() ];
	}
}


	
struct TreeNode {
	long long State;
	int parent;
	int games;
	int white_win;
	int black_win;
	vector<int> edges;		
};


class MonteCarloTreeSearchAgent : public Agent {
	private :
		int SelectionCount = 10;
	    int ExpansionCount = 10;
	    int SimulationCount = 10;
	    vector<TreeNode> Tree;
	    int newNode( long long State , int p );
	    int V;
	    bool isWhite;
	    double evaluate ( int v );
	    void Simulate ( int v );
	    void Expand ( int v );
	    void GoTo ( long long s );
	    int Select ( int v );
	    
	
	public : 
		virtual long long getMove( long long current_state );
		virtual string getName();
		virtual void Clear( bool isWhite );
		void SetConstants ( int SE , int EX , int SI );
	
	
};


long long MonteCarloTreeSearchAgent::getMove( long long s ) {
		GoTo ( s );
		Expand ( V );
		
		for ( int i = 0 ; i < SelectionCount ; i++ ) {
			int v = Select ( V );
			for ( int j = 0 ; j < ExpansionCount ; j++ )
				Expand ( v );
		}
		
		double best = evaluate ( Tree[V].edges[0]);
		int bestEdge = 0;
		
		for ( int i = 1 ; i < Tree[V].edges.size() ; i++ ) {
			double cur = evaluate ( Tree[V].edges[i]);
			if ( cur > best ) {
				best = cur;
				bestEdge = i;
			}
			
		}
		V = Tree[V].edges[bestEdge];
		return Tree[V].State;
		
	}


void MonteCarloTreeSearchAgent::GoTo ( long long s ) {
		if ( s == Tree[V].State )
			return;
		
		
		for ( int i = 0 ; i < Tree[V].edges.size() ; i++ ) {
			if ( Tree[ Tree[V].edges[i] ].State == s ) {
				V = Tree[V].edges[i];
				return;
			}
		}
		
		Tree.clear();
		V = newNode ( s , -1 );
		
}
	
int MonteCarloTreeSearchAgent::Select ( int v ) {
		while ( Tree[v].edges.size() != 0 ){
			v = Tree[v].edges[ rand() % ( Tree[v].edges.size() ) ] ;
		}
		
		return v;
	}


void MonteCarloTreeSearchAgent::Expand ( int v ) {
		
		pylos::Info info = pylos::getInfo ( Tree[v].State );
		
		if ( info.gameState == pylos::WHITE_WIN ) {
			Tree[v].games += SimulationCount;
			Tree[v].white_win += SimulationCount;
			return;
		}
		if ( info.gameState == pylos::BLACK_WIN ) {
			Tree[v].games += SimulationCount;
			Tree[v].black_win += SimulationCount;
			return;
		}
		vector <long long> moves = pylos::getValidMoves ( Tree[v].State );
		int i = 0;
		while ( i < moves.size() ) {	
			bool fail = false;
			for ( int j = 0 ; j < Tree[v].edges.size() ; j++ ) {
				if ( Tree[ Tree[v].edges[j] ].State == moves[i] ) {
					fail = true;
					break;
				}
			}
			if ( fail ) {
				moves[i] = moves [ moves.size() - 1];
				moves.pop_back();
			}
			else 
			 i++;
		}
		
		i = 0;
		while ( i < moves.size() && i < 3 ) {
			int r = rand() % moves.size();
			Tree[v].edges.push_back ( newNode(moves[r] , v ) );
			moves[r] = moves [ moves.size() - 1 ];
			moves.pop_back();
		}
		Simulate ( Tree[v].edges[ rand() % Tree[v].edges.size()  ] );		
}



void MonteCarloTreeSearchAgent::Simulate ( int v ) {
		int ww = 0;
		int bw = 0 ; 
		for ( int i = 0 ; i < SimulationCount ; i++ ) {
			long long s = Tree[v].State;
			while ( true ) {
				s = RandomPick::getMove( s );
				pylos::Info info = pylos::getInfo(s) ; 
				if ( info.gameState == pylos::WHITE_WIN ) {
					ww++;
					break;
				}
				else if ( info.gameState == pylos::BLACK_WIN ) {
					bw++;
					break;
				}
			}
		}
		while ( v != -1 ) {
			Tree[v].games += SimulationCount;
			Tree[v].white_win += ww;
			Tree[v].black_win += bw;
			v = Tree[v].parent;
		}
}

double MonteCarloTreeSearchAgent::evaluate ( int v ) {
	if ( Tree[v].games == 0 )
			return 0.0;
		if ( isWhite )
			return ( double(Tree[v].white_win) / Tree[v].games );
		else
			return ( double(Tree[v].black_win) / Tree[v].games );
	
}

int MonteCarloTreeSearchAgent::newNode( long long State , int p ) {
		TreeNode res;
		res.State = State;
		res.games = 0;
		res.white_win = 0;
		res.black_win = 0;
		res.edges.clear();
		res.parent = p ;
		Tree.push_back ( res ) ; 
		return Tree.size() -1;
	}

void MonteCarloTreeSearchAgent::SetConstants ( int SE , int EX , int SI ) {
	SelectionCount = SE;
	ExpansionCount = EX;
	SimulationCount = SI;
}

void MonteCarloTreeSearchAgent::Clear( bool aisWhite ) {
	isWhite = aisWhite;
	Tree.clear();
	V = newNode ( 0 , -1 );
	
}

string MonteCarloTreeSearchAgent::getName() {
	string tmp = "MCTS Agent [ SE:" ;
	tmp += std::to_string(SelectionCount);
	tmp += " EX:";
	tmp += std::to_string(ExpansionCount);
	tmp += "]";
	return ( tmp );
}




