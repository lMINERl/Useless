#ifndef _MEMORYREAD_WRITE_
#define _MEMORYREAD_WRITE_

#include "GameWindowUtil.h"
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>
#include <process.h>


using namespace std;
class MemoryRead_Write 
{
private:
	IGameAVailable * igv;
	HANDLE _processHandle ;
	vector<DWORD> _baseAddreses;
	vector<DWORD> _offsets;
	vector<DWORD> _commonOffsets;

	DWORD AddressToWrite();
	void Set_PartToEdit(unsigned int PartToEdit);

	unsigned int _part;
public:
	MemoryRead_Write(HANDLE processHandle);
	~MemoryRead_Write();
	
	void WriteToMemory(unsigned int Part,DWORD  ValueToBeSet);

};

//initializer
MemoryRead_Write::MemoryRead_Write(HANDLE processHandle){
	//this->_processHandle = igv->GetProcessHandle();
	//Static H
	//_baseAddreses.push_back((UINT_PTR)GetModuleHandle("DARKSOULS.exe") + 0x00F75700);	
	this->_processHandle = processHandle;

	_baseAddreses.push_back(0x1378700);
	_commonOffsets.push_back(0x8);

	_offsets.push_back(0x26c);
	_offsets.push_back(0x270);
	_offsets.push_back(0x278);
	_offsets.push_back(0x274);
	_offsets.push_back(0x280);
	_offsets.push_back(0x284);

}
//GC
MemoryRead_Write::~MemoryRead_Write(){
	CloseHandle(this->_processHandle);
	delete igv;
	DeleteObject(this->_processHandle);
	DeleteObject(this);
}

void MemoryRead_Write::WriteToMemory(unsigned int Part,DWORD  ValueToBeSet){
	Set_PartToEdit(Part);
	DWORD addressToWrite = AddressToWrite();
	WriteProcessMemory(this->_processHandle, (BYTE*)addressToWrite ,&ValueToBeSet,sizeof(&ValueToBeSet),NULL);
	//cout<<"done"<<endl;
}
DWORD MemoryRead_Write::AddressToWrite(){
	DWORD_PTR AdderPTR = this->_baseAddreses[0];
	DWORD Temp=0x0;

	ReadProcessMemory(this->_processHandle,(LPCVOID)AdderPTR,&Temp,sizeof(Temp),NULL);

	for(unsigned int i=0;i<this->_commonOffsets.size();++i){
		AdderPTR=(DWORD_PTR)((char*)Temp+_commonOffsets[i]);
		ReadProcessMemory(this->_processHandle,(LPCVOID)AdderPTR,&Temp,sizeof(Temp),NULL);
	}
	AdderPTR=(DWORD_PTR)((char*)Temp+ _offsets[this->_part]);
	ReadProcessMemory(this->_processHandle,(LPCVOID)AdderPTR,&Temp,sizeof(Temp),NULL);

	//cout<<AdderPTR<<endl;
	//cout<<Temp<<endl;
	return AdderPTR;
}
void MemoryRead_Write::Set_PartToEdit(unsigned int PartToEdit){

	if(PartToEdit >= 6){
		printf("part in valid Please enter from 1 ~ 6");
	}else{
		this->_part=PartToEdit;
	}
}

#endif