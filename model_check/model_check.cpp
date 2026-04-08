
#include "StdAfx.h"
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <memory>

using namespace std;

// enum type for types of formulas
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

// the class for a formula
class Formula {
public:
	Symbol mainSymbol; 
	Formula *restP; // operand
	Formula *forBinOpSecOpP; // second operand for binary operator
	string atomFormula; // string if it is an atomic formula
};

// how to compare formulas in set
class CmpFormulas {
	public:
	CmpFormulas() {}
	bool operator()( const Formula *formula1P, const Formula *formula2P ) const {
		if ( formula1P->atomFormula != "" && formula2P->atomFormula != "" ) {
			if ( strcmp( formula1P->atomFormula.c_str(), formula2P->atomFormula.c_str() ) < 0 ) {
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

// the class for state
class State {
public:
	State( string name ) {  this->name = name; 
							stateId = globalStateId++;
	}
	int stateId;
	string name;
	set<State*> nextStatesSet;
	set<Formula*, CmpFormulas> validFormulaSet;
};

// how to compare a state in a set
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

// the class for STS
class Sts {
public:
	State *startStateP;
	set<State*, CmpName> allStatesSet;


};

char* mystrdup( const char *str )
{
	int len = strlen( str );
	char *mystr = (char *)malloc( (len+1)*sizeof( char ) ); 
	for ( int i=0; i<=len; i++ ) {
		mystr[i] = str[i];
	}
	return mystr;
}

set<State*> *modelCheck( Formula *formulaP, Sts *stsP );

set<State*> *SAT_EX( Formula *formulaP, Sts *stsP )
{
	set<State*> *setOfStatesP = new set<State*>;				

	set<State*> *tmpSetP = modelCheck( formulaP, stsP );
	set<State*, CmpName>::iterator iter;
	for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
		State *stateP = *iter;
		set<State*>::iterator secondIter;
		for ( secondIter = stateP->nextStatesSet.begin(); secondIter != stateP->nextStatesSet.end(); secondIter++ ) {
			if ( tmpSetP->find( *secondIter ) != tmpSetP->end() ) {
				if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
					setOfStatesP->insert( stateP );
				}
			}
		}
	}

	free( tmpSetP );
	return setOfStatesP;
}

set<State*> *SAT_AF( Formula *formulaP, Sts *stsP )
{
	set<State*> *setOfStatesP = modelCheck( formulaP, stsP );

	bool flag = true;
	while ( flag ) {
		flag = false;
		set<State*, CmpName>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *state1P = *iter;
			bool toBeAdded = true;
			set<State*>::iterator secondIter;
			for ( secondIter = state1P->nextStatesSet.begin(); secondIter != state1P->nextStatesSet.end(); secondIter++ ) {
				State *state2P = *secondIter;
				if ( setOfStatesP->find( state2P ) == setOfStatesP->end() ) {
					toBeAdded = false;
					break;
				}
			}
			if ( toBeAdded ) {
				if ( setOfStatesP->find( state1P ) == setOfStatesP->end() ) {
					setOfStatesP->insert( state1P );
					flag = true;
				}
			}
		}
	}

	return setOfStatesP;
}



set<State*> *SAT_EU( Formula *formulaP, Sts *stsP )
{
	set<State*> *tmp1SetP = modelCheck( formulaP->restP, stsP );
	set<State*> *tmp2SetP = modelCheck( formulaP->forBinOpSecOpP, stsP );	

	bool flag = true;
	while ( flag ) {
		flag = false;
		set<State*, CmpName>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *state1P = *iter;
			set<State*>::iterator secondIter;
			for ( secondIter = state1P->nextStatesSet.begin(); secondIter != state1P->nextStatesSet.end(); secondIter++ ) {
				State *state2P = *secondIter;
				if ( tmp2SetP->find( state2P ) != tmp2SetP->end() ) {
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

set<State*> *modelCheck( Formula *formulaP, Sts *stsP )
{
	set<State*> *setOfStatesP = new set<State*>;

	if ( formulaP->mainSymbol.type == ATOM ) {
		set<State*, CmpName>::iterator iter;
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *stateP = *iter;
			if ( stateP->validFormulaSet.find( formulaP ) != stateP->validFormulaSet.end() ) {
				if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
					setOfStatesP->insert( stateP );
				}
			}
		}
		return setOfStatesP;
	}

	set<State*> *tmpSetP;
	set<State*> *secondTmpSetP;
	set<State*, CmpName>::iterator iter;
	set<State*>::iterator secondIter;
	set<State*>::iterator thirdIter;
	switch ( formulaP->mainSymbol.type ) {
	case NOT:
		tmpSetP = modelCheck( formulaP->restP, stsP );
		for ( iter = stsP->allStatesSet.begin(); iter != stsP->allStatesSet.end(); iter++ ) {
			State *stateP = *iter;
			if ( tmpSetP->find( stateP ) == tmpSetP->end() ) {
				if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
					setOfStatesP->insert( stateP );
				}
			}
		}
		break;
	case OR:
		tmpSetP = modelCheck( formulaP->restP, stsP );
		secondTmpSetP = modelCheck( formulaP->forBinOpSecOpP, stsP );
		for ( thirdIter = tmpSetP->begin(); thirdIter != tmpSetP->end(); thirdIter++ ) {
			State *stateP = *thirdIter;
			if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
		for ( secondIter = secondTmpSetP->begin(); secondIter != secondTmpSetP->end(); secondIter++ ) {
			State* stateP = *secondIter;
			if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
				setOfStatesP->insert( stateP );
			}
		}
		break;
	case AND:
		tmpSetP = modelCheck( formulaP->restP, stsP );
		secondTmpSetP = modelCheck( formulaP->forBinOpSecOpP, stsP );
		for ( secondIter = secondTmpSetP->begin(); secondIter != secondTmpSetP->end(); secondIter++ ) {
			State* stateP = *secondIter;
			if ( tmpSetP->find( stateP ) != tmpSetP->end() ) {
				if ( setOfStatesP->find( stateP ) == setOfStatesP->end() ) {
					setOfStatesP->insert( stateP );
				}
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
		setOfStatesP = SAT_EU( formulaP, stsP );
		break;
	default:
		cout << "Undefined symbol: " << formulaP->mainSymbol.type << endl;
		return NULL;
	}

	return setOfStatesP;
}


Formula* constructFormula( char *strVar )
{
	assert( strVar );
	char *str = mystrdup( strVar );
	Formula *formulaP = new Formula();
	bool atomFormula = true;

	int len = strlen( str );
	int i;
	char c;
	for ( i=0; i < len; i++ ) {
		c = str[i];
		if ( c == 'A' || c == 'E' || c == '!' || c == '.' || c == '+' || c == '!' ) {
			atomFormula = false;
			break;
		}
	}
	if ( atomFormula ) {
		formulaP->atomFormula = mystrdup( str );
		formulaP->forBinOpSecOpP = NULL;
		formulaP->restP = NULL;
		formulaP->mainSymbol.type = ATOM;
		return formulaP;
	}

	char c1 = str[0];
	char c2 = str[1];
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
		char *first = strtok( str+2, "U" );
		char *second = strtok( NULL, "U" );
		formulaP->restP = constructFormula( first );
		formulaP->forBinOpSecOpP = constructFormula( second );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = EU;
		return formulaP;
	}
	if ( c1 == '!' && ( c2 == 'A' || c2 == 'E' ) ) {
		formulaP->restP = constructFormula( str+1 );
		formulaP->forBinOpSecOpP = NULL;
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = NOT;
		return formulaP;
	}

	bool dot = false;
	bool plus = false;
	bool hasNot = false;
	for ( i=0; i<len; i++ ) {
		c = str[i];
		if ( c == '.' ) {
			dot = true;
		}
		if ( c == '+' ) {
			plus = true;
		}
		if ( c == '!' ) {
			hasNot = true;
		}
	}

	if ( plus ) { 
		char *first = strtok( str, "+" );
		char *second = strtok( NULL, "+" );
		formulaP->restP = constructFormula( first );
		formulaP->forBinOpSecOpP = constructFormula( second );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = OR;
		return formulaP;
	}

	if ( dot ) {
		char *first = strtok( str, "." );
		char *second = strtok( NULL, "." );
		formulaP->restP = constructFormula( first );
		formulaP->forBinOpSecOpP = constructFormula( second ); 
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = AND;
		return formulaP;
    }

	if ( hasNot ) {
		formulaP->restP = constructFormula( str+1 );
		formulaP->atomFormula = "";
		formulaP->mainSymbol.type = NOT;
		formulaP->forBinOpSecOpP = NULL;
		return formulaP;
	}

	return formulaP;
}

// a simple STS for testing
Sts* simpleSts()
{
	Sts* stsP = new Sts();

	stsP->startStateP = new State( string("0") );
	State* state2P = new State( string("1") );
	Formula *formulaP = new Formula();
	formulaP->atomFormula = "a";
	formulaP->restP = NULL;
	formulaP->forBinOpSecOpP = NULL;
	formulaP->mainSymbol.type = ATOM;
	state2P->validFormulaSet.insert( formulaP );
	stsP->startStateP->nextStatesSet.insert( state2P );

	stsP->allStatesSet.insert( stsP->startStateP );
	stsP->allStatesSet.insert( state2P );
	
	return stsP;
}

void createAState( char *stateInfo, Sts *stsP )
{
	bool flag;
	char *name = strtok( stateInfo, ":" );
	char *rest = strtok( NULL, ":" );
	State *stateP = new State( name );
	set<State*, CmpName>::iterator iter;
	if ( stsP->allStatesSet.empty() ) {
		stsP->startStateP = stateP;
	} else {
		if ( (iter = stsP->allStatesSet.find( stateP )) != stsP->allStatesSet.end() ) {
			delete stateP;
			stateP = *iter;
		}
	}
	stsP->allStatesSet.insert( stateP );

	char *nextStates = strtok( rest, ";" );
	char *validFormulas = strtok( NULL, ";" );
	flag = true;
	char *next = strtok( nextStates, "," );
	if ( next ) {
		State *nP = new State( next );
		if ( (iter = stsP->allStatesSet.find( nP )) != stsP->allStatesSet.end() ) {
			delete nP;
			nP = *iter;
		}
		stsP->allStatesSet.insert( nP );
		stateP->nextStatesSet.insert( nP );
		while ( flag ) {
			next = strtok( NULL, "," );
			if ( !next ) {
				flag = false;
			} else {
				State *nP = new State( next );
				if ( (iter = stsP->allStatesSet.find( nP )) != stsP->allStatesSet.end() ) {
					delete nP;
					nP = *iter;
				}
				stsP->allStatesSet.insert( nP );
				stateP->nextStatesSet.insert( nP );
			}	
		}
	}

	next = strtok( validFormulas, "," );
	if ( next ) {
		Formula *fP = new Formula();
		fP->mainSymbol.type = ATOM;
		fP->atomFormula = next;
		fP->restP = NULL;
		fP->forBinOpSecOpP = NULL;
		stateP->validFormulaSet.insert( fP );
		flag = true;
		while ( flag ) {
			next = strtok( NULL, "," );
			if ( !next ) {
				flag = false;
			} else {
				Formula *fP = new Formula();
				fP->mainSymbol.type = ATOM;
				fP->atomFormula = next;
				fP->restP = NULL;
				fP->forBinOpSecOpP = NULL;
				stateP->validFormulaSet.insert( fP );
			}
		}
	}

}

Sts* constructSts( const char *stsFileName )
{
	Sts *stsP = new Sts();
	long lSize;
	char * buffer;
	size_t result;

	FILE *pFile = fopen( stsFileName, "r" );

	if (pFile==NULL) {fputs ("File error\n",stderr); exit (1);}

	// obtain file size:
	fseek( pFile , 0 , SEEK_END );
	lSize = ftell( pFile );
	rewind( pFile );

	// allocate memory to contain the whole file:
	buffer = (char*)malloc( sizeof(char)*lSize );
	if ( buffer == NULL ) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread( buffer, 1, lSize, pFile );

	int count = 0;
	for ( int i=0; i < lSize; i++ ) {
		if ( buffer[i] == '\n' ) {
			count++;
		} else if ( buffer[i] == 'e' ) {
			buffer[i] = 0;
			break;
		}
	}

	char *tmpBuf = buffer;

	bool flag = true;

	char **next;
	next = (char **)malloc( (count+1)*sizeof(char *) );
	next[0]= strtok( buffer, "\n" );
	if ( next[0] ) {
		for ( int i=1; i < count; i++ ) {
			next[i] = strtok( NULL, "\n" );
		}
	}
	for ( int j=0; j < count; j++ ) {
		createAState( next[j], stsP );
	}
	 // terminate
	fclose (pFile);
	free (tmpBuf);
	return stsP;
}

Sts* parseSts( int argc, const char *stsFileName )
{
	Sts *stsP;
	if ( (argc < 3) ) {
		cout << "No STS filename specified!" << endl;
		cout << "Using file: './data/STS.txt'" << endl;
		stsFileName = "./data/STS.txt";
	}

	cout << "Constructing STS...." << endl;
#if 1
	stsP = constructSts( stsFileName );
	cout << "Construction of STS from file '" << stsFileName << "' is complete!" << endl;
	return ( stsP );

#else
	return ( simpleSts() ); 
#endif

}

Formula* parseFormula( int argc, const char *argv )
{
	Formula *fP;
	if ( (argc < 2) || !argv ) {
		cout << "No Formula to parse!" << endl;
		argv = "!EU!c2Uc0.c1";
#if 0
		argv = "AF!c0.AFc1";
#endif
		cout << "Using formula: '" << argv << "' for demonstration purpose." << endl;
	}

	cout << "Constructing the formula...." << endl;
	fP = constructFormula( mystrdup(argv) );
	cout << "Construction of formula for '" << argv << "' is complete!" << endl;
	return( fP ); 
}


int main( int argc, char **argv )
{
	Formula* formulaP = parseFormula( argc, argv[1] );
	Sts* stsP = parseSts( argc, argv[2] );

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