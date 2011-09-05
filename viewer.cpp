/*
#include "Viewer.h"

int main(int argc, char *argv[])
{
	Viewer().run(argc, argv);
}

int main_sgf(int argc, char *argv[])
{
	std::string filename;
	unsigned ply = 0xffff;

	if ( argc < 2 )
		return intret(0) << "sgf sgf_file [ply]\n";

	if ( 2 <= argc )
		filename = argv[1];
	if ( 3 <= argc )
		ply = lex_cast<unsigned>(argv[2]);

	Sgf sgf(filename);
	StringBoard board(sgf.info.board_size);

	ply = (std::min) (ply, unsigned(sgf.moves.size()));
	for ( unsigned i=0; i<ply; i++ )
		board.make(sgf.moves[i]);
	
	board.show(std::cout << "\n");

	return 0;
}

int sgf_test()
{
	Sgf sgf;
	sgf.load("a.sgf");

	StringBoard board(sgf.info.board_size);

	for ( unsigned i=0; i<sgf.moves.size(); i++ )
	{
		board.show(std::cout << "\n");
		board.make(sgf.moves[i]);
	}
	return 0;
}

#include "StringBoard.h"
int board_test()
{
	typedef StringBoard Board;
	std::auto_ptr<Board> board;
	board.reset(new Board(9));  board->show(std::cerr);

	board->make("e5");	board->make("e6");	board->show();
	board->make("f6");	board->make("f7");	board->show();
	board->make("e7");	board->make("e8");	board->show();
	board->make("d6");						board->show();
	return 0;
}
*/

