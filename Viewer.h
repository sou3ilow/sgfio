
#ifndef VIEWER_H
#define VIEWER_H

#include "StringBoard.h"
#include "Sgf.h"
#include "console.h"

class Viewer
{
	Input input;
	Sgf sgf;
	typedef StringBoard Board;
	std::auto_ptr<Board> board;

public:

	bool init(const std::string& filename, int ply)
	{
		if ( filename.empty() )
			return boolret(false) << "usage: sgfview sgffile [ply]\n";
		else if ( !sgf.load(filename) )
			return boolret(false) << "cannot load file: " << filename << "\n";
		else
		{
			board.reset(new Board(sgf.info.board_size));
			board->komi = sgf.info.komi;
			goto_ply(ply);
			return true;
		}
	}

	void goto_ply(int ply)
	{
		assert ( board.get() );
		board->init();

		ply = (std::max) (ply, 0);
		ply = (std::min) (ply, int(sgf.moves.size()));

		int i;
		try {
			for ( i=0; i<ply; i++ )
				board->make(sgf.moves[i]);
		}
		catch(std::runtime_error& error)
		{
			std::cerr
				<< error.what() << "\n"
				<< "cannot make " << i << "th move: " << board->pos2gtp(sgf.moves[i]) << "\n"
				<< "hit enter";
			std::string str;
			std::getline(std::cin, str);
		}

	}

	void run(int argc=0, char* argv[]=NULL)
	{
		std::string filename;
		unsigned ply = 0xffff;
		
		if ( 2 <= argc ) filename = argv[1];
		if ( 3 <= argc ) ply = lex_cast<unsigned>(argv[2]);

		if ( init(filename, ply) )
			loop();
	}

	void loop()
	{
		bool looping = true;
		while ( looping )
		{
			if ( board.get() )
			{
				std::stringstream ss;
				board->show(ss);
				clsprint(ss.str());
			}

			Command com = input.wait_command();

			switch ( com.type )
			{
			case Command::Next:     if ( board.get() ) goto_ply(board->ply + 1); break;
			case Command::Previous: if ( board.get() ) goto_ply(board->ply - 1); break;
			case Command::Start:    if ( board.get() ) goto_ply(0); break;
			case Command::End:      if ( board.get() ) goto_ply(0xffff); break;
			case Command::Quit:     looping = false; break;
			case Command::Unknown:  std::cerr << "unknown command\n";
			}
		}
	}// loop
};

#endif//VIEWER_H

