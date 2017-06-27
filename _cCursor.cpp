#pragma once
#include "_stdafx.h"

void Cursor::Set(SDL_Renderer *screen){
	this->screen = screen;
	this->img_cursorchips[0] = IMG_Load("image\\cursor\\cursor0.png");
	this->img_cursorchip[0] = SDL_CreateTextureFromSurface(screen,img_cursorchips[0]);
	this->img_cursorchips[1] = IMG_Load("image\\cursor\\cursor1.png");
	this->img_cursorchip[1] = SDL_CreateTextureFromSurface(screen,img_cursorchips[1]);
	this->img_cursorchips[2] = IMG_Load("image\\cursor\\cursor2.png");
	this->img_cursorchip[2] = SDL_CreateTextureFromSurface(screen,img_cursorchips[2]);

	// �� ���ۿ� Ŀ�� 
	int i;
	char temp[100] = {"image\\cursor\\cursor00.png"};// ��� ����
	// ���� ����
	for(i=11;i<=65;i++){
		temp[19]=i/10 + '0'; // ��ο� ���� �ο�
		temp[20]=i%10 + '0';
		this->img_cursorchips[i] = IMG_Load(temp);
		this->img_cursorchip[i] = SDL_CreateTextureFromSurface(screen,img_cursorchips[i]);
		if(i%10==5){ // 5��°������
			i+=5;
		}
	}
	// ������ ����
	for(i=1;i<=9;i++){
		temp[19]='7'; // ��ο� ���� �ο�
		temp[20]=i + '0';
		this->img_cursorchips[70+i] = IMG_Load(temp);
		this->img_cursorchip[70+i] = SDL_CreateTextureFromSurface(screen,img_cursorchips[70+i]);
	}
	this->img_cursorchips[80] = IMG_Load("image\\cursor\\cursor80.png");
	this->img_cursorchip[80] = SDL_CreateTextureFromSurface(screen,img_cursorchips[80]);

	// ���콺 ��ǥ �ʱ�ȭ
	this->info.x = -1;
	this->info.y = -1;
}

void Cursor::Update(int type){
	// Ŀ�� ����� ����
	img_cursor = SDL_CreateTextureFromSurface(screen,img_cursorchips[type]);
	img_cursors = img_cursorchips[type];
	this->type = type;

	// Ŀ�� ������ ������Ʈ
	SDL_QueryTexture(img_cursor,NULL,NULL,&info.w,&info.h);
	scale = 1.0;
}

void Cursor::Print(){
	if(info.x == -1 && info.y == -1){
		return;
	}
	apply_surface(info.x,info.y,img_cursor,screen,scale,0,false,true);
}

void Cursor::Destroy(){
	int i;
	SDL_DestroyRenderer(screen);
	for(i=0;i<100;i++){
		SDL_FreeSurface(img_cursorchips[i]);
		SDL_DestroyTexture(img_cursorchip[i]);
	}
	// ���� ����
	/*for(i=11;i<=65;i++){
		SDL_FreeSurface(img_cursorchips[i]);
		SDL_DestroyTexture(img_cursorchip[i]);
		if(i%10==5){ // 5��°������
			i+=5;
		}
	}*/
}