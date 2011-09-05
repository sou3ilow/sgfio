
#ifndef STRING_BOARD_H
#define STRING_BOARD_H

#include <map>
#include "DiskBoard.h"
#include "String.h"

class StringBoard : public DiskBoard
{
public:
	typedef std::map<anchor_t, String> Strings;
	typedef std::set<anchor_t> Anchors;
	typedef std::vector<anchor_t> AnchorBoard;

private:
	static const anchor_t InvalidAnchor = 0;

	Strings strings[COLOR_NUM];
	AnchorBoard anchor_board;
	
public:
	StringBoard(int size)
		:	DiskBoard(size)
		,	anchor_board(ArraySize, InvalidAnchor)
	{
	}

	void init()
	{
		DiskBoard::init();
		for ( BlackWhite bw; bw; bw.next() )
			strings[*bw].clear();

		std::fill(anchor_board.begin(), anchor_board.end(),	InvalidAnchor);
	}

private:
	const String& string_of(Color color, anchor_t anchor) const
	{
		return strings[color].find(anchor)->second;
	}
	
	void set_anchors(anchor_t anchor, const Positions& positions)
	{
		for ( Positions::const_iterator p=positions.begin(); p!=positions.end(); ++p )
			anchor_board[*p] = anchor;
	}

public:
	bool is_notob(int pos)
	{
		return !is_disk(pos, OB);
	}

public:
	struct MoveEffect
	{
		String string;       // including position played
		Anchors friends;     // will united to the above string
		Anchors enemies;     // will decrease their librty or die
		Anchors capturable;  // subset of enemies

		void init()
		{
			string.init();
			friends.clear();
			enemies.clear();
			capturable.clear();
		}
	};

public:
	void make(const std::string& gtp_move)
	{
		make(gtp2pos(gtp_move));
	}

	void make(position_t pos)
	{
		Color Mine = turn;
		Color Oppo = opponent(turn);

		if ( pos == Resign )
			throw "HACK";
		else if ( pos == Pass )
			;
		else // regular move
		{
			MoveEffect e = make_virtual(Mine, pos);
			
			// remove enemy strings or decrease their liberty
			if ( !e.enemies.empty() )
				for ( Anchors::iterator a=e.enemies.begin(); a!=e.enemies.end(); ++a )
					if ( 0 == e.capturable.count(*a) ) // not found
						strings[Oppo][*a].liberty.erase(pos);
					else
					{
						String& s = strings[Oppo][*a];

						// capture
						captures[Mine] += s.positions.size();

						for ( Positions::const_iterator i=s.positions.begin(); i!=s.positions.end(); ++i )
						{
							// change on board
							replace(EMPTY, *i);

							// increase liberty of the neigbour
							for ( News n = news(*i); n; n.next() )
								if ( is_disk(*n, Mine) )
									strings[Mine][anchor_board[*n]].liberty.insert(*i);
						}

						// reset anchors
						set_anchors(InvalidAnchor, s.positions);

						// remove string
						strings[Oppo].erase(*a);
					}

			// remove connected strings ( board is not changed )
			if ( !e.friends.empty() )
				for ( Anchors::iterator a=e.friends.begin(); a!=e.friends.end(); ++a )
					strings[Mine].erase(*a);

			// add new string
			anchor_t new_anchor = e.string.anchor();
			strings[Mine].insert(Strings::value_type(new_anchor, e.string));

			// refresh anchors
			set_anchors(new_anchor, e.string.positions);

			// only new stone
			replace(Mine, pos);
		} // regular move

		turn = Oppo;
		second_last = last_move;
		last_move = pos;
		ply++;
	}

public:
	MoveEffect make_virtual(Color color, int pos) const
	{
		assert ( color == BLACK || color == WHITE );
		
		MoveEffect ret;

		if ( pos == Pass   ) return ret;
		if ( pos == Resign ) return ret;

		assert ( is_onboard(pos) );

		const Color Mine = color;
		const Color Oppo = opponent(color);

		ret.string.positions.insert(pos);

		// sorrounded condition
		for ( News n = news(pos); n; n.next() )
		{
			if ( !is_onboard(*n) )
				;
			else if ( is_empty(*n) )        ret.string.liberty.insert(*n);
			else if ( is_disk(*n, Mine) )   ret.friends.insert(anchor_board[*n]);
			else if ( is_disk(*n, Oppo) ) { ret.enemies.insert(anchor_board[*n]);

				// if the opponent has only one liberty, the place will be liberty of new stone
				if ( string_of(Oppo, anchor_board[*n]).liberty.size() == 1 )
					ret.string.liberty.insert(*n);
			}
		}

		// add disk as string 
		//strings[disk].insert(Strings::value_type(s.anchor(), s));
		
		// connect to friends
		for ( Anchors::iterator a=ret.friends.begin(); a!=ret.friends.end(); ++a)
			ret.string.join(string_of(Mine, *a));

		// erace an liberty
		ret.string.liberty.erase(pos);
		
		// decrease enemy's liberty
		for ( Anchors::iterator a=ret.enemies.begin(); a!=ret.enemies.end(); ++a)
		{
			const String& s = string_of(Oppo, *a);
			if ( 1 < s.liberty.size() )
				;//strings[Oppo][*a].liberty.erase(pos);
			else
			{
				assert ( s.liberty.size() == 1 );
				//if ( s.liberty.size() != 1 ) show_strings();

				// count increasing liberty
				Positions liberty_plus;
				for ( Positions::const_iterator d=s.positions.begin(); d!=s.positions.end(); ++d)
					for ( News n=news(*d); n; n.next() )
						if ( is_disk(*n, Mine) && 1 == ret.string.positions.count(*n) )
							liberty_plus.insert(*n);
				ret.string.liberty.insert(liberty_plus.begin(), liberty_plus.end());

				// remove
				ret.capturable.insert(*a);
			}
		}

		return ret;
	}

public:
	void show_strings(std::ostream& os=std::cout) const
	{
		show(os);

		for ( BlackWhite bw; bw; bw.next() )
		{
			os << player(*bw) << ":";
			for ( Strings::const_iterator s=strings[*bw].begin(); s!=strings[*bw].end(); ++s )
				s->second.show(*this, os << "\t");
		}
	}

};

#endif//STRING_BOARD_H
