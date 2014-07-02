#include "DBTable.h"

DBTable::DBTable(char* table_name,int col_num)
:name(table_name),col_num(col_num){}

bool DBTable::AddData(int Num){

	int row_count = cells.size();//���݂̃e�[�u���̍s��
	int last_col_num;//�Ō�̍s�̗�

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//�V�����s�����
		cells.resize(++row_count);
	}

	//�f�[�^�̑}��
	cells[row_count-1].push_back(DBCell(1));
	cells[row_count-1].back().SetData(Num);

	//�f�[�^�̏ƍ�
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}

bool DBTable::AddData(double Num){

	int row_count = cells.size();//���݂̃e�[�u���̍s��
	int last_col_num;//�Ō�̍s�̗�

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//�V�����s�����
		cells.resize(++row_count);
	}

	//�f�[�^�̑}��
	cells[row_count-1].push_back(DBCell(2));
	cells[row_count-1].back().SetData(Num);

	//�f�[�^�̏ƍ�
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}
bool DBTable::AddData(const char* str){

	int row_count = cells.size();//���݂̃e�[�u���̍s��
	int last_col_num;//�Ō�̍s�̗�

	if(row_count != 0){
		last_col_num = cells[row_count-1].size();
	}else{
		last_col_num = col_num;
	}
	
	if(last_col_num >= col_num){
		//�V�����s�����
		cells.resize(++row_count);
	}

	//�f�[�^�̑}��
	cells[row_count-1].push_back(DBCell(3));
	cells[row_count-1].back().SetData(str);

	//�f�[�^�̏ƍ�
	if(cells[0][cells[row_count-1].size()-1].dataType != cells[row_count-1].back().dataType){
		return false;
	}

	return true;
}

bool DBTable::DataCheck(){
	int row_count = cells.size();//���݂̃e�[�u���̍s��

	if(row_count == 0){
		//�e�[�u���͋�
		return true;
	}

	int last_col_num = cells[row_count-1].size();//�Ō�̍s�̗�

	//�񐔂̃`�F�b�N

	//�Ō�̍s�ȊO�̗�̐����`�F�b�N����
	for(int i=0;i<row_count-1;++i){
		if(col_num != cells[i].size()){
			return false;
		}
	}
	//�Ō�̍s�̓I�[�o�[�̂݃`�F�b�N����
	if(last_col_num > col_num){
		return false;
	}

	//�f�[�^�̏ƍ�

	//�Ō�̍s�ȊO���`�F�b�N����
	for(int i=0;i<row_count-1;++i){
		for(int j=0;j<col_num-1;++j){
			if(cells[0][j].dataType != cells[i][j].dataType){
				return false;
			}
		}
	}
	//�Ō�̍s�͓r���܂�
	for(int j=0;j<last_col_num;++j){
		if(cells[0][j].dataType != cells[row_count-1][j].dataType){
			return false;
		}
	}
	return true;
}