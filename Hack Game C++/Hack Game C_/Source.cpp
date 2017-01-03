#include <Windows.h>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>

#include "GameWindowUtil.h"
#include "MemoryRead_Write.h"

enum Body { Head = 0 , Armor = 1 , Leggings = 2 , Gauntles = 3 , Ring1 = 4 , Ring2 = 5};

int main(int argc,char ** argv){
	IGameAVailable * igv;
	{
		LPCSTR GameWindow="DARK SOULS";
		igv= new GameWindowUtil(GameWindow);
	}
	MemoryRead_Write * mrw (NULL);
	if (!igv->GetGameAvilabilty()){

	}else{
		mrw = new MemoryRead_Write(igv->GetProcessHandle());
		cout<<"Press PGDN to Activate/Deactivate"<<endl;
		SHORT ActiveKey=false;//For PGDN
		bool Activate=false;// is PGDN Pressed ?

		SHORT HotKeyF1 = false;
		bool BooleanHotKeyF1= false;
		SHORT HotKeyF2 = false;
		bool BooleanHotKeyF2= true;
		while (1)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));//Save Processor time

			if(GetAsyncKeyState(VK_PRIOR)){
				DeleteObject( igv );
				DeleteObject(mrw);
				return 0;//Exit on PGUP
			}else{
				ActiveKey = GetAsyncKeyState(VK_NEXT); //Acctivate on PGDN
				(ActiveKey)? Activate=!Activate: NULL;
				/*if(ActiveKey){
				Activate=!Activate;*/

				//}
				if(!Activate){//Program is not Active
				}else{//Program is Active
					bool writeMemory=false;
					HotKeyF1 = GetAsyncKeyState(0x51);
					if(HotKeyF1){
						BooleanHotKeyF1=!BooleanHotKeyF1;
						BooleanHotKeyF2=!BooleanHotKeyF2;
						writeMemory=true;
					}
					if(writeMemory){
						if(igv->GetGameAvilabilty()){
							if(BooleanHotKeyF1){//F1 is Active
								mrw->WriteToMemory(Head,0x0006B6C0);//Base Address
								mrw->WriteToMemory(Armor,0x0006BAA8);
								mrw->WriteToMemory(Leggings,0x0006C278);
								mrw->WriteToMemory(Gauntles,0x0006BE90);
								mrw->WriteToMemory(Ring1,0x00000064);
								mrw->WriteToMemory(Ring2,0x0000008F);
							}
							if(BooleanHotKeyF2){
								mrw->WriteToMemory(Head,0x000493E0);
								mrw->WriteToMemory(Armor,0x0009C7E8);
								mrw->WriteToMemory(Leggings,0x0000CF08);
								mrw->WriteToMemory(Gauntles,0x0009CBD0);
								mrw->WriteToMemory(Ring1,0x00000080);
								mrw->WriteToMemory(Ring2,0x0000008F);
							}
						}
						writeMemory=false;
					}
				}

			}
		}
	}
	DeleteObject( igv );
	if(mrw!=NULL){
		DeleteObject(mrw);
	}
	return 0;
}
