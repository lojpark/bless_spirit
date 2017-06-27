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
	// ���� ���¶��
	if(woe == true){
		// ���� �� ���ǽ��� � �� ���
		*((Uint32 *)img_maps->pixels + y*img_maps->pitch/4 + x) = SDL_MapRGB(img_maps->format, 0, 0, 0);
		// �� ���� ����
		map[y][x]=true;
	}
	// ����� ���¶��
	else{
		// ���� �� ���ǽ� �����
		*((Uint32 *)img_maps->pixels + y*img_maps->pitch/4 + x) = NULL;
		// �� ���� ����
		map[y][x]=false;
	}
	return;
}
void MakeMap::DrawShape(int x, int y)
{
	// ���� ���� ��� ���¶��
	if(cCursor.type==11){
		// �� ��� ����
		Draw(x,y);
		return;
	}
	int i, j, i2, j2;
	// Ŀ�� ũ���� �� ���鼭
	i2=0;
	for(i=-cCursor.info.h/2;i<cCursor.info.h/2;i++){
		j2=0;
		for(j=-cCursor.info.w/2;j<cCursor.info.w/2;j++){
			// ���� �κ��̸�
			if(SDL_MapRGB(cCursor.img_cursors->format, 0, 0, 0) == *((Uint32 *)cCursor.img_cursors->pixels + i2*cCursor.img_cursors->pitch/4 + j2) ){
				//�� ���
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
		// ��Ҹ� �����ٸ�
		if(cancel){
			motion--;
			scale = sin(motion/30);
			apply_surface(470,300,img_menu,screen,scale,90,false,true);
			if(motion<=0){
				SDL_DestroyTexture(img_line);
				return false;
			}
		}
		// ���� ��
		else if(motion/30 < PI/2){
			motion++;
			scale = sin(motion/30);
			apply_surface(470,300,img_menu,screen,scale,90,false,true);
		}
		// �� ��
		else{
			RGB[0]=RGB[1]=0;
			// ����
			if(isCrash(cCursor.info.x,cCursor.info.y,530,215,50,30)) RGB[0]=255;
			// ���
			if(isCrash(cCursor.info.x,cCursor.info.y,585,215,50,30)) RGB[1]=255;
			// �� Ŀ��
			if(isCrash(cCursor.info.x,cCursor.info.y,400,240,272,30) || isCrash(cCursor.info.x,cCursor.info.y,400,350,272,90)){
				cCursor.Update(1);
			}else cCursor.Update(0);

			// �� ���� ���� �Ұ�
			if(strlen(text_title)==0 || strlen(text_desc[0])==0){
				RGB[0]=128;
			}

			apply_surface(470,300,img_menu,screen,scale,90,false,true);
			font_t.Print(530,215,"����",RGB[0],RGB[0],RGB[0]);
			font_t.Print(585,215,"���",RGB[1],RGB[1],RGB[1]);

			font_t.Print(360,215,"����",0,0,0);
			font_d.PrintInput(340,240,text_title,0,0,0,false);
			apply_surface(470,250,img_line,screen,1.0,0.0,false,true);

			font_t.Print(360,290,"����",0,0,0);
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

		// ���α׷��� �̺�Ʈ�� ���� ��
		if( SDL_PollEvent( &event ) )
		{
			// ���콺 ������ ����
			if( event.type == SDL_MOUSEMOTION){
				// ���콺 ��ǥ ����
				cCursor.info.x = event.motion.x;
				cCursor.info.y = event.motion.y;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN && !cancel){
				// ����
				if(RGB[0]==255) break;
				// ���
				if(RGB[1]==255) cancel=true;

				// Ŀ�� ��Ŀ�� �̵�
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

	// ������ ����
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
	// �� ���ǽ� ������Ʈ
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

	// ����ڿ��� ������ �� ������Ʈ
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
	
	// ��� ���
	if(soh) apply_surface(70, 0, img_bgi[bgnum], screen, 1.0, 0, false, false);
	else apply_surface(70, 0, img_bgi[0], screen, 1.0, 0, false, false);

	// ȯ�� ���
	j=0;
	if(cCursor.type==80){ // ���� ������ ȯ���� �߷�����
		for(i=1;i<=envt.n;i++){
			// �ϳ��� ����
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

	// ����Ʈ ���
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

	// �� ���
	apply_surface(70, 0, img_map, screen, 1.0, 0, false, false);

	SDL_Rect clip;
	clip.x = clip.y = 0;
	clip.h = clip.w = 70;

	// ���� ���� ũ��� ���
	for(i=0;i<6;i++){
		// ���
		apply_surface( toolLX[i] + 175 , i*70+35, img_toolSize, screen,1.0,0,false,true);
		for(j=0;j<5;j++){
			// Ŀ�� ���
			apply_surface( toolLX[i] + (j*70) + 35 , i*70+35,
			cCursor.img_cursorchip[ (i+1)*10 + j + 1 ],
			screen,1.0,0,false,true);
		}
	}
	// ���� ���� ��� ���
	apply_surface(0,0,img_tool,screen,1.0,0,false,false);
	for(i=0;i<6;i++){ // ���� ���� �̹��� ���
		// �����ڸ�
		apply_surface(0,i*70,img_tbdr,screen,1.0,0,false,false);
		apply_surface(35,i*70+35,
			cCursor.img_cursorchip[ (i+1)*10 + toolL[i] ],
			screen,1.0,0,false,true);
	}
	// �����ڸ�
	apply_surface(0,6*70,img_tbdr,screen,1.0,0,false,false);
	apply_surface(0,7*70,img_tbdr,screen,1.0,0,false,false);
	// ��� ���̱� / ������ ���
	if(soh == true) clip.x=0;
	else clip.x=70;
	apply_surface(0,6*70,img_soh,screen,1.0,0,false,false,&clip);
	// ���� / ���찳 ���
	if(woe == true) clip.x=0;
	else clip.x=70;
	apply_surface(0,7*70,img_woe,screen,1.0,0,false,false,&clip);

	// ������ ���� ��� ���
	clip.h = 60;
	apply_surface(WIDTH+70,0,img_tool,screen,1.0,0,false,false);
	for(i=0;i<10;i++){ // ������ ���� ȯ�� �̹��� ���
		// �����ڸ�
		apply_surface(WIDTH+70,i*60,img_tbdr2,screen,1.0,0,false,false);
		clip.x = i*70;
		apply_surface(WIDTH+70,i*60,img_toolR,screen,1.0,0,false,false,&clip);
	}
}

void MakeMap::ClearEvent(SDL_Event* event){
	while(1){
		if( SDL_PollEvent( event ) )
		{
			// ���콺 ������ ����
			if( event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION ){
				// ���콺 ��ǥ ����
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
		// ������ ����
		if(toolLE[i] == true){
			if( toolLT[i]/50 >= PI/2 ){ // �� ��������
				toolLX[i] = 70;
			}
			else{ // ������ ���̸�
				toolLX[i] = -280 + (int)( 350*sin(toolLT[i]/50) );
				toolLT[i]++;
			}
		}
		// ���� ����
		else{
			if(toolLT[i]/50 <= 0){ // �� ������
				toolLX[i] = -280;
			}
			else{ // ���� ���̸�
				toolLX[i] = -280 + (int)( 350*sin(toolLT[i]/50) );
				toolLT[i]--;
			}
		}
	}
}

void MakeMap::Stack::Push(bool map[][WIDTH+10]){
	int i, j;

	// ������ ��á�ٸ� ���� ���� ���� ������
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
	bool fsp = false, wsp = false; // ��, �� ���� ����Ʈ
	bool ep = false; // �� ����Ʈ
	double fscale = 1.0, wscale = 1.0; // ��, �� ���� ũ��
	double fsc = 1.0, wsc = 1.0; // ��, �� ���� ũ�� ����
	int i=0;
	int x=-1, y=-1;

	Set();

	// �޴�(߾) �ʱ�ȭ
	Menu menu;
	menu.Set(470,screen);
	menu.minibg = load_image(screen,"image\\minibg.png"); // ��� ������
	menu.minibgt = load_image(screen,"image\\minibgt.png"); // ���� �ؽ��� ������
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

	// �� �ҷ�����
	SetTool();
	ClearEvent(&event);

	// �ҷ��Ծ�?
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
	
	// ����� �Ҹ� ����
	FMOD_Channel_SetVolume(channel,(*volume));

	Uint32 curTicks = SDL_GetTicks();
	
	while(!quit){
		curTicks += Set_Frame(curTicks);

		// ���� ������ ����
		SetToolSize();

		Print(fscale,wscale);
		menu.Print(bgnum,bgtnum,(*volume),(*volume_e),ep);
		cCursor.Print();

		printf("\n%d %d\n",stack.now, stack.top);

		// �޴��� ���콺�� ����
		if( menu.isOverMenu(cCursor.info.x,cCursor.info.y) ){
			menu.Open();
			// �޴� ����
			menu.select = 0;
			if( isCrash(cCursor.info.x,cCursor.info.y,420,menu.y+51+35,60,30) ) menu.select = 5; // ����
			if( isCrash(cCursor.info.x,cCursor.info.y,520,menu.y+51+35,90,30) ) menu.select = 6; // ���
			if( isCrash(cCursor.info.x,cCursor.info.y,430,menu.y+51+90,19,37) ) menu.select = 1; // �¿� ȭ��
			if( isCrash(cCursor.info.x,cCursor.info.y,510,menu.y+51+90,19,37) ) menu.select = 2;
			if( isCrash(cCursor.info.x,cCursor.info.y,430,menu.y+51+150,19,37) ) menu.select = 3;
			if( isCrash(cCursor.info.x,cCursor.info.y,510,menu.y+51+150,19,37) ) menu.select = 4;
		}
		else{
			menu.Close();
		}

		SDL_RenderPresent(screen);

		// ZŰ�� ������
		if(GetKeyState('Z') >= 0) key_stateZ=false;
		if(GetKeyState('Z') < 0 && key_stateZ==false){
			key_stateZ=true;
			if(stack.now>1){
				// ���� �� ���ÿ��� �ϳ� ����
				stack.now--;
				stack.Pop(map);
				// ���� ������ ��ü
				Undo();
				// ����ڿ��� ������ ��
				img_map = SDL_CreateTextureFromSurface(screen,img_maps);
			}
		}
		// YŰ�� ������
		if(GetKeyState('Y') >= 0) key_stateY=false;
		if(GetKeyState('Y') < 0 && key_stateY==false){
			key_stateY=true;
			if(stack.now<stack.top){
				// ���� �� ���ÿ��� �ϳ� ����
				stack.now++;
				stack.Pop(map);
				// ���� ������ ��ü
				Undo();
				// ����ڿ��� ������ ��
				img_map = SDL_CreateTextureFromSurface(screen,img_maps);
			}
		}

		// ���α׷��� �̺�Ʈ�� ���� ��
		if( SDL_PollEvent( &event ) )
		{
			// ���콺 ������ ����
			if( event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION ){
				// ���콺 ��ǥ ����
				cCursor.info.x = x = event.motion.x;
				cCursor.info.y = y = event.motion.y;
				
				// �Ҹ� ũ�� ����
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

				// �޴��� �����ٸ�
				if( event.type == SDL_MOUSEBUTTONDOWN ){
					// ����
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
				// �޴� ���� �޴��� Ŭ���ض�
				if(menu.isOverMenu(cCursor.info.x,cCursor.info.y)) continue; 

				// Ŭ�� ���� ��
				if( event.type == SDL_MOUSEBUTTONDOWN || ( (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT || event.button.button == 4) && event.type == SDL_MOUSEMOTION ) ){
					chk = false;
					for(i=0;i<6;i++){
						// ������ ������ Ŭ�� �ߴٸ�
						if( toolLE[i] == true && x >= 70 && x < 70+350 && y >= i*70 && y< (i+1)*70 ){
							chk = true;
							cCursor.Update( (y/70+1)*10 + x/70 );
							break;
						}
					}
					if( x<70 && event.button.button == 4) chk = true; // ������ Ŭ�� �巡�� �߿��� ���ٰ� �����ڸ��� ���°� ����
					// �ƴ϶��
					if( chk == false ){
						// ������ ���ٸ� ���� �Ѵ�
						for(i=0;i<6;i++){
							if(toolLE[i] == true) chk = true; // ������ ����â�� ���� ���� �ʿ� �׷����� ����
							toolLE[i] = false;
						}
					}
					if( chk == true){
						// �ٸ� ���ٸ� ������ ��츦 ����ؼ�
						// ���� ���ٸ� ������ Ŭ������ �����ٸ�
						if(x<=70 && event.button.button == SDL_BUTTON_RIGHT && event.button.button!=4){
							// ���� ���� ������ ���ٸ� ������ �Ѵ�
							toolLE[y/70] = true;
						}
						continue; // ������ ����ÿ��� �ʿ� �׷����� ����
					}
				}
				
				// ���� ���콺 Ŭ�� ����
				if(event.button.button == SDL_BUTTON_LEFT){
					// ���� ���� Ŭ��
					if(x<=70 && event.type != SDL_MOUSEMOTION){
						if(y<=420){
							cCursor.Update( (y/70)*10 +13 );
						}
						else{
							// ���� / ���찳 Ŭ��
							if(y<=490) soh = !soh;
							else woe = !woe;
						}
					}
					// ������ ���� Ŭ��
					else if(x>=WIDTH+70 && event.type != SDL_MOUSEMOTION){
						if(y<360){ // ȯ��
							cCursor.Update( (y/60) +71 );
						}
						else{ // ����Ʈ
							cCursor.Update( (y-360)/60 + 77 );
							if(cCursor.type==77) cCursor.scale = fsc;
							if(cCursor.type==78) cCursor.scale = wsc;
						}
					}

					// ���� Ŭ���ѰŸ�
					else{
						// ���� �׸��� ���̸�
						if(cCursor.type<70){
							write = true;
							// �ʿ� �׷��ش�
							DrawShape(x-70,y);
							// ����ڿ��� ������ ��
							SDL_DestroyTexture(img_map);
							img_map = SDL_CreateTextureFromSurface(screen,img_maps);
						}
						// ȯ���� �׸��� ���̸�
						else if(cCursor.type>=70 && event.type != SDL_MOUSEMOTION){ // �巡�״� �ȵ�
							// ȯ�� �߰�
							if(cCursor.type==80){ // ����!
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
							else if(cCursor.type>=77){ // ����Ʈ
								if(cCursor.type==77){
									fscale = fsc; // ũ�� ����
									fsp = true;
								}
								if(cCursor.type==78){
									wscale = wsc;
									wsp = true;
								}
								if(cCursor.type==79) ep = true;
								for(i=1;i<=envt.n;i++){ // �̹� ������ ����
									if(envt.e[i].type==cCursor.type-70){
										envt.e[i].x = x;
										envt.e[i].y = y;
										i=-1; break;
									}
								}
								if(i!=-1){ // ������ �߰�
									envt.n++;
									envt.e[envt.n].type = cCursor.type-70;
									envt.e[envt.n].x = x;
									envt.e[envt.n].y = y;
								}
							}
							else{ // �׳� ȯ��
								envt.n++;
								envt.e[envt.n].type = cCursor.type-70;
								envt.e[envt.n].x = x;
								envt.e[envt.n].y = y;
							}
						}
					}
				}
				// ������ ���콺 Ŭ�� ����
				else if(event.button.button == SDL_BUTTON_RIGHT || event.button.button==4){
					// ���� ���ٸ� �����ٸ�
					if(x<=70 && event.button.button!=4){
						// ���� ���� ������ ���ٸ� ������ �Ѵ�
						toolLE[y/70] = true;
					}
					else{
						// ����/���찳 ���� �ٲٱ�
						if(event.button.button == SDL_BUTTON_RIGHT){
							rwoe = true;
							woe=!woe;
						}
						write = true;
						// �ʿ� �׷��ش�
						DrawShape(x-70,y);
						// ����ڿ��� ������ ��
						SDL_DestroyTexture(img_map);
						img_map = SDL_CreateTextureFromSurface(screen,img_maps);
					}
				}
			}
			// ���콺�� ����
			if( event.type == SDL_MOUSEBUTTONUP ){
				// ���� �׸����� ������
				if(write == true){
					write = false;
					stack.Push(map);
				}
			}
			// ������ ���콺�� ������ ����
			if( x>=70 && event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT ){
				// ����/���찳 ���� �ٲٱ�
				if( rwoe == true ){
					rwoe = false;
					woe=!woe;
				}
			}
			if( event.type == SDL_MOUSEWHEEL ){
				// Ŀ���� ��,�� ����Ʈ�� ���
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
		this->img_bgi[i] = load_image(screen,filename); // �޹��
	}
	this->img_maps = IMG_Load("image\\blank.png"); // ���� �� ���ǽ� �⺻ ��
	this->img_map = SDL_CreateTextureFromSurface(screen,img_maps); // ����ڿ��� ������ �� �⺻ ��
	this->img_load = load_image(screen,"image\\load.png"); // �ε� �̹���
	this->img_tool = load_image(screen,"image\\toolbar.png"); // ���� ��� �̹���
	this->img_tbdr = load_image(screen,"image\\border.png"); // ��輱
	this->img_tbdr2 = load_image(screen,"image\\border2.png"); // ��輱
	this->img_woe = load_image(screen,"image\\woe.png"); // ���� / ���찳
	this->img_soh = load_image(screen,"image\\soh.png"); // ��� ���̱� / ������
	this->img_toolSize = load_image(screen,"image\\sizebar.png"); // ������ ���� ��� �̹���
	this->img_toolR = load_image(screen,"image\\toolspriteR.png"); // ������ ����
	this->envt.img[1] = load_image(screen,"image\\cursor\\cursor71.png"); // ȯ�� : ��
	this->envt.img[2] = load_image(screen,"image\\cursor\\cursor72.png"); // ȯ�� : ��
	this->envt.img[3] = load_image(screen,"image\\cursor\\cursor73.png"); // ȯ�� : ����
	this->envt.img[4] = load_image(screen,"image\\cursor\\cursor74.png"); // ȯ�� : ����->
	this->envt.img[5] = load_image(screen,"image\\cursor\\cursor75.png"); // ȯ�� : ����<-
	this->envt.img[6] = load_image(screen,"image\\cursor\\cursor76.png"); // ȯ�� : ������
	this->envt.img[7] = load_image(screen,"image\\cursor\\cursor77.png"); // ����Ʈ : �� ���� ����Ʈ
	this->envt.img[8] = load_image(screen,"image\\cursor\\cursor78.png"); // ����Ʈ : �� ���� ����Ʈ
	this->envt.img[9] = load_image(screen,"image\\cursor\\cursor79.png"); // ����Ʈ : �� ����Ʈ
	
	// ���� �� �ʱ� ����
	for(i=0;i<6;i++){
		toolL[i] = 3; // �� ���� �ʱ� ũ��
		toolLX[i] = 70-350; // ũ�� ���� x ��ġ
		toolLT[i] = 0; // ũ�� ���� ���� ��
		toolLE[i] = false; // ��������? ������?
	}
	
	// Ŀ�� �ʱ�ȭ
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
	// ���� ����
	woe = true;
	rwoe = false;
	// ��� ���̱� ����
	soh = true;
	// �� ���� �ʱ�ȭ
	write = false;
	stack.now = stack.top = 0;
	// �� ȯ�� �ʱ�ȭ
	envt.n = 0;
	
	strcpy_s(text_title,""); // ����
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