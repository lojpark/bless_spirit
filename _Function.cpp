#pragma once
#include "_stdafx.h"

bool isCrash(int x1, int y1, int x2, int y2, int w, int h){
	if( x1 >= (x2 - w/2) && x1 <= (x2 + w/2) && y1 >= (y2 - h/2) && y1 <= (y2 + h/2) ){
		return true;
	}
	return false;
}
bool isCrash2(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	if( x1 + w1/2 >= x2 - w2/2 && x1 + w1/2 <= x2 + w2/2 ){
		if( y1 + h1/2 >= y2 - h2/2 && y1 + h1/2 <= y2 + h2/2 ){
			return true;
		}
		if( y1 - h1/2 <= y2 + h2/2 && y1 - h1/2 >= y2 - h2/2 ){
			return true;
		}

		if( y1 + h1/2 >= y2 + h2/2 && y1 - h1/2 <= y2 - h2/2 ){
			return true;
		}
		if( y2 + h2/2 >= y1 + h1/2 && y2 - h2/2 <= y1 - h1/2 ){
			return true;
		}
	}
	if( x1 - w1/2 <= x2 + w2/2 && x1 - w1/2 >= x2 - w2/2 ){
		if( y1 + h1/2 >= y2 - h2/2 && y1 + h1/2 <= y2 + h2/2 ){
			return true;
		}
		if( y1 - h1/2 <= y2 + h2/2 && y1 - h1/2 >= y2 - h2/2 ){
			return true;
		}
		if( y1 + h1/2 >= y2 + h2/2 && y1 - h1/2 <= y2 - h2/2 ){
			return true;
		}
		if( y2 + h2/2 >= y1 + h1/2 && y2 - h2/2 <= y1 - h1/2 ){
			return true;
		}
	}
	return false;
}
bool isCrash2_l(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	if( x1 + w1/2 >= x2 - w2/2 && x1 + w1/2 <= x2 + w2/2 ){
		if( y1 + h1/2 >= y2 - h2/2 && y1 + h1/2 <= y2 + h2/2 ){
			return true;
		}
		if( y1 - h1/2 <= y2 + h2/2 && y1 - h1/2 >= y2 - h2/2 ){
			return true;
		}
	}
	return false;
}
bool isCrash2_r(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	if( x1 - w1/2 <= x2 + w2/2 && x1 - w1/2 >= x2 - w2/2 ){
		if( y1 + h1/2 >= y2 - h2/2 && y1 + h1/2 <= y2 + h2/2 ){
			return true;
		}
		if( y1 - h1/2 <= y2 + h2/2 && y1 - h1/2 >= y2 - h2/2 ){
			return true;
		}
	}
	return false;
}

void CreateFilename(char *filename, int num){
	filename[12]=filename[13]=filename[14]='0';

	if(num<10) filename[14]=num+'0';
	else if(num<100){
		filename[14]=num%10+'0';
		filename[13]=num/10+'0';
	}
	else{
		filename[14]=num%10+'0';
		filename[13]=(int)(num/10)%10+'0';
		filename[12]=num/100+'0';
	}
	return;
}
void CreateFilename2(char *filename, int num){
	filename[16]=filename[17]=filename[18]='0';

	if(num<10) filename[18]=num+'0';
	else if(num<100){
		filename[18]=num%10+'0';
		filename[17]=num/10+'0';
	}
	else{
		filename[18]=num%10+'0';
		filename[17]=(int)(num/10)%10+'0';
		filename[16]=num/100+'0';
	}
	return;
}

int pow2(int number){
	return number*number;
}
double absdouble(double number){
	if(number<0) return number*-1;
	return number;
}