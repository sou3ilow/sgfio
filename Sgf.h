
#ifndef SGF_H
#define SGF_H

#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <memory>

#include "util.h" 
#include "types.h"
#include "SgfCommand.h"
#include "Const.h"

/*
 * TODO
 * though sgf has recurcive format, current implementation cannot treat it.
 */


// element of game record
struct SgfMove
{
	position_t move;
	std::string comment;

	void init()
	{
		move = Const::NullMove;
		comment.clear();
	}

	operator position_t() const
	{
		return move;
	}
};

// the record
class SgfMoves : public std::vector<SgfMove>
{
private:
	int edit_index;

public:
	SgfMoves()
	{
		init();
	}

	void init()
	{
		clear();
		reserve(200); // HACK
		edit_index = -1; // HACK
	}

	void edit_next()
	{
		edit_index++;
		resize(edit_index+1);
		at(edit_index).init();
	}

	int edit_current_index()
	{
		return edit_index;
	}

	SgfMove& edit_current()
	{
		if ( edit_index < 0 )
			throw std::runtime_error("edit_next has to call befor edit_current");
		return at(edit_index);
	}
};

struct GameInfo
{
	double komi;
	int handicap_stones;
	Rule rule;
	int board_size;

	struct Player
	{
		std::string name;
		std::string rank;
		std::string team;
	};

	Player black, white;
	std::string result;
	std::string time;
	std::string date;
	std::string event_name;
	std::string round_of_game;
	std::string place;
	std::string game_name;
	std::string openingng;
	std::string game_comment;
	std::string source;
	std::string user_of_application;
	std::string annotation;
	std::string copyright;
	std::string filename;
	std::string charactor_encoding;
	int file_format_version;
	int lines;
};

struct Sgf
{
	enum ErrorOn { ErrorOnQuiet, ErrorOnWarn, ErrorOnAbort };

public:
	SgfMoves moves;

	GameInfo info;

	std::auto_ptr<Const> constants;

	static const int default_size = 19;
private:
	void change_board_size(int new_size)
	{
		if ( constants->Size != new_size )
			constants.reset(new Const(new_size));
		info.board_size = new_size;
	}

	void add_comment(const std::string& str)
	{
		if ( moves.edit_current_index() < 0 )
			info.game_comment += str;
		else
			moves.edit_current().comment += str;
	}

public:
	Sgf(const std::string& filename="", ErrorOn error_on=ErrorOnWarn)
		:	constants(new Const(default_size))
	{
		moves.init();
		if ( !filename.empty() )
			load(filename, error_on);
	}


	bool load(const std::string& filename, ErrorOn error_on=ErrorOnWarn)
	{
		std::ifstream inf(filename.c_str());

		if ( !inf )
			return boolret(false) << "cannot open file: " << filename;
			//throw std::runtime_error(tstream("cannot open file: ") << filename);
		return read(inf, error_on);
	}

	/// for stream. use load for file
	bool read(std::istream& is, ErrorOn error_on=ErrorOnWarn)
	{
		int level = 0;
		SgfCommand com;
		while ( is )
		{
			com.read_command(is);

			switch ( com.type )
			{
			// 
			case NewNode:			break; // HACK
			case BlockBegin:		level++; break; // HACK
			case BlockEnd:			level--; break; // HACK
			case Comment:			add_comment(com.param[0]); break;
			case BlackMove:			moves.edit_next(); moves.edit_current().move = constants->sgf2pos(com.param[0], true); break;
			case WhiteMove:			moves.edit_next(); moves.edit_current().move = constants->sgf2pos(com.param[0], true); break;
			//

			case FileFormat:		info.file_format_version = com.param[0].to<int>(); break;
			case CharactorEncoding:	info.charactor_encoding = com.param[0]; break;
			case SizeOfBord:		change_board_size(com.param[0].to<int>()); break;

			case BlackPlayer:		info.black.name = com.param[0]; break;
			case WhitePlayer:		info.white.name = com.param[0]; break;
			case BrackRank:			info.black.rank = com.param[0]; break;
			case WhiteRank:			info.white.rank = com.param[0]; break;
			case BlackTeam:			info.black.team = com.param[0]; break;
			case WhiteTeam:			info.white.team = com.param[0]; break;
			case Result:			info.result = com.param[0]; break;
			case Komi:				info.komi = com.param[0].to<double>(); break;
			case HandicapStones:	info.handicap_stones = com.param[0].to<int>(); break;

			case Time:				info.time = com.param[0]; break;
			case Date:				info.date = com.param[0]; break;
			case Event:				info.event_name = com.param[0]; break;
			case Round:				info.round_of_game = com.param[0]; break;
			case Place:				info.place = com.param[0]; break;
			case Rules:				info.rule = com.param[0].to<Rule>(); break;
			case GameName:			info.game_name = com.param[0]; break;
			case Opening:			info.openingng = com.param[0]; break;
			case GameComment:		info.game_comment = com.param[0]; break;
			case Source:			info.source = com.param[0]; break;
			case User:				info.user_of_application = com.param[0]; break;
			case Annotation:		info.annotation = com.param[0]; break;
			case Copyrigt:			info.copyright = com.param[0]; break;

			case Unknown:			
									if ( error_on == ErrorOnQuiet ) break;
									std::cerr << "unknown: " << com.get_error() << "\n";
									if ( error_on == ErrorOnAbort ) return false;
			default:				
									if ( error_on == ErrorOnQuiet ) break;
									std::cerr << "ignored: " << com.type_name << ":" << com.type_description() << "\n"; break;
									if ( error_on == ErrorOnAbort )
										return false;
			}

			if ( level == 0 )
				break;
		}// while
		return true;
	}
};





#endif//SGF
