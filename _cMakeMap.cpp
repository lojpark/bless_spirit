#pragma once
#include "_stdafx.h"
#include <locale.h>

void MakeMap::Undo()
{
	int i=0, j=0;
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(map[i][j]==true)
				*((Uint32 *)img_maps->pixels + i*img_maps->pitch/4 + j) = SDL_MapRGB(img_maps->format, 0, 0, 0);
			else
				*((Uint32 *)img_maps->pixels + i*img_maps->pitch/4 + j) = NULL;
		}
	}
	return;
}

void MakeMap::Draw(int x, int y)
{
	if(x<0 || x>WIDTH || y<0 || y>HEIGHT) return;
	// 쓰기 상태라면
	if(woe == true){
		// 만들 맵 서피스에 까만 점 찍기
		*((Uint32 *)img_maps->pixels + y*img_maps->pitch/4 + x) = SDL_MapRGB(img_maps->format, 0, 0, 0);
		// 맵 정보 수정
		map[y][x]=true;
	}
	// 지우기 상태라면
	else{
		// 만들 맵 서피스 지우기
		*((Uint32 *)img_maps->pixels + y*img_maps->pitch/4 + x) = NULL;
		// 맵 정보 수정
		map[y][x]=false;
	}
	return;
}
void MakeMap::DrawShape(int x, int y)
{
	// 만약 점을 찍는 상태라면
	if(cCursor.type==11){
		// 점 찍고 리턴
		Draw(x,y);
		return;
	}
	int i, j, i2, j2;
	// 커서 크기대로 쭉 보면서
	i2=0;
	for(i=-cCursor.info.h/2;i<cCursor.info.h/2;i++){
		j2=0;
		for(j=-cCursor.info.w/2;j<cCursor.info.w/2;j++){
			// 검은 부분이면
			if(SDL_MapRGB(cCursor.img_cursors->format, 0, 0, 0) == *((Uint32 *)cCursor.img_cursors->pixels + i2*cCursor.img_cursors->pitch/4 + j2) ){
				//점 찍기
				Draw(x+j,y+i);
			}
			j2++;
		}
		i2++;
	}
	return;
}

