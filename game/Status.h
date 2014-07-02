#ifndef __STATUS_H__
#define __STATUS_H__

#include <cstddef>
#include <string>
#include "Common.h"

#define LEVEL_MAX	100
#define EXP_MAX		100000000//ˆê‰­

using namespace std;

struct Status{

private:
	int Lv;
	int MaxHP;
	int HP;
	int MaxMP;
	int MP;
	int Exp;
	int Attack;
	int Defense;
	int Skill;

public:
	Status();
	Status(
		int Lv,
		int MaxHP,
		int HP,
		int MaxMP,
		int MP,
		int Exp,
		int Attack,
		int Defense,
		int Skill
		);
	
	void InitStatus(
		int Lv,
		int MaxHP,
		int HP,
		int MaxMP,
		int MP,
		int Exp,
		int Attack,
		int Defense,
		int Skill
		);

	bool AllHeal();
	//Level
	bool UpLevel(int up);
	bool SetLevel(int lv);
	int GetLevel();

	//MaxHP
	bool	UpMaxHP(int up);
	bool	SetMaxHP(int max_hp);
	int	GetMaxHP();

	//HP
	bool	DamageHP(int damage);
	bool	HealHP(int heal);
	bool	AllHealHP();
	bool	SetHP(int hp);
	int	GetHP();

	//MaxMP
	bool	UpMaxMP(int up);
	bool	SetMaxMP(int max_mp);
	int	GetMaxMP();

	//MP
	bool	UseMP(int use);
	bool	HealMP(int heal);
	bool	AllHealMP();
	bool	SetMP(int mp);
	int	GetMP();
	
	//Exp
	bool	UpExp(int up);
	bool	SetExp(int exp);
	int	GetExp();

	//Attack
	bool	UpAttack(int up);
	bool	SetAttack(int attack);
	int	GetAttack();

	//Defense
	bool	UpDefense(int up);
	bool	SetDefense(int defense);
	int	GetDefense();
};


#endif	/*__STATUS_H__*/