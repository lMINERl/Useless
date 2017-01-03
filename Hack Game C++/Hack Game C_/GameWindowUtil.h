#ifndef _GAMEWINDOWUTIL_
#define _GAMEWINDOWUTIL_

#include <string>
#include <Windows.h>
#include <iostream>
using namespace std;

__interface IGameAVailable
{
	bool GetGameAvilabilty();
	HANDLE GetProcessHandle();
	DWORD GetProcessId();
};

class GameWindowUtil : public  IGameAVailable {

private:

	//Fields
	LPCSTR _windowName;
	HWND _gameHwnd;
	DWORD _processId;
	HANDLE _processHandle;

	//Functions
	void FindGameWindow();
	void GetGameWindowProcessId();
	void OpenGameProcess();
	bool IsReadyToHack ();

public:

	//Functions
	bool GetGameAvilabilty();
	DWORD GetProcessId();
	HANDLE GetProcessHandle();

	//Constructors & Destructors
	GameWindowUtil(LPCSTR WindowName);
	~GameWindowUtil();
};
//Constructor
GameWindowUtil::GameWindowUtil(LPCSTR WindowName){
	this->_windowName=WindowName;
	this->_gameHwnd=NULL;
	this->_processId=NULL;
	this->_processHandle=NULL;
}
//Destructor
GameWindowUtil::~GameWindowUtil(){
	CloseHandle(this->_processHandle);
	delete this->_gameHwnd;
	DeleteObject(this->_gameHwnd);
	DeleteObject(this->_processHandle);
	DeleteObject(this->_gameHwnd);
	DeleteObject(this);
}

void GameWindowUtil::FindGameWindow(){

	this->_gameHwnd = FindWindow(NULL,this->_windowName); 
	//this->_gameHwnd = FindWindowEx(NULL,0,0,this->_windowName); //depends on window you are running
}

void GameWindowUtil::GetGameWindowProcessId(){
	if(this->_gameHwnd){
		GetWindowThreadProcessId(this->_gameHwnd,&this->_processId);
	}else{
		printf("Game Window Not Found\n");
	}
}

void GameWindowUtil::OpenGameProcess(){
	if(this->_processId!=0){
		this->_processHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,this->_processId);
		
	}else{
		printf("Failed to get Process Id\n");
	}
}

bool GameWindowUtil::IsReadyToHack(){
	this->FindGameWindow();
	this->GetGameWindowProcessId();
	this->OpenGameProcess();
	if(this->_processHandle == INVALID_HANDLE_VALUE || this->_processHandle==NULL){
		printf("Not Ready to Hook\n");
		return false;
	}else{
		printf("Ready to Hook \n");
		return true;
	}
}

bool GameWindowUtil::GetGameAvilabilty(){
	return this->IsReadyToHack();
}
HANDLE GameWindowUtil::GetProcessHandle(){
	return this->_processHandle;
}
DWORD GameWindowUtil::GetProcessId(){
	return this->_processId;
}
#endif