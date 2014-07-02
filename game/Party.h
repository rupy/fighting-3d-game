#ifndef __PARTY_H__
#define __PARTY_H__

#include "Status.h"
#include <map>
#include <string>

using namespace std;

#define MONEY_MAX 100000000//ˆê‰­
#define ITEM_MAX 20

struct Party{
public:

	Status			member1;
	Status			member2;
	Status			member3;
	Status			member4;

private:

	size_t			Money;
	map<string,int>	Items;

public:

	void	InitParty(size_t money);

	//Money
	void	EarnMoney(size_t earn);
	void	SpendMoney(size_t spend);
	void	SetMoney(size_t money);
	size_t	GetMoney();

	//Items
	size_t AddItem(char* name);
	size_t LostItem(char* name);
	size_t GetHowManyItems(char* name);
};

#endif	/*__PARTY_H__*/