bool MakeMap::Save(double fscale, double wscale, SDL_Texture *img_menu){
	int i, j;
	bool quit = false, cancel = false;
	double scale = 0.0, motion = 0.0;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();
	SDL_Texture *img_line = load_image(screen,"image\\line.png");
	Uint8 RGB[2]={0};
	Font font_t, font_d;
	font_t.Set(screen,"spring.ttf",25);
	font_d.Set(screen,"NanumGothicCoding.ttf",16);
	char composition[32]={""};
	bool focus = false;

	while( !quit ){
		curTicks += Set_Frame(curTicks);

		Print(fscale,wscale);
		// 취소를 눌렀다면
		if(cancel){
			motion--;
			scale = sin(motion/30);
			apply_surface(470,300,img_menu,screen,scale,90,false,true);
			if(motion<=0){
				SDL_DestroyTexture(img_line);
				return false;
			}
		}
		// 여는 중
		else if(motion/30 < PI/2){
			motion++;
			scale = sin(motion/30);
			apply_surface(470,300,img_menu,screen,scale,90,false,true);
		}
		// 연 후
		else{
			RGB[0]=RGB[1]=0;
			// 저장
			if(isCrash(cCursor.info.x,cCursor.info.y,530,215,50,30)) RGB[0]=255;
			// 취소
			if(isCrash(cCursor.info.x,cCursor.info.y,585,215,50,30)) RGB[1]=255;
			// 글 커서
			if(isCrash(cCursor.info.x,cCursor.info.y,400,240,272,30) || isCrash(cCursor.info.x,cCursor.info.y,400,350,272,90)){
				cCursor.Update(1);
			}else cCursor.Update(0);

			// 글 없이 저장 불가
			if(strlen(text_title)==0 || strlen(text_desc[0])==0){
				RGB[0]=128;
			}

			apply_surface(470,300,img_menu,screen,scale,90,false,true);
			font_t.Print(530,215,"저장",RGB[0],RGB[0],RGB[0]);
			font_t.Print(585,215,"취소",RGB[1],RGB[1],RGB[1]);

			font_t.Print(360,215,"제목",0,0,0);
			font_d.PrintInput(340,240,text_title,0,0,0,false);
			apply_surface(470,250,img_line,screen,1.0,0.0,false,true);

			font_t.Print(360,290,"설명",0,0,0);
			for(i=0;i<3;i++){
				font_d.PrintInput(340,315+i*25,text_desc[i],0,0,0,false);
				apply_surface(470,325+i*25,img_line,screen,1.0,0.0,false,true);
			}
		}
		cCursor.Print();
		printf("%d\n",len_title);
		if(!focus) font_d.PrintInput(cursor_title,240,composition,0,0,0,false);
		else font_d.PrintInput(cursor_desc[line],315+line*25,composition,0,0,0,false);

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
			if( event.type == SDL_MOUSEBUTTONDOWN && !cancel){
				// 저장
				if(RGB[0]==255) break;
				// 취소
				if(RGB[1]==255) cancel=true;

				// 커서 포커스 이동
				if(isCrash(cCursor.info.x,cCursor.info.y,470,240,272,30) && focus){
					strcat_s(text_desc[line], composition);
					strcpy_s(composition,"");
					focus = false;
				}
				if(isCrash(cCursor.info.x,cCursor.info.y,470,350,272,90) && !focus){
					strcat_s(text_title, composition);
					strcpy_s(composition,"");
					focus = true;
				}
			}
			if(event.type==SDL_TEXTINPUT){
				if(!focus){
					strcat_s(text_title, event.text.text);
					len_title++;
					cursor_title+=8;
					if(strlen(event.text.text)>1){
						len_title+=2;
						cursor_title+=8;
					}
				}
				else{
					strcat_s(text_desc[line], event.text.text);
					len_desc[line]++;
					cursor_desc[line]+=8;
					if(strlen(event.text.text)>1){
						len_desc[line]+=2;
						cursor_desc[line]+=8;
					}
					if(cursor_desc[line]>=580) line++;
				}
			} 
			if(event.type==SDL_TEXTEDITING){
				strcpy_s(composition,event.edit.text);
			}
			if(event.type==SDL_KEYDOWN){
				if(GetAsyncKeyState(VK_BACK) < 0){
					if(!focus){
						if(text_title[len_title-1]<0){
							if(strlen(composition)>0) strcpy_s(composition,"");
							else{
								if(len_title>=3){
									text_title[len_title-1]=text_title[len_title-2]=text_title[len_title-3]=NULL;
									len_title-=3;
									cursor_title-=16;
								}
							}
						}else{
							if(strlen(composition)>0) strcpy_s(composition,"");
							else{
								if(len_title>=1){
									text_title[len_title-1]=NULL;
									len_title--;
									cursor_title-=8;
								}
							}
						}
					}else{
						if(text_desc[line][len_desc[line]-1]<0){
							if(strlen(composition)>0) strcpy_s(composition,"");
							else{
								if(len_desc[line]>=3){
									text_desc[line][len_desc[line]-1]=text_desc[line][len_desc[line]-2]=text_desc[line][len_desc[line]-3]=NULL;
									len_desc[line]-=3;
									cursor_desc[line]-=16;
								}else if(line>0){
									line--;
									text_desc[line][len_desc[line]-1]=text_desc[line][len_desc[line]-2]=text_desc[line][len_desc[line]-3]=NULL;
									len_desc[line]-=3;
									cursor_desc[line]-=16;
								}
							}
						}else{
							if(strlen(composition)>0) strcpy_s(composition,"");
							else{
								if(len_desc[line]>=1){
									text_desc[line][len_desc[line]-1]=NULL;
									len_desc[line]--;
									cursor_desc[line]-=8;
								}else if(line>0){
									line--;
									text_desc[line][len_desc[line]-1]=NULL;
									len_desc[line]--;
									cursor_desc[line]-=8;
								}
							}
						}
					}
				}
			}
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}

	// 데이터 저장
	char filename[100] = {"data\\usermap\\map000.txt"};
	CreateFilename2(filename,listn);

	fopen_s(&out,filename,"w");

	fprintf_s(out,"%d\n",envt.n);
	for(i=1;i<=envt.n;i++){
		fprintf_s(out,"%d %d %d\n",envt.e[i].type,envt.e[i].x,envt.e[i].y);
	}
	fprintf_s(out,"%lf %lf\n",fscale,wscale);
	fprintf_s(out,"%d\n",len_title);
	for(i=0;i<3;i++) fprintf_s(out,"%d ",len_desc[i]);
	fprintf_s(out,"\n%d %d\n",line,cursor_title);
	for(i=0;i<3;i++) fprintf_s(out,"%d ",cursor_desc[i]);
	fprintf_s(out,"\n%s\n%s\n",text_title,text_desc[0]);
	if(line>0) fprintf_s(out,"%s\n",text_desc[1]);
	if(line>1) fprintf_s(out,"%s\n",text_desc[2]);
	fprintf_s(out,"%d %d\n",bgnum,bgtnum);

	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			fprintf_s(out,"%d ",map[i][j]);
		}
		fprintf_s(out,"\n");
	}

	fclose(out);
	
	SDL_DestroyTexture(img_line);
	return true;
}
void MakeMap::Load(double *fscale, double *wscale){
	int i, j;
	char filename[100] = {"data\\usermap\\map000.txt"};
	CreateFilename2(filename,listn);

	fopen_s(&in,filename,"r");

	fscanf_s(in,"%d\n",&envt.n);
	for(i=1;i<=envt.n;i++){
		fscanf_s(in,"%d %d %d\n",&envt.e[i].type,&envt.e[i].x,&envt.e[i].y);
	}
	fscanf_s(in,"%lf %lf\n",fscale,wscale);
	fscanf_s(in,"%d\n",&len_title);
	for(i=0;i<3;i++) fscanf_s(in,"%d ",&len_desc[i]);
	fscanf_s(in,"\n%d %d\n",&line,&cursor_title);
	for(i=0;i<3;i++) fscanf_s(in,"%d ",&cursor_desc[i]);

	for(i=0;;i++){
		fscanf_s(in,"%c",&text_title[i]);
		if(text_title[i]=='\n'){
			text_title[i]=NULL;
			break;
		}
	}
	for(i=0;;i++){
		fscanf_s(in,"%c",&text_desc[0][i]);
		if(text_desc[0][i]=='\n'){
			text_desc[0][i]=NULL;
			break;
		}
	}
	if(line>0){
		for(i=0;;i++){
			fscanf_s(in,"%c",&text_desc[1][i]);
			if(text_desc[1][i]=='\n'){
				text_desc[1][i]=NULL;
				break;
			}
		}
	}
	if(line>1){
		for(i=0;;i++){
			fscanf_s(in,"%c",&text_desc[2][i]);
			if(text_desc[2][i]=='\n'){
				text_desc[2][i]=NULL;
				break;
			}
		}
	}
	fscanf_s(in,"%d %d\n",&bgnum,&bgtnum);

	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			fscanf_s(in,"%d",&map[i][j]);
		}
		fscanf_s(in,"\n");
	}
	
	fclose(in);
	// 맵 서피스 업데이트
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if(map[i][j] == 0){
				*((Uint32 *)img_maps->pixels + i*img_maps->pitch/4 + j) = NULL;
			}
			else{
				*((Uint32 *)img_maps->pixels + i*img_maps->pitch/4 + j) = SDL_MapRGB(img_maps->format, 0, 0, 0);
			}
		}
	}

	// 사용자에게 보여줄 맵 업데이트
	img_map = SDL_CreateTextureFromSurface(screen,img_maps);
}

