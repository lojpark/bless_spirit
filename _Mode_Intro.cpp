#pragma once
#include "_stdafx.h"
#define sink 100

void Mode_Intro(SDL_Renderer *screen, Cursor cCursor, FMOD_SYSTEM *g_System, Stage *stg)
{
	bool quit = false;
	int i, time=0;
	char filename[30]={"image\\intro\\intro0.png"};
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();

	SDL_Texture *img_intro[10];
	for(i=0;i<10;i++){
		filename[17] = i+'0';
		img_intro[i] = load_image(screen,filename);
	}
	int x[10]={   0, 240,  10, 0,   800,-460, 800, 170, 520, 0};
	double y[10]={0,  10, 200, 0,     0,  50, 250, 230, 515, 0};
	Uint8 alpha[10]={255,0,0,0,255,75,75,0,0,255};

	// 메인 화면
	time = 0;
	while( !quit ){
		time++;
		curTicks += Set_Frame(curTicks);

		if(time>=sink+200 && time<sink+455){
			alpha[1] ++;
		}
		if(time>=sink+600 && time<sink+855){
			alpha[1] --;
		}
		if(time>=sink+700 && time<sink+955){
			alpha[2] ++;
		}
		if(time>=sink+1100 && time<sink+1355){
			alpha[2] --;
		}
		if(time>=sink+1200 && time<sink+1455){
			alpha[3] ++;
			if(time==sink+1200) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).envt.sound[1]);
		}
		if(time>=sink+1700 && time<sink+1880){
			x[4] = 800 + 320*sin(PI + (double)(time-sink-1700)/120);
			if(time==sink+1700) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).envt.sound[0]);
		}
		if(time>=sink+2000 && time<sink+2180){
			x[5] = -460+460*sin((double)(time-sink-2000)/120);
			alpha[5] ++;
		}
		if(time>=sink+2100 && time<sink+2280){
			x[6] = 800-460*sin((double)(time-sink-2100)/120);
			alpha[6] ++;
		}
		if(time>=sink+2400 && time<sink+2655){
			y[7] = 230-60*sin((double)(time-sink-2400)/145);
			y[8] = 515-60*sin((double)(time-sink-2400)/145);
			alpha[7] ++;
			alpha[8] ++;
		}
		if(time>=sink+2655){
			FMOD_Channel_SetVolume((*stg).channel_e,(float)1.0-(float)(time-sink-2655)/200);
		}
		if(time>=sink+2855) break;

		for(i=0;i<9;i++){
			SDL_SetTextureAlphaMod(img_intro[i],alpha[i]);
			apply_surface(x[i],(int)y[i],img_intro[i],screen,1.0,0.0,false,false);
		}

		cCursor.Print();

		SDL_RenderPresent(screen);

		// 프로그램에 이벤트가 있을 때
		if( SDL_PollEvent( &event ) )
		{
			// 마우스 움직임 감지
			if( event.type == SDL_MOUSEMOTION){
				// 마우스 좌표 설정
				cCursor.info.x = event.motion.x;
				cCursor.info.y = event.motion.y;
			}
			if( event.type == SDL_KEYDOWN ){
				if(GetAsyncKeyState(VK_ESCAPE) < 0) {
					quit = true;
				}
			}
			if( event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	for(i=0;i<10;i++){
		SDL_DestroyTexture(img_intro[i]);
	}
	return;
}