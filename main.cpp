#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <assert.h>

#include "Pylos.h"
#include "Agent.h"
#include "RandomAgent.h"
#include "MonteCarloAgent.h"



using namespace std;

void Play ( int GameCnt , Agent *P1 , Agent *P2 ) {
	printf ( "%s vs %s \n", P1->getName().c_str(), P2->getName().c_str() );
	
	//P1 - White ; P2 - Black
	
	srand(clock());
	
	
	int WhiteWin = 0;
	int BlackWin = 0;
	int Draw = 0;
	
	long long WhiteTime = 0;
	long long BlackTime = 0;
	
	for ( int i = 0 ; i < GameCnt ; i++ ) {
		long long state = 0 ;
		int t = 0 ; 
		
		P1->Clear(true);
		P2->Clear(false);
		
		while ( true ) {
			
			pylos::Info info = pylos::getInfo ( state );
			if ( info.gameState != pylos::IN_PROGRESS ) {
				if ( info.gameState == pylos::WHITE_WIN ) 
					WhiteWin++;
				else
					BlackWin++;
				
				break;
			}
			
			if ( t > 10000 ) {
				Draw++;
				break;
			}
			
			int ST = clock();
			
			if ( t % 2 == 0 )
				state = P1->getMove(state);
			else
				state = P2->getMove(state);
			
			if ( t % 2 == 0 )
				WhiteTime += (clock()-ST);
			else
				BlackTime += (clock()-ST);
							
			t++;
			
		}
	}
	
	
	
	printf ( "White Wins : %i  ( %s ) \n", WhiteWin, P1->getName().c_str() ); 
	printf ( "Black Wins : %i  ( %s ) \n", BlackWin, P2->getName().c_str() ); 
	printf ( "Draw       : %i \n" , Draw );
	
	printf ( "White time : %f s \n" , (WhiteTime)/(double)CLOCKS_PER_SEC );
	printf ( "Black time : %f s \n" , (BlackTime)/(double)CLOCKS_PER_SEC );
	printf ( "\n" );
	
	
	srand(clock());
	
	WhiteWin = 0;
	BlackWin = 0;
    Draw = 0;
    
    
	WhiteTime = 0;
	BlackTime = 0;
	
	for ( int i = 0 ; i < GameCnt ; i++ ) {
		long long state = 0 ;
		int t = 0 ; 
		
		P1->Clear(false);
		P2->Clear(true);
		
		while ( true ) {
			
			pylos::Info info = pylos::getInfo ( state );
			if ( info.gameState != pylos::IN_PROGRESS ) {
				if ( info.gameState == pylos::WHITE_WIN ) 
					WhiteWin++;
				else
					BlackWin++;
				
				break;
			}
			
			if ( t > 10000 ) {
				Draw++;
				break;
			}
			
			int ST = clock();
			
			if ( t % 2 == 0 )
				state = P2->getMove(state);
			else
				state = P1->getMove(state);
			
			if ( t % 2 == 0 )
				WhiteTime += (clock()-ST);
			else
				BlackTime += (clock()-ST);
			
			t++;
			
		}
	}
	
	printf ( "White Wins : %i  ( %s ) \n", WhiteWin, P2->getName().c_str() ); 
	printf ( "Black Wins : %i  ( %s ) \n", BlackWin, P1->getName().c_str() ); 
	printf ( "Draw       : %i \n" , Draw );
	
	printf ( "White time : %f s \n" , (WhiteTime)/(double)CLOCKS_PER_SEC );
	printf ( "Black time : %f s \n" , (BlackTime)/(double)CLOCKS_PER_SEC );
	
	printf ( "-------------------------------------- \n" );
	
	
	
	
	
	
	
}


int main(){
	
	
	MonteCarloTreeSearchAgent *MC1 = new MonteCarloTreeSearchAgent();
	MonteCarloTreeSearchAgent *MC2 = new MonteCarloTreeSearchAgent();
	
	MC2->SetConstants ( 32 , 32 , 1 );
	
	RandomAgent *R1 = new RandomAgent();
	RandomAgent *R2 = new RandomAgent();
	
	const int delta = 500;
	
	Play ( delta , R1 , R2 );
	Play ( delta  , MC1 , R1 );
	Play ( delta  , MC2 , R1 );
	Play ( delta , MC1 , MC2 );
	
	return 0;
}