void MakeMap::SetTool(){

	int w = WIDTH;
	double seta = 0.0;
	SDL_Event event;
	bool quit = false;

	Uint32 curTicks = SDL_GetTicks();

	while(!quit){
		curTicks += Set_Frame(curTicks);

		w = (int)( WIDTH + 140 * sin(seta) );
		if(w%2==1) w++;
		seta+=0.03;
		if(seta>=3.14/2) break;
		SDL_SetWindowSize(window,w,HEIGHT);
		SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);

		apply_surface(0,0,img_tool,screen,1.0,0,false,false);
		apply_surface(w-70,0,img_tool,screen,1.0,0,false,false);
		apply_surface((w-WIDTH)/2,0,img_load,screen,1.0,0,false,false);
		SDL_RenderPresent(screen);

		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}
	SDL_SetWindowSize(window,WIDTH+140,HEIGHT);
	apply_surface(0,0,img_tool,screen,1.0,0,false,false);
	apply_surface(w-70,0,img_tool,screen,1.0,0,false,false);
	apply_surface((w-WIDTH)/2,0,img_load,screen,1.0,0,false,false);
	SDL_RenderPresent(screen);
}
void MakeMap::CloseTool(){
	SDL_Event event;
	bool quit = false;

	int w = WIDTH+140;
	double seta = 3.14 / 2;

	Uint32 curTicks = SDL_GetTicks();

	while(1){
		curTicks += Set_Frame(curTicks);

		w = (int)( WIDTH + 140 * sin(seta) );
		if(w%2==1) w++;
		seta+=0.03;
		if(seta>=3.14) break;
		SDL_SetWindowSize(window,w,HEIGHT);
		SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
		apply_surface(0,0,img_tool,screen,1.0,0,false,false);
		apply_surface(w-70,0,img_tool,screen,1.0,0,false,false);
		apply_surface((w-WIDTH)/2,0,img_load,screen,1.0,0,false,false);
		SDL_RenderPresent(screen);

		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}
	SDL_SetWindowSize(window,WIDTH,HEIGHT);
	apply_surface(0,0,img_tool,screen,1.0,0,false,false);
	apply_surface(w-70,0,img_tool,screen,1.0,0,false,false);
	apply_surface((w-WIDTH)/2,0,img_load,screen,1.0,0,false,false);
	SDL_RenderPresent(screen);
}

