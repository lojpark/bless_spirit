#pragma once
#include "_stdafx.h"

MakeMap cMakemap;

int listn;
struct Maplist{
	int x, y;
	char name[200];
}list[100];

void Menu(SDL_Renderer *screen, Font font, short select){
	Uint8 RGB[3] = {0};
	if(select==1) RGB[0] = 255;
	if(select==2) RGB[1] = 255;
	if(select==3) RGB[2] = 255;
	font.Print(90,35,"지형 만들기",RGB[0],RGB[0],RGB[0]);
	font.Print(90,85,"지형 지우기",RGB[1],RGB[1],RGB[1]);
	font.Print(90,135,"나가기",RGB[2],RGB[2],RGB[2]);
}

void MapCreateEffect(SDL_Renderer *screen, MapButton btn, int x, int y, Particle *ptc){
	int i;
	for(i=0;i<10;i++){
		(*ptc).Add( 0 , x , y , 1.0, (rand()%360)*PI/180 );
	}
}

void Save(){
	int i;
	FILE *out;
	fopen_s(&out,"data\\usermaplist.txt","w");
	fprintf_s(out,"%d\n",listn);
	for(i=0;i<listn;i++){
		fprintf_s(out,"%d %d\n",list[i].x,list[i].y);
		fprintf_s(out,"%s\n",list[i].name);
	}
	fclose(out);
}
void Load(){
	int i, j;
	FILE *in = NULL;
	fopen_s(&in,"data\\usermaplist.txt","r");
	if(in!=NULL){
		fscanf_s(in,"%d\n",&listn);
		for(i=0;i<listn;i++){
			fscanf_s(in,"%d %d\n",&list[i].x,&list[i].y);
			for(j=0;;j++){
				fscanf_s(in,"%c",&list[i].name[j]);
				if(list[i].name[j]=='\n'){
					list[i].name[j]=NULL;
					break;
				}
			}
		}
		fclose(in);
	}else{
		listn=0;
	}
}
void Update(int wh){
	int i;
	char filename[100] = {"data\\uermap\\map000.txt"};
	char new_filename[100] = {"data\\usermap\\map000.txt"};

	CreateFilename(filename,wh);

	// 파일 지우기
	remove(filename);

	// 파일 이름 당기기
	for(i=wh+1;i<listn;i++){
		CreateFilename(filename,i);
		CreateFilename(new_filename,i-1);
		rename(filename,new_filename);
	}

	// 리스트에서 파일 지우기
	listn--;
	for(i=wh;i<listn;i++){
		list[i] = list[i+1];
	}

	// 바뀐 리스트 저장
	Save();
}

