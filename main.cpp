
#include <iostream>
#include <memory>
#include <string>

#include "option.h"
#include "Sgf.h"
#include "StringBoard.h"

const std::string Usage =
	"sgf filenam [options]";

int main(int argc, char *argv[])
{
	Option opt(Usage, 10);
	
	opt.set_options()
		("help"                                         , "show this message")
		("version"                                      , "show version")
		("ply"    ,  "show ply")
		("q"    ,  "silently")
		("info"    , Option::Value("%d-%b(%B)%w(%W)%l") , "get game info")
		("record"  , Option::Value("csa")               , "convert record to one of csa, kif, ki2, pico, usi?, fsen?'")
		("board"   , Option::Value("-1")                , "show board at specified ply")
	;

	// parse option
	opt.parse(argc, argv);

	// check
	if ( opt.has("help") )
	{
		std::cout << opt << "\n";
		exit(0);
	}
	if ( opt.count() == 0 )
	{
		std::cout << Usage << "\n";
		exit(1);
	}

	// parse	
	const std::string filename = opt[0];
	Sgf::ErrorOn error_on = opt.has("q")
		?	Sgf::ErrorOnQuiet
		:	Sgf::ErrorOnWarn;
	Sgf record(filename, error_on);

	// show 
	if ( opt.has("ply") )
	{
		std::cout << record.moves.size() << "\n";
	}
}
