#include "DBCell.h"
#include <cstring>

DBCell::DBCell(int dataType)
:dataType(dataType),bNull(true){
}

bool DBCell::SetData(int Num){
	if(dataType != 1){
		return false;
	}
	data.iNum = Num;

	return true;
}
bool DBCell::SetData(double Num){
	if(dataType != 2){
		return false;
	}
	data.dNum = Num;

	return true;
}
bool DBCell::SetData(const char* str){
	if(dataType != 3){
		return false;
	}
	strcpy(data.str,str);

	return true;

}
int DBCell::GetInt(){
	if(dataType == 1){
		return 0;
	}
	return data.iNum;
}
double DBCell::GetDouble(){
	if(dataType == 2){
		return 0;
	}
	return data.dNum;
}
char* DBCell::GetText(){
	if(dataType == 3){
		return 0;
	}
	return data.str;
}