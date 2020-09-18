#include "FE_Libs.h"

enum FE_EVENT_PAIR_TYPE{
	FE_NO_COMBO = 0,
	FE_AND_COMBO = 1,
	FE_OR_COMBO = 2
};

/// class intended to store a pair (or no pair) of events
/// equivalent to Node* for the parser
class FE_EventPair{
public:
	FE_EventPair();
	~FE_EventPair();

protected:
	FE_EVENT_PAIR_TYPE pair_type;
	FE_EventPair* event0;
	FE_EventPair* event1;
	string name;
};



//asdf
