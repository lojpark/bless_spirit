#pragma once
#include "_stdafx.h"

int listnN = 0;
struct Maplist{
	int x, y, ai;
	char name[200];
}listN[40] = {0,};

void MenuN(SDL_Renderer *screen, Font font, short select){
	Uint8 RGB[3] = {0};
	if(select==1) RGB[0] = 255;
	font.Print(70,45,"나가기",RGB[0],RGB[0],RGB[0]);
}

void LoadN(MapInfo *info, bool first){
	int i, j;
	FILE *in = NULL;
	fopen_s(&in,"data\\maplist.txt","r");
	if(in!=NULL){
		fscanf_s(in,"%d\n",&listnN);
		for(i=0;i<listnN;i++){
			fscanf_s(in,"%d%d%d\n",&listN[i].x,&listN[i].y,&listN[i].ai);
			for(j=0;;j++){
				fscanf_s(in,"%c",&listN[i].name[j]);
				if(listN[i].name[j]=='\n'){
					listN[i].name[j]=NULL;
					break;
				}
			}
		}
		fclose(in);
	}else{
		listnN = 0;
	}

	if(!first){
		in = NULL;
		fopen_s(&in,"data\\mapinfo.txt","r");
		if(in!=NULL){
			fscanf_s(in,"%d\n",&(*info).n);
			for(i=0;i<(*info).n;i++){
				fscanf_s(in,"%d ",&(*info).trophy[i]);
			}
			fclose(in);
		}else{
			(*info).n=0;
			(*info).trophy[0]=0;
		}
	}
}

int Mode_SelectMap(SDL_Window *window, SDL_Renderer *screen, Cursor cCursor, MapInfo *info, FMOD_SYSTEM *g_System, Stage *stg, float *volume, float *volume_e)
{
	int i;
	bool tEnding = false;
	bool quit = false;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();

	for(i=0;i<40;i++){
		listN[i].x = 0;
		listN[i].y = 0;
		listN[i].ai = 0;
		strcpy_s(listN[i].name,"");
	}
	LoadN(info,true);

	Font font, font_c;
	font.Set(screen,"spring.ttf",30);
	font_c.Set(screen,"spring.ttf",20);

	MapButton btn;
	btn.img = load_image(screen,"image\\mapbutton.png");
	for(i=0;i<5;i++){
		btn.clip[i].x = i*40;
		btn.clip[i].y = 0;
		btn.clip[i].w = 40;
		btn.clip[i].h = 40;
	}

	Particle ptc;
	ptc.Set(screen);
	SDL_SetTextureColorMod(ptc.img,255,127,39);
	SDL_SetTextureBlendMode(ptc.img,SDL_BLENDMODE_ADD);
	Particle ptc_s;
	ptc_s.Set(screen);
	SDL_SetTextureColorMod(ptc_s.img,128,128,128);
	SDL_SetTextureBlendMode(ptc_s.img,SDL_BLENDMODE_ADD);

	if((*info).n==0) LoadN(info,false);

	short select = 0;

	SDL_Texture *img_backmap = load_image(screen,"image\\backmap.png");
	SDL_Texture *img_fire = load_image(screen,"image\\cursor\\fire.png");
	SDL_Texture *img_water = load_image(screen,"image\\cursor\\water.png");

	quit = false;
	while( !quit ){
		curTicks += Set_Frame(curTicks);

		apply_surface(0,0,img_backmap,screen,1.0,0,false,false);
		tEnding = true;
		for(i=0;i<=(*info).n;i++){
			if(i==33) break;
			if(i==32){
				if(!tEnding) break;
			}
			if(i<32 && (*info).trophy[i]!=3){
				tEnding = false;
			}
			apply_surface(listN[i].x, listN[i].y, btn.img, screen, 1.0, 0.0, false, true, &btn.clip[(*info).trophy[i]]);
			if((*info).trophy[i]==3){
				if(rand()%80+1==5) ptc.Add(1,listN[i].x+5,listN[i].y-10,0.3,rand()%360);
			}
			else if((*info).trophy[i]==2){
				if(rand()%160+1==5) ptc_s.Add(1,listN[i].x+5,listN[i].y-10,0.3,rand()%360);
			}
		}

		if(cCursor.type==0)	MenuN(screen,font,select);

		cCursor.Print();

		ptc.Print(screen);
		ptc_s.Print(screen);

		// 맵 정보 보기
		for(i=0;i<listnN;i++){
			if(i>(*info).n){
				break;
			}
			if(isCrash(cCursor.info.x,cCursor.info.y,listN[i].x,listN[i].y,40,40)){
				font_c.PrintInput(cCursor.info.x,cCursor.info.y-30,listN[i].name,0,0,0);
				if(listN[i].ai==1 || listN[i].ai==3) apply_surface(cCursor.info.x-20,cCursor.info.y-10,img_fire,screen,1.0,0.0,false,true);
				if(listN[i].ai==2 || listN[i].ai==3) apply_surface(cCursor.info.x+20,cCursor.info.y-10,img_water,screen,1.0,0.0,false,true);
			}
		}

		// 프로그램에 이벤트가 있을 때
		if( SDL_PollEvent( &event ) )
		{
			// 마우스 움직임 감지
			if( event.type == SDL_MOUSEMOTION){
				// 마우스 좌표 설정
				cCursor.info.x = event.motion.x;
				cCursor.info.y = event.motion.y;
				// 나가기
				if(isCrash(cCursor.info.x,cCursor.info.y,70,45,90,30)){
					if(select!=1) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).sound_e[4]);
					select = 1;
				}
				else select = 0;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ){
				// 메뉴 선택
				if(cCursor.type==0){
					if(select==1) break;
				}
				// 맵 선택
				for(i=0;i<listnN;i++){
					if(!tEnding && i==32){
						break;
					}
					if(i>(*info).n){
						break;
					}
					if(isCrash(cCursor.info.x,cCursor.info.y,listN[i].x,listN[i].y,40,40)){
						SDL_DestroyTexture(btn.img);SDL_DestroyTexture(img_backmap);ptc.Destroy();ptc_s.Destroy();font.Destroy();font_c.Destroy();
						return i;
					}
				}
			}
			if( event.type == SDL_KEYDOWN ){
				if(GetAsyncKeyState(VK_ESCAPE) < 0){
					quit = true;
				}
			}
			if( event.type == SDL_QUIT ){
				quit = true;
			}
		}
		
		SDL_RenderPresent(screen);
	}

	SDL_DestroyTexture(btn.img);SDL_DestroyTexture(img_backmap);ptc.Destroy();ptc_s.Destroy();font.Destroy();font_c.Destroy();
	return 486;
}