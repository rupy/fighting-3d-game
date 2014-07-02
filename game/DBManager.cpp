#include "DBManager.h"
#include "Error.h"
#include "UnicodeF.h"
#include <sstream>

#ifndef SQLITE_ENABLE_COLUMN_METADATA
#	define SQLITE_ENABLE_COLUMN_METADATA
#endif

#pragma comment(lib,"sqlite3.lib")
#include "include/sqlite3.h"
bool DBManager::Select(char* filename,char* col_name,char* tablename){

	sqlite3 *db;
	char **pErrMsg = 0;
	sqlite3_stmt *stmt = NULL;
	int r;

	r = sqlite3_open(filename, &db);

	if(r != SQLITE_OK){
		DisplayErrMsg(sqlite3_errmsg(db));
		return false;
	}

	ostringstream select_state;
	select_state<<"SELECT "<<col_name<<" FROM "<<tablename;

	//SQL�����������R���p�C��������
	if(SQLITE_OK == (r = sqlite3_prepare(db, select_state.str().c_str(), strlen(select_state.str().c_str()), &stmt,0))){
		sqlite3_reset(stmt);
		int col_num = sqlite3_column_count(stmt);
		tables.push_back(DBTable(tablename,col_num));//�e�[�u���𐶐�

		while (SQLITE_ROW == (r=sqlite3_step(stmt))){//�s�̃��[�v
			int col_num = sqlite3_column_count(stmt);
			for(int i=0;i<col_num;i++){//��̃��[�v
				int type = sqlite3_column_type(stmt,i);//�󂯎���ނ̔���

				//������p�̕ϐ��錾
				int size;
				const char* sdata;

				switch(type){
					case SQLITE_INTEGER://����
						if(!tables[0].AddData( sqlite3_column_int(stmt, i) )){
							DisplayErrMsg("�f�[�^�����������ł�");
							return false;
						}
						break;
					case SQLITE_FLOAT://���������_
						if(!tables[0].AddData( sqlite3_column_double(stmt,i) )){
							DisplayErrMsg("�f�[�^�����������ł�");
							return false;
						}
						break;
					case SQLITE_TEXT://������
						sdata = (const char*)sqlite3_column_text(stmt, i);
						size = strlen(sdata);
						sdata = CUnicodeF::utf8_to_sjis(sdata,&size);
						if(!tables[0].AddData(sdata)){
							DisplayErrMsg("�f�[�^�����������ł�");
							return false;
						}
						break;
					default://���̑�
						sdata = (const char*)sqlite3_column_text(stmt, i);
						size = strlen(sdata);
						sdata = CUnicodeF::utf8_to_sjis(sdata,&size);
						if(!tables[0].AddData(sdata)){
							DisplayErrMsg("�f�[�^�����������ł�");
							return false;
						}
						break;
				}
			}

		}
		
		if(!tables.back().DataCheck()){
			DisplayErrMsg("�f�[�^�����������ł�");
			return false;
		}
		if (SQLITE_DONE!=r){
			// �G���[
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);


	return true;

}