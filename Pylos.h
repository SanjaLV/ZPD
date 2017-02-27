#pragma once
#include <vector>
#include <stdio.h>


/* State = 
*
* 0-29 - bit is this ball white
* 30-59 - bit is this ball black
* 60 - turn 0 - white ; 1 = black
* X is empty if ( X && !(X+30) )
*/


typedef std::pair<long long , int> pp;

namespace pylos {
	
	const int BLACK_WIN = 2;
	const int WHITE_WIN = 1;
	const int IN_PROGRESS = 0;
	   
	
	struct Info {
		int black;
		int white;
		int gameState;
	};
	
	Info getInfo ( long long State ) {
		Info res;
		
		res.black = 15 ; res.white = 15 ; res.gameState = IN_PROGRESS;
		
		for ( int i = 0 ; i < 30 ; i++ )
			if ( State & ( 1LL << i ) )
				res.white--;
		
		for ( int i = 30 ; i < 60 ; i++ )
			if ( State & ( 1LL << i ) )
				res.black--;
		
		if ( res.black == 0 )
			res.gameState = WHITE_WIN;
		if ( res.white == 0 )
			res.gameState = BLACK_WIN;
			
		return res;		
	}
	
	void DebugView ( long long State ) {
		int field[30];
		for ( int i = 0 ; i < 30 ; i++ ) 
			field[i] = 0;
			
		// unpacking
		
		for( int i = 0 ; i < 30 ; i++ )
			if ( State & ( 1LL << i ) )
				field[i] = 1;
				
		for( int i = 30 ; i < 59 ; i++ )
			if ( State & ( 1LL << i ) )
				field[i-30] = 2;
		
		for ( int i = 0 ; i < 30 ; i++ )
			printf ( "%i " , field[i] );
		printf ( "\n" );
	}
	
	long long Place( long long StartState , int x , bool isBlack ) {
		long long tmp = StartState;
		
		if ( isBlack ) 
			tmp += ( 1LL << ( x + 30 ) );
		else
			tmp += ( 1LL << ( x ) );
		
		tmp = tmp ^ ( 1LL << 60 );
		
		
		return tmp;
	}
	
	int f ( int x ) {
		if ( x <= 18 )
			return 0;
		if ( x <= 21 )
			return 1;
		if ( x <= 24 )
			return 2;
		if ( x <= 26 )
			return 0;
		if ( x <= 28 )
			return 1;
	}
	
	bool Exist ( long long s , int x ){
		return ( (s &( 1LL << x ) ) || (s & ( 1LL << ( x + 30 ) ) ) );
	}	
	
	bool LookUp ( long long s , int x ) {// return True if we can grab x
		
		//1 st floor
		if ( x < 16 ) {
			
			if ( x == 12 || x == 13 || x == 8 || x == 9 )
				if ( Exist(s , 22 ) )
					return false;
			if ( x == 13 || x == 14 || x == 9 || x == 10 )
				if ( Exist(s , 23 ) )
					return false;
			if ( x == 14 || x == 15 || x == 10 || x == 11 )
				if ( Exist(s , 24 ) )
					return false;
			
			if ( x == 4 || x == 5 || x == 8 || x == 9 )
				if ( Exist(s , 19 ) )
					return false;
			if ( x == 5 || x == 6 || x == 9 || x == 10 )
				if ( Exist(s , 20 ) )
					return false;
			if ( x == 6 || x == 7 || x == 10 || x == 11 )
				if ( Exist(s , 21 ) )
					return false;
			
			if ( x == 0 || x == 1 || x == 4 || x == 5 )
				if ( Exist(s , 16 ) )
					return false;
			if ( x == 1 || x == 2 || x == 5 || x == 6 )
				if ( Exist(s , 17 ) )
					return false;
			if ( x == 2 || x == 3 || x == 6 || x == 7 )
				if ( Exist(s , 18 ) )
					return false;
			
			 
		}
		else if ( x < 25 ) {
			
			if ( x == 19 || x == 20 || x == 22 || x == 23 )
				if ( Exist(s , 27 ) )
					return false;
			if ( x == 20 || x == 21 || x == 23 || x == 24 )
				if ( Exist(s , 28 ) )
					return false;
		    
			if ( x == 16 || x == 17 || x == 19 || x == 20 )
				if ( Exist(s , 25 ) )
					return false;
			if ( x == 17 || x == 18 || x == 20 || x == 21 )
				if ( Exist(s , 26 ) )
					return false;
		    
		}
		
		
		return true;
	}
	
