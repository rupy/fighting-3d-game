#ifndef __DBCell_H__
#define __DBCell_H__
class DBCell{

	union CellData{
		int		iNum;
		double	dNum;
		char	str[50];
	}data;

	
public:
	int dataType;
	bool bNull;

	DBCell(int dataType);

	bool SetData(int iNum);
	bool SetData(double dNum);
	bool SetData(const char* str);

	int GetInt();
	double GetDouble();
	char* GetText();


};

#endif	/*__DBCell_H__*/