#ifndef __DBTable_H__
#define __DBTable_H__
#include <vector>
#include <string>
#include "DBCell.h"

using namespace std;
class DBTable{

	string		name;
	int			col_num;
	vector<vector<DBCell>> cells;
public:
	DBTable(char* table_name,int col_num);
	bool AddData(int Num);
	bool AddData(double Num);
	bool AddData(const char* str);

	bool DataCheck();

};

#endif	/*__DBTable_H__*/