	bool LookDown ( long long s , int x ){ // return True is we can place @ X
		
		
		
		if ( x < 16 ) // First Floor
			return true;
		
		bool res = true;
		
		if ( x < 25 ) {// Second Floor 
			
			res = res && Exist ( s , x-16 + f(x) );
			res = res && Exist ( s , x-15 + f(x) );
			res = res && Exist ( s , x-12 + f(x) );
			res = res && Exist ( s , x-11 + f(x) );			
		
		}
		else if ( x < 29 ) {
			res = res && Exist ( s , x-9 + f(x) );
			res = res && Exist ( s , x-8 + f(x) );
			res = res && Exist ( s , x-6 + f(x) );
			res = res && Exist ( s , x-5 + f(x) );
			
		}
		else {
			res = res && Exist ( s , 25 );
			res = res && Exist ( s , 26 );
			res = res && Exist ( s , 27 );
			res = res && Exist ( s , 28 );
		}
		
		return res;
	}
	
	void Remove ( std::vector<long long> &ans , long long s , int x ) { 
		
		bool canRemove = false;
		
		int delta = 30;
		if ( s & ( 1LL << 60 ) )
			delta =0;
			
		
		if ( (s&(1LL<<(27+delta))) && (s&(1LL<<(28+delta))) && (s&(1LL<<(25+delta))) && (s&(1LL<<(26+delta))) && ( x <= 28 ) && ( x >= 25 ) )
			canRemove = true;
		
		//2-floor
		for(int i = 16 ; i < 21 ; i++ ) 
			if ( (s&(1LL<<(i+delta))) && ( s&(1LL<<(i+1+delta))) && ( s&(1LL<<(i+3+delta) ) ) && ( s&(1LL<<(i+3+delta + 1) ) )
				 && ( ( i == x ) || ( i+1 == x ) || ( i+3 == x ) || ( i+4 == x ) ) )
				canRemove = true;
		
		//1-floor
		for(int i = 0 ; i < 16 ; i++ )
			if ( (s&(1LL<<(i+delta))) && ( s&(1LL<<(i+1+delta))) && ( s&(1LL<<(i+4+delta) ) ) && ( s&(1LL<<(i+4+delta + 1) ) ) 
				&& ( ( i == x ) || ( i+1 == x ) || ( i+4 == x ) || ( i+5 == x ) ) )
				canRemove = true;
		
		
		
		if ( canRemove ) {
			
			
			for ( int i = 0 ; i < 29 ; i++ ) {
				
				if( (s&(1LL << (i+delta) ) ) && LookUp ( s , i ) ) {
					
					
					ans.push_back( s - (1LL << (i+delta) ) );
					for ( int j = i+1 ; j < 29 ; j++ ) {
						if( (s&(1LL << (j+delta) ) ) && LookUp ( s , j ) )
							ans.push_back(s - (1LL << ( i + delta ) ) - ( 1LL << ( j + delta ) ) );
					
					}
					
				}
			}
			
		}
		else {
			ans.push_back(s);
			
		}
	}
	
	
	std::vector<long long> getValidMoves( long long State ) {
		
		
		bool BlackTour = (State & ( 1LL << 60 ));
		
		
		std::vector<pp> res;
		
		res.clear();
		
		
		// try to place new
		for ( int i = 0 ; i < 30 ; i++ ) {
			if ( (State & ( 1LL <<i ) ) == 0  && (State & ( 1LL <<(i+30)) ) == 0 && LookDown(State , i) ){
				res.push_back ( std::make_pair( Place(State,i, BlackTour ) , i ) );
			}
		}
		//try to move up
		for ( int i = 0 ; i < 25 ; i++ ) {
			
			int shift = 0;
			if ( BlackTour )
				shift = 30;
			
			if ( State & ( 1LL << ( i+shift ) ) && LookUp ( State , i ) ) {
				
					
				long long NewState = State - ( 1LL << ( i + shift ) );
				
				
				int start;
				if ( i < 16 )
					start = 16;
				else 
					start = 25;
					
				for ( int j = start ; j <= 28 ; j++ ) {
					
					if ( (State & ( 1LL <<j ) ) == 0  && (State & ( 1LL <<(j+30)) ) == 0 && LookDown(NewState,j) ) {
					
						res.push_back ( std::make_pair( Place( NewState,j,BlackTour) , j ) );
					}
				}
					
					
			}
		}/*for*/
		
		
		std::vector<long long> ans;
		ans.clear();
		
		
		for ( int i = 0 ; i < res.size() ; i++ ) {
			Remove( ans , res[i].first , res[i].second );
		}
		
		return ans;
		
	}
	
	
	
	
};

