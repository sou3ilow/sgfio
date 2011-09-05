
#ifndef INPUT_H
#define INPUT_H

#include <map>

#ifdef WIN32
#include <cstdlib>
#include <conio.h>
#pragma warning(disable:4996) // getch
inline void clsprint(const std::string& str)
{
	system("cls");
	std::cout << str;
}

#else
#include <curses.h>
inline void clsprint(const std::string& str)
{
	clear();
	mvprintw(0, 0, str.c_str());
	refresh();
}

#endif

#define KEYS(DEF) \
	DEF(q, Quit, false) \
	DEF(j, Next, false) \
	DEF(k, Previous, false) \
	DEF(h, Start, false) \
	DEF(l, End, false)

struct Command
{
	enum command_t
	{
		Unknown,
		#define YONLY(X, Y, Z) Y,
		KEYS(YONLY)
		#undef YONLY
	};

	command_t type;
	exstring param;

	Command(command_t type, const std::string& param="")
		: type(type)
		, param(param)
	{
	}
};
	
struct Input
{
	typedef std::map<std::string, Command::command_t> CommandMap;
	typedef std::map<Command::command_t, bool> ParamMap;
	CommandMap cmap; // key to command
	ParamMap pmap;

	Input()
	{
		#define CDEF(X, Y, Z) cmap.insert(CommandMap::value_type(std::string(# X), Command::Y));
		#define PDEF(X, Y, Z) pmap.insert(ParamMap::value_type(Command::Y, Z));

		KEYS(CDEF)
		KEYS(PDEF)

		CDEF(\x1b, Quit, false);

		#undef CDEF
		#undef PDEF		

		#ifndef WIN32
		std::ios::sync_with_stdio();
		initscr();
		cbreak();
		noecho();
		#endif
	}
	~Input()
	{
		#ifndef WIN32
		endwin();
		#endif
	}

	Command wait_command()
	{
		std::string command[2];
		int index = 0;

		while ( true )
		{
			int c = getch();

			if (false)
				;
			else if ( c == ' ' || c == '\t' )
			{
				if ( index == 0 && !command[0].empty() )
					index++;
			}
			else if ( c == '\n' || c == '\r' || c == '\f' )
			{
				if ( command[0].empty() )
					;
				else
				{	
					CommandMap::const_iterator i = cmap.find(command[0]);

					return ( i != cmap.end() )
						?	Command(i->second, command[1])
						:	Command(Command::Unknown, command[1]);
				}
			}
			else
			{
				command[index] += char(c);

				CommandMap::const_iterator i;
				if ( command[0].size() == 1 
				  && cmap.end() != ( i = cmap.find(command[0])) 
				  && pmap[i->second] == false
				  )
					return Command(i->second);
			}
		} // while
	}
};

#undef KEYS

#endif//INPUT_H
