
#include "StdAfx.h"
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <assert.h>

using namespace std;

typedef enum {
	UNDEFINED = -1,
	ATOM,
	NOT,
	OR,
	AND,
	AF,
	EX,
	EU
} Type;

class Symbol {
public:
	Type type;
};

class Formula {
public:
	Symbol mainSymbol;
	Formula *restP; 
	Formula *forBinOpSecOpP;
	string atomFormula;
};

class CmpFormulas {
	public:
	CmpFormulas() {}
	bool operator()( const Formula *formula1P, const Formula *formula2P ) const {
		if ( formula1P->atomFormula == "" && formula2P->atomFormula == "" ) {
			if ( formula1P->atomFormula < formula2P->atomFormula ) {
				return true;
			} else {
				return false;
			}
		} else {
			return ( formula1P->mainSymbol.type < formula2P->mainSymbol.type );
		}
	}
};


int globalStateId;
class State {
public:
	State( string name ) {  this->name = name; 
							stateId = globalStateId++;
	}
	int stateId;
	set<State*> nextStatesSet;
	set<Formula*, CmpFormulas> validFormulaSet;
};

class CmpName {
public:
	bool operator()( const State *state1P, const State *state2P ) const {
		if ( state1P->name < state2P->name ) {
			return true;
		} else {
			return false;
		}
	}
};


class Sts {
public:
	State *startStateP;
	set<State*, CmpName> allStatesSet;


};

set<State*> modelCheck( Formula *formulaP, Sts stsP );

set<State*> SAT_EX( Formula *formulaP, Sts *stsP )
{
	set<State*> *setOfStatesP = new set<State*>;

	set<State*> *tmpSetP = modelCheck( formulaP, stsP );
	
	for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
		State *stateP = *iter;
		set<State*>::iterator secondIter;
		for ( secondIter = stateP->nextStatesSet.begin(); secondIter = stateP->nextStatesSet.end(); secondIter++ ) {
			if ( tmpSetP->find( *secondIter ) != tmpSetP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
	}

	delete tmpSetP;
	return setOfStatesP;
}

set<State*> SAT_AF( Formula *formulaP, Sts *stsP )
{
	set<State*> *setOfStatesP = modelCheck( formulaP, stsP );

	bool flag = true;
	while ( flag ) {
		flag = false;
		set<State*>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *state1P = *iter;
			bool toBeAdded = true;
			set<State*>::iterator secondIter;
			for ( secondIter = state1P->nextStatesSet.begin(); secondIter = state1P->nextStatesSet.end(); secondIter++ ) {
				State *state2P = *secondIter;
				if ( setOfStatesP->find( state2P ) == setOfStatesP->end() ) {
					toBeAdded = false;
				}
			}
			if ( toBeAdded ) {
				if ( setOfStatesP->find( state1P ) == setOfStates->end() ) {
					setOfStatesP->insert( state1P );
					flag = true;
				}
			}
		}
	}

	return setOfStatesP;
}



set<State*> SAT_EU( Formula *formulaP, Sts *stsP )
{
	set<State*> *tmp1SetP = modelCheck( formulaP->restP, stsP );
	set<State*> *tmp2SetP = modelCheck( formulaP->forBinOpSecOpP, stsP );	

	bool flag = true;
	while ( flag ) {
		flag = false;
		set<State*>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *state1P = *iter;
			for ( secondIter = state1P->nextStatesSet.begin(); secondIter = state1P->nextStatesSet.end(); secondIter++ ) {
				State *state2P = *secondIter;
				if ( tmp2SetP->find( state2P ) != tmp2SetP->end() {
					if ( tmp1SetP->find( state1P ) != tmp1SetP->end() ) {
						if ( tmp2SetP->find( state1P ) == tmp2SetP->end() ) {
							tmp2SetP->insert( state1P );
							flag = true;
						}
					}
				}
			}
		}
	}

	return tmp2SetP;
}

