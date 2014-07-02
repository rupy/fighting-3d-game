#ifndef __STATUS_MENU_H__
#define __STATUS_MENU_H__

#include "Party.h"
#include <d3dx9.h>
#include "MySpriteObject.h"
#include "NumberObject2.h"
#include "StringObject2.h"

class StatusMenu{
public:
	MySpriteObject backGround;
protected:
	Party* partydata;
	enum CurPos{
		NAME,
		HP,
		MP,
		EXP,
		ATTACK,
		DEFENSE,
		SKILL,
	};

	CurPos curPos;
	StringObject strName1;
	StringObject strName2;
	StringObject strName3;
	StringObject strName4;
	StringObject strLv1;
	StringObject strLv2;
	StringObject strLv3;
	StringObject strLv4;
	StringObject strHP1;
	StringObject strHP2;
	StringObject strHP3;
	StringObject strHP4;
	StringObject strPP1;
	StringObject strPP2;
	StringObject strPP3;
	StringObject strPP4;
	StringObject strHPsep1;
	StringObject strHPsep2;
	StringObject strHPsep3;
	StringObject strHPsep4;
	StringObject strPPsep1;
	StringObject strPPsep2;
	StringObject strPPsep3;
	StringObject strPPsep4;
	StringObject strGold;
	NumberObject numGold;
	NumberObject numLv1;
	NumberObject numLv2;
	NumberObject numLv3;
	NumberObject numLv4;
	NumberObject numHP1;
	NumberObject numHP2;
	NumberObject numHP3;
	NumberObject numHP4;
	NumberObject numPP1;
	NumberObject numPP2;
	NumberObject numPP3;
	NumberObject numPP4;
	NumberObject numMaxHP1;
	NumberObject numMaxHP2;
	NumberObject numMaxHP3;
	NumberObject numMaxHP4;
	NumberObject numMaxPP1;
	NumberObject numMaxPP2;
	NumberObject numMaxPP3;
	NumberObject numMaxPP4;

public:	
	StatusMenu(Party* party);
	~StatusMenu();

	void SetData();


	void Appear();

};


#endif	/*__STATUS_MENU_H__*/