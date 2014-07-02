#ifndef __SAVE_MANAGER_H__
#define __SAVE_MANAGER_H__


#include "GameState.h"

#define SAVE_MAX 21
class SaveManager{
public:
	GameState	state;
	SaveManager();
	~SaveManager();

	void InitState();
	bool DataSave(size_t num);
	bool DataLoad(size_t num);
	bool SaveFileExists(size_t num);
	long CheckSaveFile();

};


#endif	/*__SAVE_MANAGER_H__*/