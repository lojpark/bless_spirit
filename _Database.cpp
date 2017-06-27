#pragma once
#include "_stdafx.h"

int Database_nexttrophy(int stagenum, int trophy){
	Uint16 db[40][2]={8,11,
					 13,15,
					 8,11,
					 9,11,
					 14,16,
					 8,10,
					 8,20,
					 15,17,
					 8,10,
					 40,40, // 10
	                 34,36,
					 33,35,
					 30,32,
					 43,45,
					 15,17,
					 12,14,
					 28,30,
					 45,46,
					 16,18,
					 38,40, // 20
					 60,70,
					 29,31,
					 28,31,
					 27,34,
					 18,21,
					 20,23,
					 15,19,
					 14,18,
					 30,40,
					 20,25, // 30
					 16,20,
					 999,999,
					 999,999};
	if(trophy==3) return 0;
	if(trophy==2) return db[stagenum][0];
	if(trophy==1) return db[stagenum][1];
	return 0;
}

int Database_trophy(int stagenum, int time){
	Uint16 db[40][2]={8,11,
					 13,15,
					 8,11,
					 9,11,
					 14,16,
					 8,10,
					 8,20,
					 15,17,
					 8,10,
					 40,40, // 10
	                 34,36,
					 33,35,
					 30,32,
					 43,45,
					 15,17,
					 12,14,
					 28,30,
					 45,46,
					 16,18,
					 38,40, // 20
					 60,70,
					 29,31,
					 28,31,
					 27,34,
					 18,21,
					 20,23,
					 15,19,
					 14,18,
					 30,40,
					 20,25, // 30
					 16,20,
					 999,999,
					 999,999};
	if(time <= db[stagenum][0]){
		return 3;
	}
	else if(time <= db[stagenum][1]){
		return 2;
	}
	return 1;
}