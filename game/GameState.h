#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "Party.h"

class GameState{
public:
	Party	party;
	int		sceneNum;
	int		placeNum;
	string	file;
	GameState();
	~GameState();
};

#endif	/*__GAME_STATE_H__*/