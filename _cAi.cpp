#pragma once
#include "_stdafx.h"

void Ai::Set(){
	up[0] = false;
	down[0]= false;
	left[0] = false;
	right[0] = false;
	up[1] = false;
	down[1]= false;
	left[1] = false;
	right[1] = false;
	n = 0;
	time = 0;
	timed = 0;
}

void Ai::Load(int stagenum){
	int temp;
	char filename[20]={"data\\ai\\ai000_f.txt"};
	filename[11]=stagenum/10 + '0';
	filename[12]=stagenum%10 + '0';
	fopen_s(&in_f,filename,"r");
	if(in_f!=NULL){
		fscanf_s(in_f,"%d",&temp);
		fscanf_s(in_f,"%d%d%d",&record_f[0],&record_f[1],&record_f[2]);
	}
	filename[14]='w';
	filename[11]=stagenum/10 + '0';
	filename[12]=stagenum%10 + '0';
	fopen_s(&in_w,filename,"r");
	if(in_w!=NULL){
		fscanf_s(in_w,"%d",&temp);
		fscanf_s(in_w,"%d%d%d",&record_w[0],&record_w[1],&record_w[2]);
	}
}

/*void Ai::Save(){
	int i;
	FILE *out;
	fopen_s(&out,"data\\ai.txt","w");
	fprintf(out,"%d\n",n);
	for(i=0;i<n;i++){
		fprintf(out,"%d %d %d\n",record[0][i],record[1][i],record[2][i]);
	}
	fclose(out);
}*/