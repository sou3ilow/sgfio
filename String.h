#ifndef STRING_H
#define STRING_H

#include <set>
#include "Const.h"

typedef std::set<position_t> Positions;

struct String
{
	Positions positions;
	Positions liberty;

	void init()
	{
		positions.clear();
		liberty.clear();
	}

	int anchor() const
	{
		return *positions.begin();
	}

	void join(const String& s)
	{
		positions.insert(s.positions.begin(), s.positions.end());
		liberty.insert(s.liberty.begin(), s.liberty.end());
	}

	void show(const Const& c, std::ostream& os=std::cout) const
	{
		os << c.pos2gtp(anchor());

		os << " >>";
		for ( Positions::const_iterator i=positions.begin(); i!=positions.end(); ++i )
			os << ' ' << c.pos2gtp(*i);

		os << " >>";
		for ( Positions::const_iterator i=liberty.begin(); i!=liberty.end(); ++i )
			os << ' ' << c.pos2gtp(*i);

		os << "\n";
	}

	void show(std::ostream& os=std::cout) const
	{
		for ( Positions::const_iterator i=positions.begin(); i!=positions.end(); ++i )
			os << ' ' << *i;
	}

	void show_debug() const
	{
		show(std::cerr);
	}
};

#endif//STRING_H
