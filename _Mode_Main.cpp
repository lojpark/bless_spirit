#pragma once
#include "_stdafx.h"

int Mode_Main(SDL_Window *window, SDL_Renderer *screen, FMOD_SYSTEM *g_System, FMOD_CHANNEL *channel, FMOD_SOUND *sound, BackGroundImage bgi, Cursor cCursor, int n)
{
	int i;
	bool quit = false;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();

	// 배경 이미지 초기화
	bgi.type = 1;
	bgi.alpha = 0;

	SDL_Texture *img_npb = load_image(screen,"image\\npb.png");
	SDL_SetTextureBlendMode(img_npb,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img_npb,128);

	Font font;
	font.Set(screen,"spring.ttf",30);
	Font font_t;
	font_t.Set(screen,"spring.ttf",60);
	int y[10]={0}, time=0;

	SDL_Texture *img_btn[3];
	double btn_scale[3]={1.0,1.0,1.0};
	double btn_motion[3]={0,0,0};
	img_btn[0] = load_image(screen,"image\\button1.png");
	img_btn[1] = load_image(screen,"image\\button2.png");
	img_btn[2] = load_image(screen,"image\\button3.png");

	int limit = 1;
	if(n<=4){
		limit=1;
	}else if(n<=8){
		limit=2;
	}else if(n<=13){
		limit=3;
	}else if(n<=15){
		limit=4;
	}else if(n<=19){
		limit=5;
	}else if(n<=25){
		limit=6;
	}else{
		limit=7;
	}

	// 메인 화면
	while( !quit ){
		curTicks += Set_Frame(curTicks);

		if(isCrash(cCursor.info.x,cCursor.info.y,200,500,400,300)){
			if(btn_motion[0]==0) Sound_Play(&g_System,&channel,&sound);
			if(btn_motion[0]<20) btn_motion[0]++;
			btn_scale[0] = 1.0 + 0.2*sin(btn_motion[0]/30);
		}
		else{
			if(btn_motion[0]>0) btn_motion[0]--;
			btn_scale[0] = 1.0 + 0.2*sin(btn_motion[0]/30);
		}
		if(isCrash(cCursor.info.x,cCursor.info.y,600,500,400,300)){
			if(btn_motion[1]==0) Sound_Play(&g_System,&channel,&sound);
			if(btn_motion[1]<20) btn_motion[1]++;
			btn_scale[1] = 1.0 + 0.2*sin(btn_motion[1]/30);
		}
		else{
			if(btn_motion[1]>0) btn_motion[1]--;
			btn_scale[1] = 1.0 + 0.2*sin(btn_motion[1]/30);
		}
		
		bgi.Print_Back(screen,false);
		apply_surface(200,500,img_btn[0],screen,btn_scale[0],0.0,false,true);
		apply_surface(600,500,img_btn[1],screen,btn_scale[1],0.0,false,true);
		if(isCrash(cCursor.info.x,cCursor.info.y,200,500,400,300)) font.Print(cCursor.info.x,cCursor.info.y-30,"이야기 방식",0,0,0);
		if(isCrash(cCursor.info.x,cCursor.info.y,600,500,400,300)) font.Print(cCursor.info.x,cCursor.info.y-30,"사용자 방식",0,0,0);

		time++;
		font_t.Print(130,y[0],"길",0,0,0);
		font_t.Print(210,y[1],"잃",0,0,0);
		font_t.Print(260,y[2],"은",0,0,0);
		font_t.Print(340,y[3],"정",0,0,0);
		font_t.Print(390,y[4],"령",0,0,0);
		font_t.Print(440,y[5],"에",0,0,0);
		font_t.Print(490,y[6],"게",0,0,0);
		font_t.Print(570,y[7],"축",0,0,0);
		font_t.Print(620,y[8],"복",0,0,0);
		font_t.Print(670,y[9],"을",0,0,0);
		for(i=0;i<10;i++){
			y[i] = 150 + 20*sin((double)(time+i*20)/60);
		}

		if(cCursor.info.x<100 && bgi.type>1) apply_surface(0,250,img_npb,screen,1.0,0,false,false);
		if(cCursor.info.x>700 && bgi.type<limit) apply_surface(764,250,img_npb,screen,1.0,0,true,false);
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
			if( event.type == SDL_MOUSEBUTTONDOWN){
				if(isCrash(cCursor.info.x,cCursor.info.y,200,500,400,300)){
					SDL_DestroyTexture(img_npb);SDL_DestroyTexture(img_btn[0]);SDL_DestroyTexture(img_btn[1]);SDL_DestroyTexture(img_btn[2]);font.Destroy();
					return 1;
				}
				if(isCrash(cCursor.info.x,cCursor.info.y,600,500,400,300)){
					SDL_DestroyTexture(img_npb);SDL_DestroyTexture(img_btn[0]);SDL_DestroyTexture(img_btn[1]);SDL_DestroyTexture(img_btn[2]);font.Destroy();
					return 2;
				}
				if(event.motion.x < 100 && bgi.type>1){
					bgi.alpha=0;
					bgi.ltype=bgi.type;
					bgi.type--;
				}
				if(event.motion.x > 700 && bgi.type<limit){
					bgi.alpha=0;
					bgi.ltype=bgi.type;
					bgi.type++;
				}
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

	SDL_DestroyTexture(img_npb);
	SDL_DestroyTexture(img_btn[0]);SDL_DestroyTexture(img_btn[1]);SDL_DestroyTexture(img_btn[2]);
	font.Destroy();
	return 0;
}