void MakeMap::Print(double fscale, double wscale){
	int i=0, j=0;
	
	// 배경 출력
	if(soh) apply_surface(70, 0, img_bgi[bgnum], screen, 1.0, 0, false, false);
	else apply_surface(70, 0, img_bgi[0], screen, 1.0, 0, false, false);

	// 환경 출력
	j=0;
	if(cCursor.type==80){ // 삭제 직전의 환경을 추려낸다
		for(i=1;i<=envt.n;i++){
			// 하나만 지워
			if(j==1){
				envt.e[i].erase = false;
				break;
			}
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70)){
				envt.e[i].erase = true;
				j=1;
			}
			else envt.e[i].erase = false;
		}
	}
	envt.Print(screen,false);

	// 포인트 출력
	for(i=1;i<=envt.n;i++){
		if(envt.e[i].type==7){
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[7],255,128,128);
			apply_surface(envt.e[i].x, envt.e[i].y, envt.img[7], screen, fscale, 0, false, true);
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[7],255,255,255);
		}
		if(envt.e[i].type==8){
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[8],255,128,128);
			apply_surface(envt.e[i].x, envt.e[i].y, envt.img[8], screen, wscale, 0, false, true);
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[8],255,255,255);
		}
		if(envt.e[i].type==9){
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[9],255,128,128);
			apply_surface(envt.e[i].x, envt.e[i].y, envt.img[9], screen, 1.0, 0, false, true);
			if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,70,70) && cCursor.type==80) SDL_SetTextureColorMod(envt.img[9],255,255,255);
		}
	}

	// 맵 출력
	apply_surface(70, 0, img_map, screen, 1.0, 0, false, false);

	SDL_Rect clip;
	clip.x = clip.y = 0;
	clip.h = clip.w = 70;

	// 왼쪽 툴바 크기바 출력
	for(i=0;i<6;i++){
		// 배경
		apply_surface( toolLX[i] + 175 , i*70+35, img_toolSize, screen,1.0,0,false,true);
		for(j=0;j<5;j++){
			// 커서 모양
			apply_surface( toolLX[i] + (j*70) + 35 , i*70+35,
			cCursor.img_cursorchip[ (i+1)*10 + j + 1 ],
			screen,1.0,0,false,true);
		}
	}
	// 왼쪽 툴바 배경 출력
	apply_surface(0,0,img_tool,screen,1.0,0,false,false);
	for(i=0;i<6;i++){ // 왼쪽 툴바 이미지 출력
		// 가장자리
		apply_surface(0,i*70,img_tbdr,screen,1.0,0,false,false);
		apply_surface(35,i*70+35,
			cCursor.img_cursorchip[ (i+1)*10 + toolL[i] ],
			screen,1.0,0,false,true);
	}
	// 가장자리
	apply_surface(0,6*70,img_tbdr,screen,1.0,0,false,false);
	apply_surface(0,7*70,img_tbdr,screen,1.0,0,false,false);
	// 배경 보이기 / 가리기 출력
	if(soh == true) clip.x=0;
	else clip.x=70;
	apply_surface(0,6*70,img_soh,screen,1.0,0,false,false,&clip);
	// 연필 / 지우개 출력
	if(woe == true) clip.x=0;
	else clip.x=70;
	apply_surface(0,7*70,img_woe,screen,1.0,0,false,false,&clip);

	// 오른쪽 툴바 배경 출력
	clip.h = 60;
	apply_surface(WIDTH+70,0,img_tool,screen,1.0,0,false,false);
	for(i=0;i<10;i++){ // 오른쪽 툴바 환경 이미지 출력
		// 가장자리
		apply_surface(WIDTH+70,i*60,img_tbdr2,screen,1.0,0,false,false);
		clip.x = i*70;
		apply_surface(WIDTH+70,i*60,img_toolR,screen,1.0,0,false,false,&clip);
	}
}

void MakeMap::ClearEvent(SDL_Event* event){
	while(1){
		if( SDL_PollEvent( event ) )
		{
			// 마우스 움직임 감지
			if( event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION ){
				// 마우스 좌표 설정
				cCursor.info.x = event->motion.x;
				cCursor.info.y = event->motion.y;
			}
		}
		else break;
	}
}

void MakeMap::SetToolSize(){
	int i=0;
	for(i=0;i<6;i++){
		// 나오는 상태
		if(toolLE[i] == true){
			if( toolLT[i]/50 >= PI/2 ){ // 다 나왔으면
				toolLX[i] = 70;
			}
			else{ // 나오는 중이면
				toolLX[i] = -280 + (int)( 350*sin(toolLT[i]/50) );
				toolLT[i]++;
			}
		}
		// 들어가는 상태
		else{
			if(toolLT[i]/50 <= 0){ // 다 들어갔으면
				toolLX[i] = -280;
			}
			else{ // 들어가는 중이면
				toolLX[i] = -280 + (int)( 350*sin(toolLT[i]/50) );
				toolLT[i]--;
			}
		}
	}
}

