#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iomanip>
#include "Const.h"

struct DiskBoard : public Const
{
	Color turn;
	Color* disk; // array
	int captures[COLOR_NUM];

	double komi;
	int ply;
	position_t last_move;
	position_t second_last;

public:
	DiskBoard(int size)
		: Const(size)
		, komi(6.5) // HACK
	{
		disk = new Color[ArraySize];
		init();
	}

	~DiskBoard()
	{
		delete[] disk;
	}

public:
	void init()
	{
		turn = BLACK;

		for ( int i=0; i<COLOR_NUM; i++ )
			captures[i] = 0;

		for ( int y=0; y<ExSize; y++ )
		for ( int x=0; x<ExSize; x++ )
			disk[to_pos(x, y)] = is_onboard(x, y)
				? EMPTY
				: OB;
		
		last_move = NullMove;
		second_last = NullMove;
		ply = 0;
	}

public:
	bool is_empty(position_t pos) const
	{
		assert ( is_onexboard(pos) );
		return disk[pos] == EMPTY;
	}

	bool is_disk(position_t pos, Color d) const
	{
		assert ( is_onexboard(pos) );
		return disk[pos] == d;
	}

public:
	Color disk_on(position_t pos) const
	{
		assert ( is_onexboard(pos) );
		return disk[pos];
	}

	void replace(Color d, position_t pos)
	{
		assert ( is_onboard(pos) );
		disk[pos] = d;
	}

public:
	void show(std::ostream& os=std::cout) const
	{
		#define FILE_HEAD() do {          \
			os << "  ";                   \
			for ( int x=1; x<=Size; x++ ) \
				os << ' ' << file(x);     \
			os << '\n'; } while(0)

		FILE_HEAD();
		for ( int y=1; y<=Size; y++ )
		{
			os << std::setw(2) << rank(y);
			
			for ( int x=1; x<=Size; x++ )
				os << ' ' << mark(disk[to_pos(x, y)]);
			os << std::setw(2) << rank(y);
			
			if (false) ;
			else if ( y == 2        ) os << "  komi : " << komi << "\n";
				//:
			else if ( y == Size - 5 ) os << "  black: " << captures[BLACK] << "\n";
			else if ( y == Size - 4 ) os << "  white: " << captures[WHITE] << "\n";
			else if ( y == Size - 2 ) os << "  ply  : " << ply << "\n";
			else if ( y == Size - 1 )   os << "  last : " << pos2gtp(last_move) << "\n";
			else if ( y == Size - 0 ) os << "  trun : " << player(turn) << "\n";
			else                      os << "\n";
		}
		FILE_HEAD();
		#undef FILEHEAD
	}
};

#endif//BOARD_H
