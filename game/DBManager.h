#ifndef __DB_Manager_H__
#define __DB_Manager_H__
#include <vector>
#include "DBTable.h"
using namespace std;

class DBManager{
public:
	//�Ǘ�����e�[�u��
	vector<DBTable> tables;

	bool Select(char* filename,char* col_name,char* tablename);
};
#endif	/* __DB_Manager_H__*/