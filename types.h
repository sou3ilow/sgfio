
#ifndef TYPES_H
#define TYPES_H

/// Position, Move
typedef short position_t;
//typedef int position_t;

/// Anchor for string
typedef position_t anchor_t;
//typedef short anchor_t; // for debug


/// Color
enum Color
{
	EMPTY, BLACK, WHITE, OB, COLOR_NUM = 3
};

struct BlackWhite
{
	int bw;
public:
	BlackWhite() : bw(BLACK) {}
	Color operator* () const { return static_cast<Color>( bw ); }
	operator bool() const { return bw == BLACK || bw == WHITE; }
	void next() { bw++; }
};

/// Rule
enum Rule
{
	Chinese, Japanese
};

std::istream& operator >> (std::istream& is, Rule& rule)
{
	std::string str;
	is >> str;
	if ( false ) ;
	else if ( str == "Chinese" ) rule = Chinese;
	else if ( str == "Japanese" ) rule = Japanese;
	else rule = Chinese;
	return is;	
}

#endif//TYPES_H
