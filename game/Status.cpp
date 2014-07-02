#include "Status.h"

Status::Status(){}

Status::Status(
	int Lv,
	int MaxHP,
	int HP,
	int MaxMP,
	int MP,
	int Exp,
	int Attack,
	int Defense,
	int Skill)
	:Lv(Lv),MaxHP(MaxHP),HP(HP),MaxMP(MaxMP),MP(MP),Exp(Exp),Attack(Attack),Defense(Defense),Skill(Skill)
{
}
void Status::InitStatus(
	int Lv,
	int MaxHP,
	int HP,
	int MaxMP,
	int MP,
	int Exp,
	int Attack,
	int Defense,
	int Skill)
{
	this->Lv		=	Lv;
	this->MaxHP		=	MaxHP;
	this->HP		=	HP;
	this->MaxMP		=	MaxMP;
	this->MP		=	MP;
	this->Exp		=	Exp;
	this->Attack	=	Attack;
	this->Defense	=	Defense;
	this->Skill		=	Skill;
}

bool Status::AllHeal(){
	AllHealHP();
	AllHealMP();
	return true;
}
//Level
bool Status::UpLevel(int up){

	unless(up + Lv > LEVEL_MAX){
		Lv += up;
		return true;
	}else{
		Lv = LEVEL_MAX;
		return false;
	}

}
bool Status::SetLevel(int lv){

	unless(LEVEL_MAX >lv){
		Lv = lv;
		return true;
	}else{
		return false;
	}
}

int Status::GetLevel(){
	return Lv;
}

//MaxHP
bool Status::UpMaxHP(int up){
	MaxHP += up;
	return true;
}
bool Status::SetMaxHP(int max_hp){
	MaxHP = max_hp;
	return true;
}
int Status::GetMaxHP(){
	return MaxHP;
}

//HP
bool Status::DamageHP(int damage){
	unless(HP - damage	< 0		){
		HP -= damage;
		return true;
	}else{
		HP = 0;
		return false;
	}
}
bool Status::HealHP(int heal){
	unless(HP + heal	> MaxHP	){
		HP += heal;
		return true;
	}else{
		HP = MaxHP;
		return false;
	}
}
bool Status::AllHealHP(){
	HP = MaxHP;
	return true;
}
bool Status::SetHP(int hp){
	unless(hp>MaxHP){
		HP = hp;
		return true;
	}else{
		HP = MaxHP;
		return false;
	}
}
int Status::GetHP(){
	return HP;
}

//MaxMP
bool Status::UpMaxMP(int up){
	MaxMP += up;
	return true;
}
bool Status::SetMaxMP(int max_mp){
	MaxMP = max_mp;
	return true;
}
int Status::GetMaxMP(){
	return MaxMP;
}

//MP
bool Status::UseMP(int use){
	unless(MP - use	< 0	){
		MP -= use;
		return true;
	}else{
		return false;
	}
}
bool Status::HealMP(int heal){
	unless(MP + heal	> MaxMP	){
		MP += heal;
		return true;
	}else{
		MP = MaxMP;
		return false;
	}
}
bool Status::AllHealMP(){
	MP = MaxMP;
	return true;
}
bool Status::SetMP(int mp){
	unless(mp > MaxMP	){
		MP = mp;
		return true;
	}else{
		MP = mp;
		return false;
	}
}
int Status::GetMP()	{
	return MP;
}

//Exp
bool Status::UpExp(int up){
	unless(Exp + up		> EXP_MAX){
		Exp += up;
		return true;
	}else{
		return false;
	}
}
bool Status::SetExp(int exp){
	unless(exp	> EXP_MAX){
		Exp = exp;
		return true;
	}else{
		exp = EXP_MAX;
		return false;
	}
}
int Status::GetExp(){
	return Exp;
}

//Attack
bool Status::UpAttack(int up){
	Attack += up;
	return true;
}
bool Status::SetAttack(int attack){
	Attack = attack;
	return true;
}
int Status::GetAttack(){
	return Attack;
}

//Defense
bool Status::UpDefense(int up){
	Defense += up;
	return true;
}
bool Status::SetDefense(int defense){
	Defense = defense;
	return true;
}
int Status::GetDefense(){
	return Defense;
}