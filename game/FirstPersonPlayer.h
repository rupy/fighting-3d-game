#ifndef __FIRST_PERSON_PLAYER_H__
#define __FIRST_PERSON_PLAYER_H__

#include "Player.h"
#include "AnimObject.h"

class FPPlayer:public Player{
public:
	MoveMode	mode;

	FPPlayer(Camera* cam );
	~FPPlayer();

	void MoveAbsolutely(KeyMessage* keyMessage);
	void MoveRelatively(KeyMessage* keyMessage);

	void Move(KeyMessage* keyMessage);
};

#endif	/*__FIRST_PERSON_PLAYER_H__*/