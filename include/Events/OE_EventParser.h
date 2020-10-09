#ifndef OE_EVENTPARSER_H
#define OE_EVENTPARSER_H


#include <types/OE_TypesBase.h>


/// TODO: INCOMPLETE (READ: NON FUNCTIONAL)


enum OE_EVENT_PAIR_TYPE{
	OE_NO_COMBO = 0,
	OE_AND_COMBO = 1,
	OE_OR_COMBO = 2
};

/// class intended to store a pair (or no pair) of events
/// equivalent to Node* for the parser
class OE_EventPair{
public:
	OE_EventPair();
	~OE_EventPair();

protected:
	OE_EVENT_PAIR_TYPE pair_type;
	OE_EventPair* event0;
	OE_EventPair* event1;
	std::string name;
};



#endif