int Mode_SelectCreateMap(SDL_Window *window, SDL_Renderer *screen, Cursor cCursor, FMOD_SYSTEM *g_System, Stage *stg, float *volume, float *volume_e)
{
	int i, j;
	bool quit = false;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();

	for(i=0;i<40;i++){
		list[i].x = 0;
		list[i].y = 0;
		strcpy_s(list[i].name,"");
	}

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

	Load();

	short select = 0;

	SDL_Texture *img_backcreate = load_image(screen,"image\\backcreate.png");

	cMakemap.window = window;
	cMakemap.screen = screen;

	quit = false;
	while( !quit ){
		curTicks += Set_Frame(curTicks);

		apply_surface(0,0,img_backcreate,screen,1.0,0,false,false);
		for(i=0;i<listn;i++){
			apply_surface(list[i].x, list[i].y, btn.img, screen, 1.0, 0.0, false, true, &btn.clip[4]);
			if(rand()%80+1==5) ptc.Add(1,list[i].x+5,list[i].y-10,0.2,rand()%360);
		}

		if(cCursor.type==0)	Menu(screen,font,select);
		// 맵 타겟팅
		if(cCursor.type==2){
		}

		cCursor.Print();

		ptc.Print(screen);

		// 맵 정보 보기
		for(i=0;i<listn;i++){
			if(isCrash(cCursor.info.x,cCursor.info.y,list[i].x,list[i].y,40,40)){
				font_c.PrintInput(cCursor.info.x,cCursor.info.y-30,list[i].name,0,0,0);
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
				// 지형 만들기
				if(isCrash(cCursor.info.x,cCursor.info.y,90,35,200,30)){
					if(select!=1) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).sound_e[4]);
					select = 1;
				}
				// 지형 지우기
				else if(isCrash(cCursor.info.x,cCursor.info.y,90,85,200,30)){
					if(select!=2) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).sound_e[4]);
					select = 2;
				}
				// 나가기
				else if(isCrash(cCursor.info.x,cCursor.info.y,90,135,90,30)){
					if(select!=3) Sound_Play(&g_System,&(*stg).channel_e,&(*stg).sound_e[4]);
					select = 3;
				}
				else select = 0;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ){
				// 타겟팅 중 오른쪽 클릭하면 취소
				if( event.button.button == SDL_BUTTON_RIGHT ){
					if(cCursor.type==2 || cCursor.type==80) cCursor.Update(0);
				}
				// 일반 커서
				if(cCursor.type==0){
					// 메뉴 선택
					if(select==1) cCursor.Update(2);
					if(select==2) cCursor.Update(80);
					if(select==3) break;
					
					// 맵 선택
					for(i=0;i<listn;i++){
						if(isCrash(cCursor.info.x,cCursor.info.y,list[i].x,list[i].y,40,40)){
							SDL_DestroyTexture(btn.img);SDL_DestroyTexture(img_backcreate);ptc.Destroy();font.Destroy();font_c.Destroy();
							return i;
						}
					}
				}
				// 맵 타겟팅
				else if(cCursor.type==2){
					// 겹치는지 판정
					j = 0;
					for(i=0;i<listn;i++){
						if(isCrash(cCursor.info.x,cCursor.info.y,list[i].x,list[i].y,40,40)){
							j=i;
							i=-1;
							break;
						}
					}
					// 겹친다
					if(i==-1){
						cMakemap.locationX = list[j].x;
						cMakemap.locationY = list[j].y;
						cMakemap.listn = j;
						strcpy_s(list[j].name,cMakemap.Main((*stg).channel,volume,volume_e,true));
						MapCreateEffect(screen,btn,cCursor.info.x,cCursor.info.y,&ptc);
					}
					// 아니면 만들어
					else{
						list[listn].x = cCursor.info.x;
						list[listn].y = cCursor.info.y;
						cMakemap.locationX = cCursor.info.x;
						cMakemap.locationY = cCursor.info.y;
						cMakemap.listn = listn;
						strcpy_s(list[listn].name,cMakemap.Main((*stg).channel,volume,volume_e));
						if(strlen(list[listn].name)!=0){
							listn++;
							MapCreateEffect(screen,btn,cCursor.info.x,cCursor.info.y,&ptc);
						}
					}
					// 초기화
					Save();
					select=0;
					cCursor.Update(0);
				}
				// 맵 삭제
				else if(cCursor.type==80){
					// 겹치는지 판정
					j = 0;
					for(i=0;i<listn;i++){
						if(isCrash(cCursor.info.x,cCursor.info.y,list[i].x,list[i].y,40,40)){
							j=i;
							i=-1;
							break;
						}
					}
					// 겹친다
					if(i==-1){
						MapCreateEffect(screen,btn,cCursor.info.x,cCursor.info.y,&ptc);
						Update(j);
					}
				}
			}
			if( event.type == SDL_KEYDOWN ){
				if(GetAsyncKeyState(VK_ESCAPE) < 0){
					if(cCursor.type==2 || cCursor.type==80) cCursor.Update(0);
					else quit = true;
				}
			}
			if( event.type == SDL_QUIT ){
				quit = true;
			}
		}
		
		SDL_RenderPresent(screen);
	}

	SDL_DestroyTexture(btn.img);
	SDL_DestroyTexture(img_backcreate);
	ptc.Destroy();
	font.Destroy();
	font_c.Destroy();
	return 486;
}