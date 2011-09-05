// test by: g++ -DMACRO_TEST table.h -E

// char, desc, id, kind
#define ID_GAME(DEF) \
	DEF( d , "date            " , date           , GAME)  \
	DEF( l , "game length     " , game_length    , GAME)  \
	DEF( b , "black name      " , black_name     , GAME)  \
	DEF( w , "white name      " , white_name     , GAME)  \
	DEF( B , "black elo rating" , black_rate     , GAME)  \
	DEF( W , "white elo rating" , white_rate     , GAME)  \
	DEF( s , "start at        " , start_at       , GAME)  \
	DEF( f , "finish at       " , end_at         , GAME)  \
	DEF( e , "event name      " , event_name     , GAME)  \
	DEF( t , "title           " , title          , GAME)  \
	DEF( o , "opening         " , opening        , GAME)
#define ID_BOARD(DEF) \
	DEF( p , "ply             " , ply            , BOARD) \
	DEF( K , "Kakinoki boad   " , kakinoki_board , BOARD) \
	DEF( J , "JSA board       " , jsa_board      , BOARD) \
	DEF( U , "USI board       " , usi_board      , BOARD) \
	DEF( C , "CSA board       " , csa_board      , BOARD) \
	DEF( P , "pico board      " , pico_board     , BOARD) \
	DEF( E , "EPS board       " , eps_board     , BOARD) \
	DEF( S , "SVG board       " , svg_board      , BOARD) \
	DEF( O , "OhTex board     " , othex_board    , BOARD) \
	DEF( c , "comment         " , comment        , BOARD) \
	DEF( m , "last move       " , last_move      , BOARD) \
	DEF( n , "next move       " , next_move      , BOARD) \
	DEF( M , "legal moves     " , legal_moves    , BOARD) \
	DEF( N , "legal moves num " , legal_move_num , BOARD)

#define USAGE_STR(C, DESC, ID, TYPE) \
	const std::string ID = " %" #C " " DESC " ";

#define CHECK_CHAR_DUPLICATE(C, DESC, ID, TYPE) \
	const char C;

#define CHECK_ID_DUPLICATE(C, DESC, ID, TYPE) \
	const char ID;

#ifdef MACRO_TEST
namesapce char_dup {
	ID_GAME(CHECK_CHAR_DUPLICATE)
	ID_BOARD(CHECK_CHAR_DUPLICATE)
}
namesapce id_dup { 
	ID_GAME(CHECK_ID_DUPLICATE) 
	ID_BOARD(CHECK_ID_DUPLICATE) 
}
//IDS(USAGE_STR)
#endif


