#include "DBTable.h"

DBTable::DBTable(char* table_name,int col_num)
:name(table_name),col_num(col_num){}

bool DBTable::AddData(int Num){

	int row_count = cells.size();//現在のテーブルの行数
	int last_col_num;//最後の行の列数

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//新しい行を作る
		cells.resize(++row_count);
	}

	//データの挿入
	cells[row_count-1].push_back(DBCell(1));
	cells[row_count-1].back().SetData(Num);

	//データの照合
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}

bool DBTable::AddData(double Num){

	int row_count = cells.size();//現在のテーブルの行数
	int last_col_num;//最後の行の列数

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//新しい行を作る
		cells.resize(++row_count);
	}

	//データの挿入
	cells[row_count-1].push_back(DBCell(2));
	cells[row_count-1].back().SetData(Num);

	//データの照合
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}
bool DBTable::AddData(const char* str){

	int row_count = cells.size();//現在のテーブルの行数
	int last_col_num;//最後の行の列数

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//新しい行を作る
		cells.resize(++row_count);
	}

	//データの挿入
	cells[row_count-1].push_back(DBCell(3));
	cells[row_count-1].back().SetData(str);

	//データの照合
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}

bool DBTable::DataCheck(){
	int row_count = cells.size();//現在のテーブルの行数

	if(row_count == 0){
		//テーブルは空
		return true;
	}

	int last_col_num = cells[row_count-1].size();//最後の行の列数

	//列数のチェック

	//最後の行以外の列の数をチェックする
	for(int i=0;i<row_count-1;++i){
		if(col_num != cells[i].size()){
			return false;
		}
	}
	//最後の行はオーバーのみチェックする
	if(last_col_num > col_num){
		return false;
	}

	//データの照合

	//最後の行以外をチェックする
	for(int i=0;i<row_count-1;++i){
		for(int j=0;j<col_num-1;++j){
			if(cells[0][j].dataType != cells[i][j].dataType){
				return false;
			}
		}
	}
	//最後の行は途中まで
	for(int j=0;j<last_col_num;++j){
		if(cells[0][j].dataType != cells[row_count-1][j].dataType){
			return false;
		}
	}
	return true;
}