// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include <stdlib.h>
#include<vector>

#define FALSE 0
#define TRUE 1

struct Gen sealed{ //Gen for
	uint_fast16_t GenID:1;							//Identifies The group ID either 0~1
	uint_fast16_t IsPaired:1;						//Indecates if its Paired or not
	uint_fast16_t PairedWithPreferedIndex:14;		//from 0 ~ 16,383 u have only 14 bits- contains the index of the prefrence list which is paired with
	std::vector<uint_fast16_t> PrefrenceList;		//list that Holds The Preferable pairing from the other group from lowest to highest ie: 0 is loswest 9 is highest(note:. each member have full prefrencelist to the other group)
	std::vector<uint_fast16_t> PairingListRequest;  //list that Holds The Pairing Request
	void fn_Erase(){ //reset values
		GenID=0;
		IsPaired=FALSE;
		PairedWithPreferedIndex=0;
		PrefrenceList.clear();
		PairingListRequest.clear();
	}
};

void fn_RequestPairing(std::vector<Gen>& G1,uint_fast16_t Periority); //Request Pairing from the other group according to thier top periority
void fn_AcceptRejectPairing(std::vector<Gen>& G2);//Accept the higher periorty According to Prefrence list and reject all the others
uint_fast16_t fn_GetTheRemainingUnPaired(std::vector<Gen> &gen);//Check for Remaining unpaired Group for pairing
int _tmain(int argc, _TCHAR* argv[]){

	//printf("%d",sizeof(Gen));
	const uint_fast16_t GenNum (20);	//must be even to create stable else will create soft of unpaired members
	const uint_fast16_t HalfGenNum(GenNum/2);
	std::vector<Gen> gen;
	uint_fast16_t i(0);
	//initialize - categorize groups  and randomize prefrence list to the opposite group
	{
		uint_fast16_t j(0);
		Gen tempGen;
		uint_fast16_t temp(0);
		uint_fast16_t PosARandom(0);
		uint_fast16_t PosBRandom(0);
		//categorize gen into 2 groups

		for(i=0;i<GenNum;++i){
			if(i>=HalfGenNum){//split Gen into 2 groups 1 / 0
				tempGen.GenID=0;
				for(j=0;j<HalfGenNum;++j){//initialize PrefrenceList from 0~9
					tempGen.PrefrenceList.push_back(j);
				}
			}else{
				tempGen.GenID=1;
				for(j=HalfGenNum;j<GenNum;++j){//initialize PrefrenceList from 10~19
					tempGen.PrefrenceList.push_back(j);
				}
			}
			for(j=0;j<HalfGenNum;++j){//randomize templist by swapping the 2 random positions
				PosARandom=rand()%HalfGenNum+0;
				PosBRandom=rand()%HalfGenNum+0;

				temp=tempGen.PrefrenceList[PosARandom];
				tempGen.PrefrenceList[PosARandom]=tempGen.PrefrenceList[PosBRandom];
				tempGen.PrefrenceList[PosBRandom]=temp;
			}

			gen.push_back(tempGen);
			tempGen.fn_Erase();
		}
	}
	//Algorithm
	//for all group 1 suggest to pair up with the 2nd group according to thier prefrence list

	uint_fast16_t RemainingUnpaired=0;
	i=0;
	do{
		fn_RequestPairing(gen,gen[i].PrefrenceList.size()-1-i); //Request pairing from top periortiy 
		fn_AcceptRejectPairing(gen);							//other group choose to accept or reject

		RemainingUnpaired=fn_GetTheRemainingUnPaired(gen);		//see if still Remaining unpaired
		++i;
		if(RemainingUnpaired>1&&i>=gen[i].PrefrenceList.size()){//if ther is still member that is not paired revisite the prefrence list 
			i=0;
		}
	}while(RemainingUnpaired);

	//Display All
	for(i=0;i<gen.size();++i){
		if(gen[i].GenID==0){
			printf("%d is paired with score is %d \n",i,gen[i].PairedWithPreferedIndex);
		}
	}
	//Erase all prefrence list
	for(i=0;i<gen[i].PrefrenceList.size();++i){
		gen[i].PrefrenceList.clear();
	}
	
	//Display total size
	int total_size=0;
	for(i=0;i<gen.size();++i){
		total_size=total_size+sizeof(gen[i]);
	}
	printf("%d",total_size);
	return 0;
}

void fn_RequestPairing(std::vector<Gen>& G1,uint_fast16_t Periority){
	printf("\n");
	printf("Request Pairing:\n");
	uint_fast16_t i=0;
	for(uint_fast16_t i=0;i<G1[i].PrefrenceList.size();++i){
		if(G1[i].GenID==1){
			if(!G1[i].IsPaired){
				if(Periority<G1[i].PrefrenceList.size()){
					G1[G1[i].PrefrenceList[Periority]].PairingListRequest.push_back(i);
					printf("%d requestPairing with %d \n",i,G1[i].PrefrenceList[Periority]);
				}else{
					printf("Periority cant be  More than Gen.PrefrenceList fn_RequestPairing Main" );
				}
			}
		}
	}
}
void fn_AcceptRejectPairing(std::vector<Gen>& G2){
	printf("Accept Pairing \n");
	for(uint_fast16_t i=0;i<G2.size();++i){
		if(G2[i].GenID==0){//pick only the other group

			for(uint_fast16_t j=0;j<G2[i].PairingListRequest.size();++j){
				for(uint_fast16_t k=0;k<G2[i].PrefrenceList.size();++k){

					if(G2[i].PairingListRequest[j]==G2[i].PrefrenceList[k]){
						if(G2[i].PairedWithPreferedIndex<=k){//if the next one is higher in Prefernce list
							if(G2[i].IsPaired){//Break with the Previous one
								G2[G2[i].PrefrenceList[G2[i].PairedWithPreferedIndex]].IsPaired=FALSE;
								printf("%d Broke with %d \n",i,G2[i].PrefrenceList[G2[i].PairedWithPreferedIndex]);
							}
							G2[G2[i].PrefrenceList[k]].IsPaired=TRUE;
							G2[i].IsPaired=TRUE;
							G2[i].PairedWithPreferedIndex=k;
							printf("%d Paired with %d \n",i,G2[i].PrefrenceList[G2[i].PairedWithPreferedIndex]);
							break;
						}

					}
				}
			}
			G2[i].PairingListRequest.clear();
		}
	}
}
uint_fast16_t fn_GetTheRemainingUnPaired(std::vector<Gen>& gen){
	uint_fast16_t j=0;
	for(uint_fast16_t i=0;i<gen.size();++i){
		if(!gen[i].IsPaired){
			++j;
		}
	}
	return j;
}