void MakeMap::Stack::Push(bool map[][WIDTH+10]){
	int i, j;

	// 스택이 꽉찼다면 가장 이전 것을 지우자
	if(this->now == 9) Erase(map);

	this->now++;
	this->top = this->now;
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			this->smap[this->now][i][j] = map[i][j];
		}
	}
}
void MakeMap::Stack::Pop(bool map[][WIDTH+10]){
	int i, j;
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			map[i][j] = this->smap[this->now][i][j];
		}
	}
}
void MakeMap::Stack::Erase(bool map[][WIDTH+10]){
	int k, i, j;
	for(k=0;k<9;k++){
		for(i=0;i<HEIGHT;i++){
			for(j=0;j<WIDTH;j++){
				this->smap[k][i][j] = this->smap[k+1][i][j];
			}
		}
	}
	this->now--;
}

char* MakeMap::Main(FMOD_CHANNEL *channel, float *volume, float *volume_e, bool isLoad)
{
	SDL_Event event;
	bool quit = false, chk = false;
	bool key_stateZ = false, key_stateY = false;
	bool fsp = false, wsp = false; // 불, 물 시작 포인트
	bool ep = false; // 끝 포인트
	double fscale = 1.0, wscale = 1.0; // 불, 물 시작 크기
	double fsc = 1.0, wsc = 1.0; // 불, 물 시작 크기 변경
	int i=0;
	int x=-1, y=-1;

	Set();

	// 메뉴(上) 초기화
	Menu menu;
	menu.Set(470,screen);
	menu.minibg = load_image(screen,"image\\minibg.png"); // 배경 설정용
	menu.minibgt = load_image(screen,"image\\minibgt.png"); // 지형 텍스쳐 설정용
	for(i=1;i<=7;i++){
		menu.clip_bg[i].x = (i-1)*48;
		menu.clip_bg[i].y = 0;
		menu.clip_bg[i].w = 48;
		menu.clip_bg[i].h = 48;
	}
	for(i=1;i<=4;i++){
		menu.clip_bgt[i].x = (i-1)*48;
		menu.clip_bgt[i].y = 0;
		menu.clip_bgt[i].w = 48;
		menu.clip_bgt[i].h = 48;
	}
	menu.arrow = load_image(screen,"image\\arrow.png");
	menu.arrowhite = load_image(screen,"image\\arrowhite.png");

	// 툴 불러오기
	SetTool();
	ClearEvent(&event);

	// 불러왔어?
	if(isLoad){
		Load(&fscale,&wscale);
		fsc = fscale;
		wsc = wscale;
		fsp = wsp = ep = false;
		for(i=1;i<=envt.n;i++){
			if(envt.e[i].type==7) fsp=true;
			if(envt.e[i].type==8) wsp=true;
			if(envt.e[i].type==9) ep=true;
		}
		stack.Push(map);
	}
	
	// 배경음 소리 조절
	FMOD_Channel_SetVolume(channel,(*volume));

	Uint32 curTicks = SDL_GetTicks();
	
	while(!quit){
		curTicks += Set_Frame(curTicks);

		// 툴바 나오고 들어가고
		SetToolSize();

		Print(fscale,wscale);
		menu.Print(bgnum,bgtnum,(*volume),(*volume_e),ep);
		cCursor.Print();

		printf("\n%d %d\n",stack.now, stack.top);

		// 메뉴에 마우스가 가면
		if( menu.isOverMenu(cCursor.info.x,cCursor.info.y) ){
			menu.Open();
			// 메뉴 선택
			menu.select = 0;
			if( isCrash(cCursor.info.x,cCursor.info.y,420,menu.y+51+35,60,30) ) menu.select = 5; // 저장
			if( isCrash(cCursor.info.x,cCursor.info.y,520,menu.y+51+35,90,30) ) menu.select = 6; // 취소
			if( isCrash(cCursor.info.x,cCursor.info.y,430,menu.y+51+90,19,37) ) menu.select = 1; // 좌우 화살
			if( isCrash(cCursor.info.x,cCursor.info.y,510,menu.y+51+90,19,37) ) menu.select = 2;
			if( isCrash(cCursor.info.x,cCursor.info.y,430,menu.y+51+150,19,37) ) menu.select = 3;
			if( isCrash(cCursor.info.x,cCursor.info.y,510,menu.y+51+150,19,37) ) menu.select = 4;
		}
		else{
			menu.Close();
		}

		SDL_RenderPresent(screen);

		// Z키가 눌리면
		if(GetKeyState('Z') >= 0) key_stateZ=false;
		if(GetKeyState('Z') < 0 && key_stateZ==false){
			key_stateZ=true;
			if(stack.now>1){
				// 이전 맵 스택에서 하나 빼서
				stack.now--;
				stack.Pop(map);
				// 현재 맵으로 교체
				Undo();
				// 사용자에게 보여줄 맵
				img_map = SDL_CreateTextureFromSurface(screen,img_maps);
			}
		}
		// Y키가 눌리면
		if(GetKeyState('Y') >= 0) key_stateY=false;
		if(GetKeyState('Y') < 0 && key_stateY==false){
			key_stateY=true;
			if(stack.now<stack.top){
				// 다음 맵 스택에서 하나 빼서
				stack.now++;
				stack.Pop(map);
				// 현재 맵으로 교체
				Undo();
				// 사용자에게 보여줄 맵
				img_map = SDL_CreateTextureFromSurface(screen,img_maps);
			}
		}

		// 프로그램에 이벤트가 있을 때
		if( SDL_PollEvent( &event ) )
		{
			// 마우스 움직임 감지
			if( event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION ){
				// 마우스 좌표 설정
				cCursor.info.x = x = event.motion.x;
				cCursor.info.y = y = event.motion.y;
				
				// 소리 크기 조절
				if(event.button.button == SDL_BUTTON_LEFT){
					if(isCrash(cCursor.info.x,cCursor.info.y,505,menu.y+51+205,100,30)){
						(*volume_e) = (float)((cCursor.info.x-465)/(float)80);
						if((*volume_e)<0.0) (*volume_e) = 0.0;
						if((*volume_e)>1.0) (*volume_e) = 1.0;
					}
					if(isCrash(cCursor.info.x,cCursor.info.y,505,menu.y+51+245,100,30)){
						(*volume) = (float)((cCursor.info.x-465)/(float)80);
						if((*volume)<0.0) (*volume) = 0.0;
						if((*volume)>1.0) (*volume) = 1.0;
						FMOD_Channel_SetVolume(channel,(*volume));
					}
				}

				// 메뉴를 눌렀다면
				if( event.type == SDL_MOUSEBUTTONDOWN ){
					// 저장
					if(menu.select==5 && ep==true){
						Save(fscale,wscale,menu.img);
					}
					if(menu.select==6){
						break;
					}
					switch(menu.select){
					case 1:
						bgnum--;
						if(bgnum<1) bgnum=7;
						break;
					case 2:
						bgnum++;
						if(bgnum>7) bgnum=1;
						break;
					case 3:
						bgtnum--;
						if(bgtnum<1) bgtnum=4;
						break;
					case 4:
						bgtnum++;
						if(bgtnum>4) bgtnum=1;
						break;
					}
				}
				// 메뉴 위면 메뉴만 클릭해라
				if(menu.isOverMenu(cCursor.info.x,cCursor.info.y)) continue; 

				// 클릭 했을 때
				if( event.type == SDL_MOUSEBUTTONDOWN || ( (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT || event.button.button == 4) && event.type == SDL_MOUSEMOTION ) ){
					chk = false;
					for(i=0;i<6;i++){
						// 사이즈 변경을 클릭 했다면
						if( toolLE[i] == true && x >= 70 && x < 70+350 && y >= i*70 && y< (i+1)*70 ){
							chk = true;
							cCursor.Update( (y/70+1)*10 + x/70 );
							break;
						}
					}
					if( x<70 && event.button.button == 4) chk = true; // 오른쪽 클릭 드래그 중에는 툴바가 나오자마자 들어가는거 방지
					// 아니라면
					if( chk == false ){
						// 사이즈 툴바를 들어가게 한다
						for(i=0;i<6;i++){
							if(toolLE[i] == true) chk = true; // 사이즈 변경창을 닫을 때는 맵에 그려주지 말자
							toolLE[i] = false;
						}
					}
					if( chk == true){
						// 다른 툴바를 눌렀을 경우를 대비해서
						// 왼쪽 툴바를 오른쪽 클릭으로 눌렀다면
						if(x<=70 && event.button.button == SDL_BUTTON_RIGHT && event.button.button!=4){
							// 누른 툴의 사이즈 툴바를 나오게 한다
							toolLE[y/70] = true;
						}
						continue; // 사이즈 변경시에는 맵에 그려주지 말자
					}
				}
				
				// 왼쪽 마우스 클릭 상태
				if(event.button.button == SDL_BUTTON_LEFT){
					// 왼쪽 툴바 클릭
					if(x<=70 && event.type != SDL_MOUSEMOTION){
						if(y<=420){
							cCursor.Update( (y/70)*10 +13 );
						}
						else{
							// 연필 / 지우개 클릭
							if(y<=490) soh = !soh;
							else woe = !woe;
						}
					}
					// 오른쪽 툴바 클릭
					else if(x>=WIDTH+70 && event.type != SDL_MOUSEMOTION){
						if(y<360){ // 환경
							cCursor.Update( (y/60) +71 );
						}
						else{ // 포인트
							cCursor.Update( (y-360)/60 + 77 );
							if(cCursor.type==77) cCursor.scale = fsc;
							if(cCursor.type==78) cCursor.scale = wsc;
						}
					}

					// 맵을 클릭한거면
					else{
						// 맵을 그리는 중이면
						if(cCursor.type<70){
							write = true;
							// 맵에 그려준다
							DrawShape(x-70,y);
							// 사용자에게 보여줄 맵
							SDL_DestroyTexture(img_map);
							img_map = SDL_CreateTextureFromSurface(screen,img_maps);
						}
						// 환경을 그리는 중이면
						else if(cCursor.type>=70 && event.type != SDL_MOUSEMOTION){ // 드래그는 안됨
							// 환경 추가
							if(cCursor.type==80){ // 삭제!
								for(i=1;i<=envt.n;i++){
									if(isCrash(cCursor.info.x,cCursor.info.y,envt.e[i].x,envt.e[i].y,40,40)){
										if(envt.e[i].type==7) fsp = false;
										if(envt.e[i].type==8) wsp = false;
										if(envt.e[i].type==9) ep = false;
										for(;i<envt.n;i++){
											envt.e[i] = envt.e[i+1];
										}
										envt.n--;
										break;
									}
								}
							}
							else if(cCursor.type>=77){ // 포인트
								if(cCursor.type==77){
									fscale = fsc; // 크기 지정
									fsp = true;
								}
								if(cCursor.type==78){
									wscale = wsc;
									wsp = true;
								}
								if(cCursor.type==79) ep = true;
								for(i=1;i<=envt.n;i++){ // 이미 있으면 변경
									if(envt.e[i].type==cCursor.type-70){
										envt.e[i].x = x;
										envt.e[i].y = y;
										i=-1; break;
									}
								}
								if(i!=-1){ // 없으면 추가
									envt.n++;
									envt.e[envt.n].type = cCursor.type-70;
									envt.e[envt.n].x = x;
									envt.e[envt.n].y = y;
								}
							}
							else{ // 그냥 환경
								envt.n++;
								envt.e[envt.n].type = cCursor.type-70;
								envt.e[envt.n].x = x;
								envt.e[envt.n].y = y;
							}
						}
					}
				}
				// 오른쪽 마우스 클릭 상태
				else if(event.button.button == SDL_BUTTON_RIGHT || event.button.button==4){
					// 왼쪽 툴바를 눌렀다면
					if(x<=70 && event.button.button!=4){
						// 누른 툴의 사이즈 툴바를 나오게 한다
						toolLE[y/70] = true;
					}
					else{
						// 연필/지우개 상태 바꾸기
						if(event.button.button == SDL_BUTTON_RIGHT){
							rwoe = true;
							woe=!woe;
						}
						write = true;
						// 맵에 그려준다
						DrawShape(x-70,y);
						// 사용자에게 보여줄 맵
						SDL_DestroyTexture(img_map);
						img_map = SDL_CreateTextureFromSurface(screen,img_maps);
					}
				}
			}
			// 마우스를 떼면
			if( event.type == SDL_MOUSEBUTTONUP ){
				// 만약 그린적이 있으면
				if(write == true){
					write = false;
					stack.Push(map);
				}
			}
			// 오른쪽 마우스를 눌렀다 떼면
			if( x>=70 && event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT ){
				// 연필/지우개 상태 바꾸기
				if( rwoe == true ){
					rwoe = false;
					woe=!woe;
				}
			}
			if( event.type == SDL_MOUSEWHEEL ){
				// 커서가 불,물 포인트일 경우
				if(cCursor.type==77){
					if(event.wheel.y<0 && fsc>0.5) fsc-=0.125;
					else if(event.wheel.y>0 && fsc<1.0) fsc+=0.125;
					cCursor.scale = fsc;
				}
				else if(cCursor.type==78){
					if(event.wheel.y<0 && wsc>0.5) wsc-=0.125;
					else if(event.wheel.y>0 && wsc<1.0) wsc+=0.125;
					cCursor.scale = wsc;
				}
				else if(cCursor.type==79){
				}
				else{
					if(event.wheel.y<0 && cCursor.type%10>1){
						cCursor.Update(cCursor.type-1);
					}
					else if(event.wheel.y>0){
						if(cCursor.type<70 && cCursor.type%10<5)
							cCursor.Update(cCursor.type+1);
						if(cCursor.type>70 && cCursor.type%10<6)
							cCursor.Update(cCursor.type+1);
					}
				}
			}
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}

	CloseTool();
	ClearEvent(&event);
	
	SDL_DestroyTexture(menu.minibg);
	SDL_DestroyTexture(menu.minibgt);
	SDL_DestroyTexture(menu.arrow);
	SDL_DestroyTexture(menu.arrowhite);

	Destroy();

	return text_title;
}

void MakeMap::Set(){
	int i;

	char filename[100]={"image\\background\\back0.png"};
	for(i=0;i<=7;i++){
		filename[21]=i+'0';
		this->img_bgi[i] = load_image(screen,filename); // 뒷배경
	}
	this->img_maps = IMG_Load("image\\blank.png"); // 만들 맵 서피스 기본 값
	this->img_map = SDL_CreateTextureFromSurface(screen,img_maps); // 사용자에게 보여줄 맵 기본 값
	this->img_load = load_image(screen,"image\\load.png"); // 로딩 이미지
	this->img_tool = load_image(screen,"image\\toolbar.png"); // 툴바 배경 이미지
	this->img_tbdr = load_image(screen,"image\\border.png"); // 경계선
	this->img_tbdr2 = load_image(screen,"image\\border2.png"); // 경계선
	this->img_woe = load_image(screen,"image\\woe.png"); // 연필 / 지우개
	this->img_soh = load_image(screen,"image\\soh.png"); // 배경 보이기 / 가리기
	this->img_toolSize = load_image(screen,"image\\sizebar.png"); // 사이즈 툴바 배경 이미지
	this->img_toolR = load_image(screen,"image\\toolspriteR.png"); // 오른쪽 툴바
	this->envt.img[1] = load_image(screen,"image\\cursor\\cursor71.png"); // 환경 : 불
	this->envt.img[2] = load_image(screen,"image\\cursor\\cursor72.png"); // 환경 : 물
	this->envt.img[3] = load_image(screen,"image\\cursor\\cursor73.png"); // 환경 : 나무
	this->envt.img[4] = load_image(screen,"image\\cursor\\cursor74.png"); // 환경 : 구름->
	this->envt.img[5] = load_image(screen,"image\\cursor\\cursor75.png"); // 환경 : 구름<-
	this->envt.img[6] = load_image(screen,"image\\cursor\\cursor76.png"); // 환경 : 돌멩이
	this->envt.img[7] = load_image(screen,"image\\cursor\\cursor77.png"); // 포인트 : 불 시작 포인트
	this->envt.img[8] = load_image(screen,"image\\cursor\\cursor78.png"); // 포인트 : 물 시작 포인트
	this->envt.img[9] = load_image(screen,"image\\cursor\\cursor79.png"); // 포인트 : 끝 포인트
	
	// 왼쪽 툴 초기 상태
	for(i=0;i<6;i++){
		toolL[i] = 3; // 각 툴의 초기 크기
		toolLX[i] = 70-350; // 크기 툴바 x 위치
		toolLT[i] = 0; // 크기 툴바 가속 값
		toolLE[i] = false; // 나오는중? 들어가는중?
	}
	
	// 커서 초기화
	cCursor.Set(screen);
	cCursor.Update(0);

	Clear();
}
void MakeMap::Destroy(){
	int i;
	for(i=0;i<=7;i++){
		SDL_DestroyTexture(img_bgi[i]);
	}
	SDL_FreeSurface(img_maps);
	SDL_DestroyTexture(img_map);
	SDL_DestroyTexture(img_load);
	SDL_DestroyTexture(img_tool);
	SDL_DestroyTexture(img_tbdr);
	SDL_DestroyTexture(img_tbdr2);
	SDL_DestroyTexture(img_woe);
	SDL_DestroyTexture(img_soh);
	SDL_DestroyTexture(img_toolSize);
	SDL_DestroyTexture(img_toolR);
	for(i=1;i<=9;i++) SDL_DestroyTexture(envt.img[i]);
}
void MakeMap::Clear(){
	int i, j;

	bgnum = bgtnum = 1;
	// 쓰기 상태
	woe = true;
	rwoe = false;
	// 배경 보이기 상태
	soh = true;
	// 맵 스택 초기화
	write = false;
	stack.now = stack.top = 0;
	// 맵 환경 초기화
	envt.n = 0;
	
	strcpy_s(text_title,""); // 제목
	for(i=0;i<3;i++) strcpy_s(text_desc[i],"");
	len_title=0;
	for(i=0;i<3;i++) len_desc[i]=0;
	line=0;
	cursor_title = 340;
	for(i=0;i<3;i++) cursor_desc[i]=340;

	write = false;

	for(i=0;i<=HEIGHT;i++){
		for(j=0;j<=WIDTH;j++){
			map[i][j]=false;
		}
	}
}