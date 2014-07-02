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

	//SQL文が正しくコンパイルされれば
	if(SQLITE_OK == (r = sqlite3_prepare(db, select_state.str().c_str(), strlen(select_state.str().c_str()), &stmt,0))){
		sqlite3_reset(stmt);
		int col_num = sqlite3_column_count(stmt);
		tables.push_back(DBTable(tablename,col_num));//テーブルを生成

		while (SQLITE_ROW == (r=sqlite3_step(stmt))){//行のループ
			int col_num = sqlite3_column_count(stmt);
			for(int i=0;i<col_num;i++){//列のループ
				int type = sqlite3_column_type(stmt,i);//受け取る種類の判別

				//文字列用の変数宣言
				int size;
				const char* sdata;

				switch(type){
					case SQLITE_INTEGER://整数
						if(!tables[0].AddData( sqlite3_column_int(stmt, i) )){
							DisplayErrMsg("データがおかしいです");
							return false;
						}
						break;
					case SQLITE_FLOAT://浮動小数点
						if(!tables[0].AddData( sqlite3_column_double(stmt,i) )){
							DisplayErrMsg("データがおかしいです");
							return false;
						}
						break;
					case SQLITE_TEXT://文字列
						sdata = (const char*)sqlite3_column_text(stmt, i);
						size = strlen(sdata);
						sdata = CUnicodeF::utf8_to_sjis(sdata,&size);
						if(!tables[0].AddData(sdata)){
							DisplayErrMsg("データがおかしいです");
							return false;
						}
						break;
					default://その他
						sdata = (const char*)sqlite3_column_text(stmt, i);
						size = strlen(sdata);
						sdata = CUnicodeF::utf8_to_sjis(sdata,&size);
						if(!tables[0].AddData(sdata)){
							DisplayErrMsg("データがおかしいです");
							return false;
						}
						break;
				}
			}

		}
		
		if(!tables.back().DataCheck()){
			DisplayErrMsg("データがおかしいです");
			return false;
		}
		if (SQLITE_DONE!=r){
			// エラー
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);


	return true;

}