set<State*> modelCheck( Formula *formulaP, Sts stsP )
{
	set<State*> setOfStatesP = new set<State*>;

	if ( formulaP->mainSymbol.type == ATOM ) {
		set<State*>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *stateP = *iter;
			if ( stateP->validFormulaSet.find( formulaP ) != stateP->validFormulaSet.find( formulaP ) ) {
				setOfStatesP->insert( stateP );
			}
		}
		return setOfStatesP;
	}

	set<State*> *tmpSetP;
	set<State*> *secondTmpSetP;
	set<State*>::iterator iter;
	switch ( formulaP->mainSymbol.type ) {
	case NOT:
		tmpSetP = modelCheck( formulaP, stsP );
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *stateP = *iter;
			if ( tmpSetP->find( stateP ) == tmpSetP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
		break;
	case OR:
		tmpSetP = modelCheck( formulaP->restP, stsP );
		secondTmpSetP = modelCheck( formulaP->forBinOpSecOpP, stsP );
		for ( iter = tmpSetP->begin(); iter != tmpSetP->end(); iter++ ) {
			setOfStatesP->insert( *iter );
		}
		for ( iter = secondTmpSetP->begin(); iter != secondTmpSetP->end(); iter++ ) {
			State* stateP = *iter;
			if ( secondTmpSetP->find( stateP ) == secondTmpSetP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
		break
	case AND:
		for ( iter = secondTmpSetP->begin(); iter != secondTmpSetP->end(); iter++ ) {
			State* stateP = *iter;
			if ( tmpSetP->find( stateP ) != tmpSetP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
		break;
	case AF:
		setOfStatesP = SAT_AF( formulaP->restP, stsP );
		break;
	case EX:
		setOfStatesP = SAT_EX( formulaP->restP, stsP );
		break;
	case EU:
		setOfStatesP-> SAT_EU( formulaP, stsP );
		break;
	default:
		cout << "Undefined symbol: " << formulaP->mainSymbol.type << endl;
	}

	return setOfStatesP;
}


Formula* constructFormula( char *str )
{
	Formula *formulaP = new Formula();
	bool atomFormula = true;
	for ( i=0; i < strlen(str); i++ ) {
		char c = str[i];
		if ( c == 'A' || c == 'E' || c == '!' || c == '.' || c == '+' ) {
			atomFormula = false;
			break;
		}
	}
	if ( atomFormula ) {
		formulaP->atomFormula = strdup( str );
		formulaP->forBinOpSecOpP = NULL;
		formulaP->restP = NULL;
		formulaP->mainSymbol.type = ATOM;
		return formulaP;
	}

	if ( c == '.' ) {
		char *first = strtok( str, "." );
		char *second = strtok( NULL, "." );
		formulaP->restP = constructFormula( first );
		formulaP->forBinOpSecOpP = constructFormula( second );
    }
	if ( c == '+' ) {
		char *first = strtok( str, "+" );
		char *second = strtok( NULL, "+" );
		formulaP->restP = constructFormula( first );
		formulaP->forBinOpSecOpP = constructFormula( second );
    }


	char c1 = str[0];
	char c2 = str[2];
	if ( c1 == 'A' && c2 == 'F' ) {
		formulaP->restP = constructFormula( str+2 );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = AF;
		formulaP->forBinOpSecOpP = NULL;
		return formulaP;
	}
	if ( c1 == 'E' && c2 == 'X' ) {
		formulaP->restP = constructFormula( str+2 );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = EX;
		formulaP->forBinOpSecOpP = NULL;
		return formulaP;
	}
	if ( c1 == 'E' && c2 == 'U' ) {
		formulaP->restP = constructFormula( str+2 );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = EU;
		formulaP->forBinOpSecOpP = NULL;
		return formulaP;
	}

}

Sts* simpleSts()
{
	Sts* stsP = new Sts();

	stsP->startStateP = new State( string("1") );
	State* state2P = new State( string("2") );
	Formula *formulaP = new Formula();
	formulaP->atomFormula = "a";
	formulaP->restP = NULL;
	formulaP->forBinOpSecOpP = NULL;
	formulaP->mainSymbol.type = ATOM;
	state2P->validFormulaSet.insert( formulaP );
	stsP->startStateP->nextStatesSet.insert( state2P );

}

Sts* parseSts( char *stsFileName )
{
	assert( stsFileName );

	cout << "Constructing STS....\n" << endl;
#if 0
	return ( contructSts( stsFileName ) );
#else
	return ( simpleSts() ); 
#endif

}

Formula* parseFormula( char *argv )
{
	assert( argv );

	cout << "Constructing the formula...." << endl;

}


int main( int argc, char **argv )
{
	Formula* formulaP = parseFormula( argv[1] );
	Sts* stsP = parseSts( argv[2] );

	set<State*> *setOfStatesP = modelCheck( formulaP, stsP );

	if ( setOfStatesP->empty() ) {
		cout << "Formula is not valid in any state!" << endl;
	} else {
		cout << "Formula is valid in following states: " << endl;
		set<State*>::iterator iter;
		for ( iter = setOfStatesP->begin(); iter != setOfStatesP->end(); iter++ ) {
			cout << (*iter)->name << ", " << endl;
		}
	}

	return 0;
}