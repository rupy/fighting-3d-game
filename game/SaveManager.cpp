#pragma comment( lib, "shlwapi.lib" )

#include "SaveManager.h"
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>
#include <sstream>
#include <iomanip>
SaveManager::SaveManager(){
}
SaveManager::~SaveManager(){
}
void SaveManager::InitState(){
	state.party.member1.InitStatus(1,30,30,10,10,0,20,18,0);
	state.party.member2.InitStatus(1,20,20,20,20,0,15,10,0);
	state.party.member3.InitStatus(1,30,30,10,10,0,20,18,0);
	state.party.member4.InitStatus(1,20,20,20,20,0,15,10,0);
	state.party.InitParty(200);
	state.party.AddItem("‚â‚è");
	state.sceneNum=0;
	state.placeNum=0;
}
bool SaveManager::DataSave(size_t num){
	FILE* save_file;

	ostringstream filename;
	filename<<"save/save"<<setfill('0')<<setw(2)<<num<<".dat";

	save_file = fopen(filename.str().c_str(),"wb+");

	fwrite(&state,sizeof(GameState),1,save_file);

	fclose(save_file);

	return true;
}
bool SaveManager::DataLoad(size_t num){

	FILE* load_file;

	ostringstream filename;
	filename<<"save/save"<<setfill('0')<<setw(2)<<num<<".dat";

	load_file = fopen(filename.str().c_str(),"rb+");

	if(!load_file){return false;}

	fread(&state,sizeof(GameState),1,load_file);

	fclose(load_file);
	return true;
}
bool SaveManager::SaveFileExists(size_t num){
	ostringstream filename;
	filename<<"save/save"<<setfill('0')<<setw(2)<<num<<".dat";
	return PathFileExists(filename.str().c_str())?true:false ; 
}
long SaveManager::CheckSaveFile(){

	size_t count=0;
	for(size_t i=0;i<SAVE_MAX;++i)
		count += SaveFileExists(i);
	return count;
}