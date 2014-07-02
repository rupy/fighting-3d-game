#include "Party.h"

void Party::InitParty(size_t money){
	Money = money;
}

//***********************************************************************************************************************
//�p�[�e�B�[�X�e�[�^�X���상�\�b�h��	|					����				|				�ʏ펞					|
//										|										|				��O��					|
//***********************************************************************************************************************

//Money
void Party::EarnMoney(size_t earn)		{	unless(Money + earn	> MONEY_MAX		)		Money += earn;	
																				else	Money = MONEY_MAX;				}
void Party::SpendMoney(size_t spend)	{	unless(Money - spend< 0				)		Money -= spend;
																				else	Money = 0;						}
void Party::SetMoney(size_t money)		{	unless(Money		> MONEY_MAX		)		Money = money;					}
size_t Party::GetMoney()				{												return Money;					}

//Items
size_t Party::AddItem(char* name)		{	unless(Items[name]+1> ITEM_MAX		)		return ++Items[name];
																				else	return 0;						}
size_t Party::LostItem(char* name)		{	unless(Items[name]-1< 0				){		return --Items[name];			}
																				else{	Items.erase(name);
																						return 0;					}	}
size_t Party::GetHowManyItems(char* name){	unless(Items.find(name)==Items.end())		return Items[name];
																				else	return 0;						}