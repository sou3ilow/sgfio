#ifndef CONST_H
#define CONST_H

#include <stdexcept>
#include <cassert>
#include <cstring>
#include <algorithm>

#include "util.h"
#include "types.h"

// all members are const

class Const
{
public:
	static const position_t NullMove = 0;
	static const position_t Pass = 1;
	static const position_t Resign = 2;

public:
	const int Size;
	const int ExSize;
	const int ArraySize;
	const int North, South;
	static const int East = +1, West = -1;
	const int NorthEast, NorthWest, SouthEast, SouthWest;

public:
	Const(int size)
		: Size(size)
		, ExSize(size + 2)
		, ArraySize(ExSize * ExSize)
		, North(-ExSize), South(+ExSize)
		, NorthEast(North+East), NorthWest(North+West)
		, SouthEast(South+East), SouthWest(South+West)
	{
	}

public:
	int to_pos(int x, int y) const
	{
		return y * ExSize + x;
	}

	void to_xy(int pos, int* x, int* y) const
	{
		*x = pos % ExSize;
		*y = pos / ExSize;
	}

public:
	bool is_onexboard(position_t pos) const
	{
		return 0 <= pos && pos < ArraySize;
	}

	bool is_onboard(int x, int y) const
	{
		return 1 <= x && x <= Size
		    && 1 <= y && y <= Size;
	}

	bool is_onboard(int pos) const
	{
		int x, y;
		to_xy(pos, &x, &y);
		return is_onboard(x, y);
	}

public:
	int			N (int pos) const { return pos + North; }
	static int	E (int pos)       { return pos + East; }
	static int	W (int pos)	      { return pos + West; }
	int			S (int pos) const { return pos + South; }
	int			NE(int pos)	const { return pos + NorthEast; }
	int			NW(int pos)	const { return pos + NorthWest; }
	int			SE(int pos)	const { return pos + SouthEast; }
	int			SW(int pos)	const { return pos + SouthWest; }

public:
	// iterator for neigbours
	class News
	{
		friend class Const;

		static const int NEIGHBOURS = 4; // NEWS

		int pos[NEIGHBOURS];
		int index;

	public:
		int operator* () const { return pos[index]; }
		operator bool() const  { return index < NEIGHBOURS; }
		void next() { index++; }
	};

	News news(int pos) const
	{
		News ret;
		ret.pos[0] = N(pos);
		ret.pos[1] = E(pos);
		ret.pos[2] = W(pos);
		ret.pos[3] = S(pos);
		ret.index = 0;
		return ret;
	}

public:
	static Color opponent(Color turn)
	{
		assert ( turn == BLACK || turn == WHITE );
		assert ( turn == 1 || turn == 2 );
		return Color( turn ^ 3 );
	}

	static char mark(Color d) { return ".XO?"[d]; }

	static std::string player(Color turn)
	{
		switch (turn) {
		case EMPTY: return "empty";
		case BLACK: return "black";
		case WHITE: return "white";
		case OB:    return "ob";
		default:    return "who??";
		}
	}

public:
	std::string rank(int y) const { return tstream(Size - y + 1); }

	static char file(int x) { return " ABCDEFGHJKLMNOPQRSTUVWXYZ"[x]; }

public:
	std::string pos2gtp(position_t pos) const
	{
		if ( false ) ;
		else if ( pos == NullMove ) return "null";
		else if ( pos == Pass )     return "Pass";
		else if ( pos == Resign )   return "Resign";

		int x, y;
		to_xy(pos, &x, &y);
		return tstream()
			<< " ABCDEFGHJKLMNOPQRSTUVWXYZ"[x]
			<< Size - y + 1;
	}

	position_t gtp2pos(const std::string& gtp_move) const
	{
		for ( int i=0; i<4; i++ )
			if ( toupper(gtp_move[i]) != "PASS"[i]  )	break;
			else if ( i == 3 )							return Pass;

		for ( int i=0; i<6; i++ )
			if ( toupper(gtp_move[i]) != "RESIGN"[i]   )break;
			else if ( i == 5 )							return Resign;

		char f; int r;
		std::stringstream ss(gtp_move);
		ss >> f >> r;

		const std::string file_str(" ABCDEFGHJKLMNOPQRSTUVWXYZ"); // no I
		int x = file_str.find(toupper(f));
		int y = Size - r + 1;

		return is_onboard(x, y)
			?	to_pos(x, y)
			:	NullMove;
	}

public:
	std::string pos2sgf(position_t pos) const
	{
		if ( false ) ;
		else if ( pos == NullMove ) return "null";
		else if ( pos == Pass )     return "";
		else if ( pos == Resign )   return "tt";

		int x, y;
		to_xy(pos, &x, &y);

		return tstream()
			<< " abcdefghijklmnopqrstuvwxyz"[x]
			<< " abcdefghijklmnopqrstuvwxyz"[y];
	}

	position_t sgf2pos(const std::string& sgf_move, bool fail_on_exception=false) const
	{
		if ( sgf_move.empty() )
			return Pass;

		for ( int i=0; i<2; i++ )
			if ( tolower(sgf_move[i]) != "tt"[i]   )	break;
			else if ( i == 1 )							return Pass;

		char f, r;
		std::stringstream ss(sgf_move);
		ss >> f >> r;

		const std::string str(" abcdefghijklmnopqrstuvwxyz"); // has i
		int x = str.find(tolower(f));
		int y = str.find(tolower(r));

		if ( is_onboard(x, y) )
			return to_pos(x, y);
		
		if ( fail_on_exception )
			throw std::runtime_error("ilegalmove");
		else
			return NullMove;
	}
};

#endif//CONST_H
