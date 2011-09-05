#ifndef SGF_COMMANDS_H
#define SGF_COMMANDS_H

#include <string>

// ref. http://www.red-bean.com/sgf/properties.html

#define FF4_COMMAND(DEF) \
	DEF(B,	BlackMove) \
	DEF(W,	WhiteMove) \
	DEF(C,	Comment) \
	DEF(FF,	FileFormat) \
	DEF(VW,	ViewOnly) \
	DEF(SZ,	SizeOfBord) \
	DEF(ST,	ShowTree) \
	DEF(PB,	BlackPlayer) \
	DEF(PW,	WhitePlayer) \
	DEF(BR,	BrackRank) \
	DEF(WR,	WhiteRank) \
	DEF(BT,	BlackTeam) \
	DEF(WT,	WhiteTeam) \
	DEF(RE,	Result) \
	DEF(KM,	Komi) \
	DEF(HA,	HandicapStones) \
	DEF(TM,	Time) \
	DEF(DT,	Date) \
	DEF(EV,	Event) \
	DEF(RO,	Round) \
	DEF(PC,	Place) \
	DEF(RU, Rules) \
	DEF(GN,	GameName) \
	DEF(ON,	Opening) \
	DEF(GM,	GameComment) \
	DEF(SO,	Source) \
	DEF(US,	User) \
	DEF(AN,	Annotation) \
	DEF(CP,	Copyrigt) \
	DEF(LB,	Label) \
	DEF(CA,	CharactorEncoding)\
	DEF(AP,	Application)

enum sgfcommand_t
{
	Unknown,
	// control
	NewNode,		
	BlockBegin,
	BlockEnd,
	// normal
	#define YONLY(X, Y) Y,
		FF4_COMMAND(YONLY)
	#undef YONLY
};

inline sgfcommand_t str2sgfcommand(const std::string& str)
{
	if (false)
		;
	#define ELSEIF(X, Y) else if ( str == #X ) return Y;
		FF4_COMMAND(ELSEIF)
	#undef ELSEIFS
	else
		return Unknown;
}

inline std::string sgfcommand2str(sgfcommand_t c)
{
	switch (c)
	{
	#define CASE(X, Y) case Y: return #Y;
		FF4_COMMAND(CASE)
	#undef CASE
		default: throw std::logic_error("cannot come here");
	}
}

struct SgfCommand
{
	int line_count; // TODO
	std::string error; // TODO
	sgfcommand_t type;
	std::string type_name;	
	std::vector<exstring> param; // TODO tobevector

	SgfCommand()
	{
		line_count = 1;
	}

	void init()
	{
		type = Unknown;
		type_name.clear();
		param.clear();
	}

	std::string type_description() const
	{
		return type == Unknown
			?	"unknown"
			:	 sgfcommand2str(type);
	}
	
	std::string get_error() const
	{
		return tstream("line ") << line_count << " " << error << "\n"; 
	}

	bool read_command(std::istream& is)
	{
		if ( !is ) throw std::runtime_error("bad tstream");

		init();

		while ( is.peek() == '\f' || is.peek() == '\r' || is.peek() == '\n' )
		{
			if ( is.peek() == '\n' ) line_count++;
			is.ignore(1);
		}
		
		if ( false ) ;
		else if ( is.peek() == '(' ) { is.ignore(1); type = BlockBegin;	return true; }
		else if ( is.peek() == ';' ) { is.ignore(1); type = NewNode;	return true; }
		else if ( is.peek() == ')' ) { is.ignore(1); type = BlockEnd;	return true; }
		
		if ( !isalpha(is.peek()) )
		{
			if ( is.peek() == '\n' ) line_count++;
			error = tstream("alphabet is expected. met '") << char(is.peek()) << "'" << "(" << is.peek() << ")";
			is.ignore(1);
			type = Unknown;
			return true;
		}

		// get type 
		do {
			type_name += is.get();
			if ( !is )
				throw std::runtime_error("stream ends unexpectedly"); // 

			// HACK it's not usual
			if ( is.peek() == ';' || is.peek() == ')' )
			{
				type = str2sgfcommand(type_name);
				return true;
			}
		} while ( is.peek() != '[' );

		type = str2sgfcommand(type_name);

 		// at least one parameter required
		if ( is.peek() != '[' )
			throw std::runtime_error("no parameter given");
		else do // for multiple params [][]...
		{
			is.ignore(1); // [

			std::string contents;
			while ( true )
			{
				if ( !is ) throw std::runtime_error("stream ends unexpectedly");
				if ( is.peek() == ']' ) break;
				if ( is.peek() == '\n' ) line_count++;
				contents += is.get();
			}
			param.push_back(contents);
			
			is.ignore(1); // ]

		} while ( is.peek() == '[');

		return true;
	}
};

#undef FF4_COMMAND

#endif//SGF_COMMANDS_H
