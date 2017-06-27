#pragma once
#include "_stdafx.h"

void Menu::Set(int x, SDL_Renderer *screen){
	this->screen = screen;

	img = load_image(screen,"image\\menu.png");
	vol_bar = load_image(screen,"image\\vol_bar.png");
	vol_button = load_image(screen,"image\\vol_button.png");

	this->x = x;
	y = -346;
	select = 0;
	motion = 0.0;

	font1.Set(screen,"spring.ttf",30);
	font2.Set(screen,"spring.ttf",25);
}
void Menu::Open(){
	now = true;
	if(motion/50<PI/2) motion++;
	y = -346 + (int)(295*sin(motion/50));
}
void Menu::Close(){
	now = false;
	if(motion/50>0) motion--;
	y = -346 + (int)(295*sin(motion/50));
}
void Menu::Print(float volume, float volume_e){ // 스테이지의 메뉴 출력
	apply_surface(x-119,y,img,screen,1.0,0,false,false);
	Stage(volume,volume_e);
}
void Menu::Print(int bgnum, int bgtnum, float volume, float volume_e, bool possible){ // 맵 만들기의 메뉴 출력
	apply_surface(x-119,y,img,screen,1.0,0,false,false);
	Map(bgnum, bgtnum, volume, volume_e,possible);
}

void Menu::Stage(float volume, float volume_e){
	Uint8 RGB[5] = {0};
	RGB[select] = 255;

	font1.Print(400, y+51+35, "다시 하기",  RGB[1],RGB[1],RGB[1]);
	font1.Print(400, y+51+75, "다른거 할래",RGB[2],RGB[2],RGB[2]);
	font1.Print(400, y+51+115,"재미 없어",  RGB[3],RGB[3],RGB[3]);
	font1.Print(400, y+51+155,"전체 화면",  RGB[4],RGB[4],RGB[4]);

	font2.Print(400-70,y+51+205,"효과음",   0,0,0);
	font2.Print(400-70,y+51+245,"배경음",   0,0,0);

	apply_surface(435,y+51+205,vol_bar,screen,1.0,0.0,false,true);
	apply_surface((int)(395+volume_e*80),y+51+205,vol_button,screen,1.0,0.0,false,true);
	apply_surface(435,y+51+245,vol_bar,screen,1.0,0.0,false,true);
	apply_surface((int)(395+volume*80),y+51+245,vol_button,screen,1.0,0.0,false,true);
}
void Menu::Map(int bgnum, int bgtnum, float volume, float volume_e, bool possible){
	Uint8 RGB[2] = {0};
	if(select==5) RGB[0] = 255;
	if(select==6) RGB[1] = 255;
	if(possible==false) RGB[0] = 128;

	font1.Print(420, y+51+35, "저장",  RGB[0],RGB[0],RGB[0]);
	font1.Print(520, y+51+35, "나가기",  RGB[1],RGB[1],RGB[1]);

	apply_surface(470,y+51+90,minibg,screen,1.0,0.0,false,true,&clip_bg[bgnum]);
	if(select==1) apply_surface(430,y+51+90,arrowhite,screen,1.0,0.0,false,true);
	else apply_surface(430,y+51+90,arrow,screen,1.0,0.0,false,true);
	if(select==2) apply_surface(510,y+51+90,arrowhite,screen,1.0,0.0,true,true);
	else apply_surface(510,y+51+90,arrow,screen,1.0,0.0,true,true);

	apply_surface(470,y+51+150,minibgt,screen,1.0,0.0,false,true,&clip_bgt[bgtnum]);
	if(select==3) apply_surface(430,y+51+150,arrowhite,screen,1.0,0.0,false,true);
	else apply_surface(430,y+51+150,arrow,screen,1.0,0.0,false,true);
	if(select==4) apply_surface(510,y+51+150,arrowhite,screen,1.0,0.0,true,true);
	else apply_surface(510,y+51+150,arrow,screen,1.0,0.0,true,true);

	font2.Print(470-70,y+51+205,"효과음",   0,0,0);
	font2.Print(470-70,y+51+245,"배경음",   0,0,0);
	apply_surface(505,y+51+205,vol_bar,screen,1.0,0.0,false,true);
	apply_surface((int)(465+volume_e*80),y+51+205,vol_button,screen,1.0,0.0,false,true);
	apply_surface(505,y+51+245,vol_bar,screen,1.0,0.0,false,true);
	apply_surface((int)(465+volume*80),y+51+245,vol_button,screen,1.0,0.0,false,true);
}
bool Menu::isOverMenu(int cx, int cy){
	if(cy < y+346+32 && cx>=x-119 && cx<=x+119){
		return true;
	}
	return false;
}
void Menu::Destroy(){
	SDL_DestroyTexture(img);
	SDL_DestroyTexture(vol_bar);
	SDL_DestroyTexture(vol_button);

	font1.Destroy();
	font2.Destroy();
}