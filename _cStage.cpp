#pragma once
#include "_stdafx.h"

void Stage::Set(SDL_Renderer *screen){
	int i, j, temp=0;

	// 배경 이미지 세팅
	//bgi.Set(screen);

	envt.img[1] = load_image(screen,"image\\envt-fire.png"); // 환경 : 불
	envt.img[2] = load_image(screen,"image\\envt-water.png"); // 환경 : 물
	envt.img[3] = load_image(screen,"image\\envt-tree.png"); // 환경 : 나무
	envt.img[4] = load_image(screen,"image\\envt-cloud.png"); // 환경 : 구름->
	envt.img[5] = load_image(screen,"image\\envt-cloud.png"); // 환경 : 구름<-
	envt.img[6] = load_image(screen,"image\\envt-stone.png"); // 환경 : 돌멩이
	envt.img[9] = load_image(screen,"image\\envt-thunder.png"); // 환경 : 벼락
	envt.img[0] = load_image(screen,"image\\envt-rain.png"); // 환경 : 비

	SDL_SetTextureBlendMode(envt.img[4],SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(envt.img[5],SDL_BLENDMODE_BLEND);

	// 환경의 스트라이프 클립 셋팅
	// 불
	//for(i=0;i<5;i++){
		for(j=0;j<4;j++){
			envt.clip[1][0][j].x=j*48;
			envt.clip[1][0][j].y=0;
			envt.clip[1][0][j].w=48;
			envt.clip[1][0][j].h=48;
		}
	//}
	// 물
	for(i=0;i<5;i++){
		for(j=0;j<8;j++){
			envt.clip[2][i][j].x=j*32;
			envt.clip[2][i][j].y=i*32;
			envt.clip[2][i][j].w=32;
			envt.clip[2][i][j].h=32;
		}
	}
	// 나무
	for(i=0;i<6;i++){
		for(j=0;j<1;j++){
			envt.clip[3][i][j].x=j*72;
			envt.clip[3][i][j].y=i*96;
			envt.clip[3][i][j].w=72;
			envt.clip[3][i][j].h=96;
		}
	}
	// 구름

	// 비
	for(i=0;i<3;i++){
		for(j=0;j<1;j++){
			envt.clip[0][i][j].x=j*16;
			envt.clip[0][i][j].y=i*15;
			envt.clip[0][i][j].w=16;
			envt.clip[0][i][j].h=15;
		}
	}
}
void Stage::Set2(SDL_Renderer *screen, int stagenum, bool reset, bool create){
	int i, j, temp=0;

	// 맵 로드
	FILE *in;
	char filename[100]={"data\\map\\map000.txt"};
	filename[13]=stagenum/10 + '0';
	filename[14]=stagenum%10 + '0';
	if(create){
		strcpy_s(filename,"data\\usermap\\map000.txt");
		filename[17]=stagenum/10 + '0';
		filename[18]=stagenum%10 + '0';
	}
	fopen_s(&in,filename,"r");

	fscanf_s(in,"%d\n",&envt.n);
	cFire.x = cFire.y = 0;
	cWater.x = cWater.y = 0;
	for(i=1;i<=envt.n;i++){
		fscanf_s(in,"%d %d %d\n",&envt.e[i].type,&envt.e[i].x,&envt.e[i].y);
		envt.e[i].x-=70;
		envt.e[i].cx = envt.e[i].x;
		envt.e[i].cy = envt.e[i].y;
		envt.e[i].oy = envt.e[i].y;
		envt.e[i].die = 0;
		envt.e[i].motion = 0;
		envt.e[i].angle = 0;
		envt.e[i].ax = 0;
		envt.e[i].ay = 0;
		envt.e[i].rainfire = false;
		envt.e[i].use = false;
		envt.e[i].erase = false;
		envt.e[i].crash = false;
		envt.e[i].whose = false;
		envt.e[i].gy = 0.0;
		// 크기 지정
		switch(envt.e[i].type){
		case 1:
			envt.e[i].w = envt.e[i].h = 48;
			break;
		case 2:
			envt.e[i].w = envt.e[i].h = 32;
			break;
		case 3:
			envt.e[i].w = 36; envt.e[i].h = 72;
			break;
		case 6:
			envt.e[i].w = envt.e[i].h = 32;
			break;
		}
		// 구름 떠오르기 효과 없앰
		if(envt.e[i].type==4 || envt.e[i].type==5) envt.e[i].motion = 300;
		// 시작 포인트 지정
		if(envt.e[i].type==7){
			cFire.x = envt.e[i].x; cFire.y = envt.e[i].y;
		}
		if(envt.e[i].type==8){
			cWater.x = envt.e[i].x; cWater.y = envt.e[i].y;
		}
		// 끝 포인트 지정
		if(envt.e[i].type==9){
			bgi.end.x = envt.e[i].x;
			bgi.end.y = envt.e[i].y;
			bgi.end.motion = 0.0;

			envt.e[i].type=0;
			envt.e[i].x=-100;
		}
	}
	fscanf_s(in,"%lf %lf",&cFire.scale,&cWater.scale);
	cFire.tscale = cFire.scale;
	cWater.tscale = cWater.scale;

	if(reset) return;
	
	strcpy_s(bgi.text_title,"");
	strcpy_s(bgi.text_desc[0],"");
	strcpy_s(bgi.text_desc[1],"");
	strcpy_s(bgi.text_desc[2],"");

	fscanf_s(in,"%d\n",&temp);
	for(i=0;i<3;i++) fscanf_s(in,"%d ",&temp);
	fscanf_s(in,"\n%d %d\n",&bgi.line,&temp);
	for(i=0;i<3;i++) fscanf_s(in,"%d ",&temp);

	for(i=0;;i++){
		fscanf_s(in,"%c",&bgi.text_title[i]);
		if(bgi.text_title[i]=='\n'){
			bgi.text_title[i]=NULL;
			break;
		}
	}
	for(i=0;;i++){
		fscanf_s(in,"%c",&bgi.text_desc[0][i]);
		if(bgi.text_desc[0][i]=='\n'){
			bgi.text_desc[0][i]=NULL;
			break;
		}
	}
	if(bgi.line>0){
		for(i=0;;i++){
			fscanf_s(in,"%c",&bgi.text_desc[1][i]);
			if(bgi.text_desc[1][i]=='\n'){
				bgi.text_desc[1][i]=NULL;
				break;
			}
		}
	}
	if(bgi.line>1){
		for(i=0;;i++){
			fscanf_s(in,"%c",&bgi.text_desc[2][i]);
			if(bgi.text_desc[2][i]=='\n'){
				bgi.text_desc[2][i]=NULL;
				break;
			}
		}
	}
	fscanf_s(in,"%d %d\n",&bgi.bgnum,&bgi.bgtnum);

	if(bgi.bgtnum==1) bgi.img_textures = IMG_Load("image\\background\\texture\\bgtexture.png");
	if(bgi.bgtnum==2) bgi.img_textures = IMG_Load("image\\background\\texture\\bgtexture-blue.png");
	if(bgi.bgtnum==3) bgi.img_textures = IMG_Load("image\\background\\texture\\bgtexture-green.png");
	if(bgi.bgtnum==4) bgi.img_textures = IMG_Load("image\\background\\texture\\bgtexture-red.png");
	bgi.type = bgi.bgnum;

	bgi.img_maps = IMG_Load("image\\blank.png");

	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			temp = 0;
			fscanf_s(in,"%d ",&temp);
			if(temp == 0) // 앞배경 서피스가 하얀 부분
			{
				// 앞배경 서피스를 NULL
				*((Uint32 *)bgi.img_maps->pixels + i*bgi.img_maps->pitch/4 + j) = NULL;
				// 앞배경 텍스쳐 서피스의 부분을 지워버린다
				*((Uint32 *)bgi.img_textures->pixels + i*bgi.img_textures->pitch/4 + j) = NULL;
			}
			else // 앞배경 서피스가 까만부분
			{
				// 앞배경 서피스를 까만색으로
				*((Uint32 *)bgi.img_maps->pixels + i*bgi.img_maps->pitch/4 + j) = SDL_MapRGB(bgi.img_maps->format, 0, 0, 0);
			}
		}
	}
	fclose(in);

	// 완성 텍스쳐!
	bgi.img_texture = SDL_CreateTextureFromSurface(screen,bgi.img_textures);
	//그림자 효과
	bgi.img_shadow = SDL_CreateTextureFromSurface(screen,bgi.img_maps);
	SDL_SetTextureAlphaMod(bgi.img_shadow,128);
	SDL_SetTextureBlendMode(bgi.img_shadow,SDL_BLENDMODE_BLEND);

	// 캐릭터 정보 세팅
	Set_Char(screen);
}

void Stage::Set_Char(SDL_Renderer *screen){
	int i;
	cFire.img = load_image(screen,"image\\fire.png"); // 불
	for(i=0;i<8;i++){
		cFire.rect[i].x=i*60;
		cFire.rect[i].y=0;
		cFire.rect[i].w=60;
		cFire.rect[i].h=60;
	}
	cFire.img_eye = load_image(screen,"image\\fire_eye.png"); // 불 눈알
	for(i=0;i<2;i++){
		cFire.rect_eye[i].x=i*7;
		cFire.rect_eye[i].y=0;
		cFire.rect_eye[i].w=7;
		cFire.rect_eye[i].h=11;
	}
	cWater.img = load_image(screen,"image\\water.png"); // 물
	for(i=0;i<9;i++){
		cWater.rect[i].x=i*60;
		cWater.rect[i].y=0;
		cWater.rect[i].w=60;
		cWater.rect[i].h=60;
	}
	cWater.img_eye = load_image(screen,"image\\water_eye.png"); // 물 눈알
	for(i=0;i<2;i++){
		cWater.rect_eye[i].x=i*8;
		cWater.rect_eye[i].y=0;
		cWater.rect_eye[i].w=8;
		cWater.rect_eye[i].h=13;
	}
	cFire.ball.img = load_image(screen,"image\\fireball.png");
	cWater.ball.img = load_image(screen,"image\\waterball.png");

	for(i=0;i<4;i++){
		cFire.ball.clip[i].x = i*40;
		cFire.ball.clip[i].y = 0;
		cFire.ball.clip[i].w = 40;
		cFire.ball.clip[i].h = 40;
	}
	for(i=0;i<4;i++){
		cWater.ball.clip[i].x = i*40;
		cWater.ball.clip[i].y = 0;
		cWater.ball.clip[i].w = 40;
		cWater.ball.clip[i].h = 40;
	}
	
	cFire.dir = true;
	cFire.ac = 1.0;
	cFire.cImg = bgi.img_maps; // 충돌 체크용 흑백 이미지에 앞배경 서피스를 넣어준다
	cFire.name = false;
	cWater.ac = 1.0;
	cWater.dir = true;
	cWater.cImg = bgi.img_maps; // 충돌 체크용 흑백 이미지에 앞배경 서피스를 넣어준다
	cWater.name = true;

	// 파티클 셋팅
	cFire.ptc.Set(screen);
	cWater.ptc.Set(screen);
	SDL_SetTextureColorMod(cFire.ptc.img,255,127,39);
	SDL_SetTextureColorMod(cWater.ptc.img,0,200,255);
	SDL_SetTextureBlendMode(cFire.ptc.img,SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(cWater.ptc.img,SDL_BLENDMODE_ADD);

	// 투명도 셋팅
	cFire.alpha = cFire.talpha = 255;
	cWater.alpha = cWater.talpha = 255;
	if(cFire.x==0 && cFire.y==0) cFire.alpha = 0.0;
	if(cWater.x==0 && cWater.y==0) cWater.alpha = 0.0;
}

void Stage::Print(SDL_Renderer* screen){
	bgi.Print_Back(screen,true);
	bgi.Print_Shadow(screen);
	envt.Print(screen,true);
	bgi.Print_Texture(screen);

	cFire.Print(screen);
	cWater.Print(screen);

	bgi.Print_Endpoint(screen);

	apply_surface(400, 300, bgi.img_glow, screen, bgi.glow2, 0, false, true);
	bgi.glow2 = 1.20 + ((double)0.20)*sin(bgi.glow1/100);
	bgi.glow1++;
}

void Stage::Interact(FMOD_SYSTEM *g_System){
	int i, j;
	double temp=0.0;

	// 클리어한 캐릭터
	if(cFire.talpha!=0 && isCrash(cFire.x,cFire.y,bgi.end.x,bgi.end.y,75,75)){
		cFire.talpha = 0;
		cFire.ay=0;
		cFire.ax/=2;
	}
	if(cWater.talpha!=0 && isCrash(cWater.x,cWater.y,bgi.end.x,bgi.end.y,75,75)){
		cWater.talpha = 0;
		cWater.ay=0;
		cWater.ax/=2;
	}

	// 불공이 물(캐릭터)에 닿음
	if( isCrash2(cWater.x, cWater.y,(int)(60*cWater.scale),(int)(60*cWater.scale),cFire.ball.x, cFire.ball.y, 40, 40)  && cFire.ball.motion>0 ){
		cWater.tscale-=0.125;
		cFire.ball.motion = 0;
		for(j=0;j<10;j++) cFire.ptc.Add( 0 , cFire.ball.x , cFire.ball.y , 1.0 , (rand()%360)*PI/180 );
	}
	// 물공이 불(캐릭터)에 닿음
	if( isCrash2(cFire.x, cFire.y,(int)(60*cFire.scale),(int)(60*cFire.scale),cWater.ball.x, cWater.ball.y, 40, 40)  && cWater.ball.motion>0 ){
		cFire.tscale-=0.125;
		cWater.ball.motion = 0;
		for(j=0;j<10;j++) cWater.ptc.Add( 0 , cWater.ball.x , cWater.ball.y , 1.0 , (rand()%360)*PI/180 );
	}

	// 불과 물이 만나면...!!!
	if(cFire.talpha!=0 && cWater.talpha!=0){
		if( isCrash2(cFire.x,cFire.y,(int)(60*cFire.scale),(int)(60*cFire.scale),cWater.x,cWater.y,(int)(60*cWater.scale),(int)(60*cWater.scale)) ){
			cFire.tscale-=0.001;
			cWater.tscale-=0.001;
		}
	}

	for(i=1;i<=envt.n;i++){
		for(j=1;j<=envt.n;j++){
			if(i==j) continue;
			if(envt.e[i].type!=6 || envt.e[j].type!=6) continue;

			if( pow((double)(envt.e[i].x-envt.e[j].x),2) + pow((double)(envt.e[i].y-envt.e[j].y),2) <= 1024){
				j=-1;
				break;
			}
		}
		if(j!=-1) envt.e[i].crash = false;
	}
	for(i=1;i<=envt.n;i++){
		for(j=i+1;j<=envt.n;j++){
			if(envt.e[i].type!=6 || envt.e[j].type!=6) continue;

			if( pow((double)(envt.e[i].x-envt.e[j].x),2) + pow((double)(envt.e[i].y-envt.e[j].y),2) <= 1024){
				if(envt.e[i].ay==0 && envt.e[j].ay==0){
					temp = envt.e[i].ax;
					envt.e[i].ax = envt.e[j].ax;
					envt.e[j].ax = temp;
				}

					else if(envt.e[i].ay==0){
						envt.e[j].ay*=-0.5;
						if(envt.e[i].cx<envt.e[j].cx){
							envt.e[i].ax-=0.02;
							envt.e[j].ax+=0.02;
						}else{
							envt.e[i].ax+=0.02;
							envt.e[j].ax-=0.02;
						}
					}
					else if(envt.e[j].ay==0){
						envt.e[i].ay*=-0.5;
						if(envt.e[i].cx<envt.e[j].cx){
							envt.e[i].ax-=0.02;
							envt.e[j].ax+=0.02;
						}else{
							envt.e[i].ax+=0.02;
							envt.e[j].ax-=0.02;
						}
					}
					else{
						temp = envt.e[i].ay;
						envt.e[i].ay = envt.e[j].ay;
						envt.e[j].ay = temp;
					}

					if(envt.e[i].use==true || envt.e[j].use==true){
						envt.e[i].use=true;
						envt.e[j].use=true;
					}
					if(envt.e[i].whose==true || envt.e[j].whose==true){
						envt.e[i].whose=true;
						envt.e[j].whose=true;
					}

				envt.e[i].crash = true;
				envt.e[j].crash = true;
			}
		}
	}
	
	for(i=1;i<=envt.n;i++){
		// 환경이 없어지는 중이면 무시함
		if(envt.e[i].die > 0) continue;

		// 비
		if(envt.e[i].type==0){
			if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y)*cFire.cImg->pitch/4 + envt.e[i].x) ){
				envt.e[i].die = 1;
				continue;
			}
			for(j=1;j<=envt.n;j++){
				if(envt.e[j].type != 1) continue;
				// 불(환경)에 닿으면 불을 끈다.
				if( isCrash(envt.e[i].x, envt.e[i].y, envt.e[j].x, envt.e[j].y, 32, 32) && envt.e[j].motion>300){
					envt.e[j].die = 1;
					break;
				}
			}
		}
		// 벼락
		if(envt.e[i].type==9){
			if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y)*cFire.cImg->pitch/4 + envt.e[i].x) ){
				envt.e[i].die = 1;
				// 불
				envt.n++;
				envt.e[envt.n].type = 1;
				envt.e[envt.n].x = envt.e[i].x;
				envt.e[envt.n].y = envt.e[i].y-18;
				envt.e[envt.n].die = 0; envt.e[envt.n].motion = 0;
				continue;
			}
			for(j=1;j<=envt.n;j++){
				if(envt.e[j].type != 2 && envt.e[j].type != 3) continue;
				// 물(환경), 나무에 닿으면 없앤다.
				if( isCrash(envt.e[i].x, envt.e[i].y, envt.e[j].x, envt.e[j].y, 32, 32) ){
					envt.e[i].die = envt.e[j].die = 1;
					envt.e[j].rainfire = true; // 불도 물도 크기가 변하지 말아라!
					break;
				}
			}
		}
		// 구름
		if(envt.e[i].type==4 && envt.e[i].motion>=300){
			for(j=1;j<=envt.n;j++){
				// 반대편에서 오는 구름이 아니면 무시
				if(envt.e[j].type != 5) continue;
				// 만들어 지고 있는 구름이면 무시
				if(envt.e[j].motion < 300){
					continue;
				}
				// 구름에 닿으면... 번쩍!
				if( isCrash(envt.e[i].x+40, envt.e[i].y, envt.e[j].x, envt.e[j].y, 100, 60) ){
					Sound_Play(&g_System,&envt.channel[9],&envt.sound[9]);
					// 구름 방향 전환
					envt.e[i].type=5; envt.e[j].type=4;
					// 번쩍 255
					bgi.lp = 255;
					bgi.lpow = 20;
					// 벼락
					envt.n++;
					envt.e[envt.n].type = 9;
					envt.e[envt.n].x = envt.e[i].x+40;
					envt.e[envt.n].y = envt.e[i].y;
					envt.e[envt.n].die = 0;
					break;
				}
			}
		}
		// 돌멩이
		if(envt.e[i].type==6){
			// 오른쪽 벽에 가로막힘
			if((( SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y)*cFire.cImg->pitch/4 + envt.e[i].x+16) || SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y-14)*cFire.cImg->pitch/4 + envt.e[i].x+16) )) && envt.e[i].ax>0){
				envt.e[i].use = true;
				envt.e[i].ax=0;
				// 불이 오른쪽으로 밀었다면
				if( isCrash2_r(envt.e[i].x, envt.e[i].y,30,30, cFire.x, cFire.y, (int)(60*cFire.scale), (int)(100*cFire.scale)) && cFire.onRock==false && cFire.ay<-100){
					cFire.ax=-0.5;
				}
				// 물이 오른쪽으로 밀었다면
				if( isCrash2_r(envt.e[i].x, envt.e[i].y,30,30, cWater.x, cWater.y, (int)(60*cWater.scale), (int)(100*cWater.scale)) && cWater.onRock==false && cWater.ay<-100){
					cWater.ax=-0.5;
				}
				continue;
			}
			// 왼쪽 벽에 가로막힘
			else if((( SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y)*cFire.cImg->pitch/4 + envt.e[i].x-16) || SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y-14)*cFire.cImg->pitch/4 + envt.e[i].x-16) )) && envt.e[i].ax<0){
				envt.e[i].use = true;
				envt.e[i].ax=0;
				// 불이 왼쪽으로 밀었다면
				if( isCrash2_l(envt.e[i].x, envt.e[i].y,30,30, cFire.x, cFire.y, (int)(60*cFire.scale), (int)(100*cFire.scale)) && cFire.onRock==false && cFire.ay<-100){
					cFire.ax=+0.5;
				}
				// 물이 왼쪽으로 밀었다면
				if( isCrash2_l(envt.e[i].x, envt.e[i].y,30,30, cWater.x, cWater.y, (int)(60*cWater.scale), (int)(100*cWater.scale)) && cWater.onRock==false && cWater.ay<-100){
					cWater.ax=+0.5;
				}
				continue;
			}
			else if(envt.e[i].crash==false) envt.e[i].use=false;

			// 흔적 지우기
			Password486(i,false);

			// 실수좌표를 정수좌표로
			envt.e[i].x = (int)envt.e[i].cx;
			envt.e[i].y = (int)envt.e[i].cy;

			// 돌 위에 설 수 있게 흔적 남기기
			Password486(i,true);

			// 중력
			if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+16)*cFire.cImg->pitch/4 + envt.e[i].x) || SDL_MapRGB(cFire.cImg->format, 4, 8, 6) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+16)*cFire.cImg->pitch/4 + envt.e[i].x)){
				if(abs(envt.e[i].ay)<=0.1){
					envt.e[i].ay = 0;
					if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+15)*cFire.cImg->pitch/4 + envt.e[i].x) || SDL_MapRGB(cFire.cImg->format, 4, 8, 6) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+15)*cFire.cImg->pitch/4 + envt.e[i].x)){
						envt.e[i].cy--;
						envt.e[i].angle+=PI*1.414;
					}
				}
				if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+16)*cFire.cImg->pitch/4 + envt.e[i].x)){
					envt.e[i].ay *= -0.5;
					envt.e[i].cy += envt.e[i].ay;
				}
			}
			else{
				if(envt.e[i].ay == 0) envt.e[i].ay = 0.02;
			}

			// 경사
			if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+13)*cFire.cImg->pitch/4 + (envt.e[i].x+11)) && envt.e[i].whose==false && envt.e[i].crash==false){
				envt.e[i].ax-=0.05;
				envt.e[i].use=false;
			}
			if(SDL_MapRGB(cFire.cImg->format, 0, 0, 0) == *((Uint32 *)cFire.cImg->pixels + (envt.e[i].y+13)*cFire.cImg->pitch/4 + (envt.e[i].x-11)) && envt.e[i].whose==false && envt.e[i].crash==false){
				envt.e[i].ax+=0.05;
				envt.e[i].use=false;
			}

			// 밀기
			if(cFire.ax>=0){
				// 오른쪽으로 밀었다면
				if( isCrash2_r(envt.e[i].x, envt.e[i].y,30,30, cFire.x, cFire.y, (int)(60*cFire.scale), (int)(100*cFire.scale)) && cFire.onRock==false && cFire.ay<-100){
					if(envt.e[i].use==true){
						cFire.ax=-0.1;
						continue;
					}
					// 돌에 낑긴 상태가 아니면
					if(abs(envt.e[i].cx - (cFire.x + (30*cFire.scale) + 17)) <= 5){
						// 돌을 민다
						envt.e[i].cx = cFire.x + (30*cFire.scale) + 14;
						// 돌을 미는 중임
						envt.e[i].whose = true;
						// 돌에 가속도를 준다
						envt.e[i].ax = cFire.ax;
					}
					// 돌에 낑겼으면
					else{
						// 낑긴 위치에 따라 돌을 옮겨준다.
						if(cFire.x<envt.e[i].x){
							envt.e[i].ax=0.5;
							//envt.e[i].cx+=0.5;
							envt.e[i].angle += PI*0.5;
							cFire.ax-=0.25;
						}
						else{
							envt.e[i].ax=-0.5;
							//envt.e[i].cx-=0.5;
							envt.e[i].angle -= PI*0.5;
							cFire.ax+=0.25;
						}
					}
				}
				// 돌을 미는 중이 아님
				else envt.e[i].whose = false;
			}
			if(cFire.ax<=0){
				// 왼쪽으로 밀었다면
				if( isCrash2_l(envt.e[i].x, envt.e[i].y,30,30, cFire.x, cFire.y, (int)(60*cFire.scale), (int)(100*cFire.scale)) && cFire.onRock==false && cFire.ay<-100){
					if(envt.e[i].use==true){
						cFire.ax=+0.1;
						continue;
					}
					// 돌에 낑긴 상태가 아니면
					if(abs(envt.e[i].cx - (cFire.x - (30*cFire.scale) - 17)) <= 5){
						// 돌을 민다
						envt.e[i].cx = cFire.x - (30*cFire.scale) - 14;
						// 돌을 미는 중임
						envt.e[i].whose = true;
						// 돌에 가속도를 준다
						envt.e[i].ax = cFire.ax;
					}
					// 돌에 낑겼으면
					else{
						// 낑긴 위치에 따라 돌을 옮겨준다.
						if(cFire.x<envt.e[i].x){
							envt.e[i].ax=0.5;
							envt.e[i].angle += PI*0.5;
							cFire.ax-=0.25;
						}
						else{
							envt.e[i].ax=-0.5;
							envt.e[i].angle -= PI*0.5;
							cFire.ax+=0.25;
						}
					}
				}
				// 돌을 미는 중이 아님
				else envt.e[i].whose = false;
			}

			// 밀기
			if(cWater.ax>=0){
				// 오른쪽으로 밀었다면
				if( isCrash2_r(envt.e[i].x, envt.e[i].y,30,30, cWater.x, cWater.y, (int)(60*cWater.scale), (int)(100*cWater.scale)) && cWater.onRock==false && cWater.ay<-100){
					if(envt.e[i].use==true){
						cWater.ax=-0.1;
						continue;
					}
					// 돌에 낑긴 상태가 아니면
					if(abs(envt.e[i].cx - (cWater.x + (30*cWater.scale) + 17)) <= 5){
						// 돌을 민다
						envt.e[i].cx = cWater.x + (30*cWater.scale) + 14;
						// 돌을 미는 중임
						envt.e[i].whose = true;
						// 돌에 가속도를 준다
						envt.e[i].ax = cWater.ax;
					}
					// 돌에 낑겼으면
					else{
						// 낑긴 위치에 따라 돌을 옮겨준다.
						if(cWater.x<envt.e[i].x){
							envt.e[i].ax=0.5;
							//envt.e[i].cx+=0.5;
							envt.e[i].angle += PI*0.5;
							cWater.ax-=0.25;
						}
						else{
							envt.e[i].ax=-0.5;
							//envt.e[i].cx-=0.5;
							envt.e[i].angle -= PI*0.5;
							cWater.ax+=0.25;
						}
					}
				}
				// 돌을 미는 중이 아님
				else envt.e[i].whose = false;
			}
			if(cWater.ax<=0){
				// 왼쪽으로 밀었다면
				if( isCrash2_l(envt.e[i].x, envt.e[i].y,30,30, cWater.x, cWater.y, (int)(60*cWater.scale), (int)(100*cWater.scale)) && cWater.onRock==false && cWater.ay<-100){
					if(envt.e[i].use==true){
						cWater.ax=+0.1;
						continue;
					}
					// 돌에 낑긴 상태가 아니면
					if(abs(envt.e[i].cx - (cWater.x - (30*cWater.scale) - 17)) <= 5){
						// 돌을 민다
						envt.e[i].cx = cWater.x - (30*cWater.scale) - 14;
						// 돌을 미는 중임
						envt.e[i].whose = true;
						// 돌에 가속도를 준다
						envt.e[i].ax = cWater.ax;
					}
					// 돌에 낑겼으면
					else{
						// 낑긴 위치에 따라 돌을 옮겨준다.
						if(cWater.x<envt.e[i].x){
							envt.e[i].ax=0.5;
							envt.e[i].angle += PI*0.5;
							cWater.ax-=0.25;
						}
						else{
							envt.e[i].ax=-0.5;
							envt.e[i].angle -= PI*0.5;
							cWater.ax+=0.25;
						}
					}
				}
				// 돌을 미는 중이 아님
				else envt.e[i].whose = false;
			}
			//continue;
		}

		// 불이 환경에 닿음
		if( isCrash(envt.e[i].x, envt.e[i].y, cFire.x, cFire.y, (int)(60*cFire.scale), (int)(100*cFire.scale)) ){
			if(envt.e[i].type==3) Sound_Play(&g_System,&envt.channel[3],&envt.sound[3]);
			envt.e[i].die = 1;
			envt.e[i].whose = false;

			if(envt.e[i].type==1) cFire.tscale += 0.25; // 불
			if(envt.e[i].type==2) cFire.tscale -= 0.125; // 물
			if(envt.e[i].type==0) cFire.tscale -= 0.01; // 비

			if(envt.e[i].type==4 || envt.e[i].type==5 || envt.e[i].type==6) envt.e[i].die = 0; // 불이 구름이나 돌에 닿으면 내비 둬
			if(envt.e[i].type==0) envt.e[i].die = 30; // 비가 불에 닿으면 바로 없어짐
			continue;
		}
		// 물이 환경에 닿음
		if( isCrash(envt.e[i].x, envt.e[i].y, cWater.x, cWater.y, (int)(60*cWater.scale), (int)(100*cWater.scale)) ){
			envt.e[i].die = 1;
			envt.e[i].whose = true;

			if(envt.e[i].type==1) cWater.tscale -= 0.25;
			if(envt.e[i].type==2) cWater.tscale += 0.125;
			if(envt.e[i].type==0) cWater.tscale += 0.01;

			if(envt.e[i].type==3 || envt.e[i].type==4 || envt.e[i].type==5 || envt.e[i].type==6) envt.e[i].die = 0; // 물이 나무나 구름이나 돌에 닿으면 내비 둬
			if(envt.e[i].type==0) envt.e[i].die = 30; // 비가 물에 닿으면 바로 없어짐
			continue;
		}
		// 불공이 환경에 닿음
		if( isCrash2(envt.e[i].x, envt.e[i].y, envt.e[i].w, envt.e[i].h, cFire.ball.x, cFire.ball.y, 40, 40)  && cFire.ball.motion>0 ){
			// 나무 또는 돌에 닿으면
			if(envt.e[i].type==3 || envt.e[i].type==6){
				if(envt.e[i].type==3){ // 나무에 닿으면 치지직
					Sound_Play(&g_System,&envt.channel[3],&envt.sound[3]);
					envt.e[i].die = 1;
				}
				if(envt.e[i].type==6){
					if(cFire.ball.dir) envt.e[i].ax = 1;
					else envt.e[i].ax = -1;
				}
				cFire.ball.motion = 0;
				for(j=0;j<10;j++) cFire.ptc.Add( 0 , cFire.ball.x , cFire.ball.y , 1.0 , (rand()%360)*PI/180 );
			}
		}
		// 불공이 물 위를 스침
		if( (isCrash(envt.e[i].x, envt.e[i].y, cFire.ball.x, cFire.ball.y+20, 40, 40) || isCrash(envt.e[i].x, envt.e[i].y, cFire.ball.x, cFire.ball.y+40, 40, 40)) && cFire.ball.motion>0 ){
			if(envt.e[i].type==2){
				cFire.ball.motion=0;
				for(j=0;j<10;j++) cFire.ptc.Add( 0 , cFire.ball.x , cFire.ball.y , 1.0, (rand()%360)*PI/180 );
				// 구름 생성~
				envt.n++;
				if(cFire.ball.dir==0) envt.e[envt.n].type = 5;
				if(cFire.ball.dir==1) envt.e[envt.n].type = 4;
				envt.e[envt.n].x = cFire.ball.x;
				envt.e[envt.n].y = cFire.ball.y;
				envt.e[envt.n].cx = envt.e[i].x; envt.e[envt.n].cy = envt.e[envt.n].y;	envt.e[envt.n].oy = envt.e[envt.n].y; envt.e[envt.n].die = 0;	envt.e[envt.n].motion = 0;
			}
		}
		// 물공이 환경에 닿음
		if( isCrash2(envt.e[i].x, envt.e[i].y, envt.e[i].w, envt.e[i].h, cWater.ball.x, cWater.ball.y, 50, 50) && cWater.ball.motion>0 ){
			// 불 또는 나무 또는 돌에 닿으면
			if(envt.e[i].type==1 || envt.e[i].type==3 || envt.e[i].type==6){
				if(envt.e[i].type==1) envt.e[i].die = 1;
				if(envt.e[i].type==6){
					if(cWater.ball.dir) envt.e[i].ax = 1;
					else envt.e[i].ax = -1;
				}
				cWater.ball.motion = 0;
				for(j=0;j<10;j++) cWater.ptc.Add( 0 , cWater.ball.x , cWater.ball.y , 1.0, (rand()%360)*PI/180 );
			}
		}

		// 물은 한번에 없어져야지
		if(envt.e[i].type==2){
			for(j=1;j<=envt.n;j++){
				// 없어지는 물이 있으면
				if(envt.e[j].type == 2 && envt.e[j].die > 0){
					// 그것도 근처에
					if( abs(envt.e[i].x-envt.e[j].x) <= 35 && abs(envt.e[i].y-envt.e[j].y) <= 35 ){
						envt.e[i].die = 1;
						// 벼락이 물을 껐으면
						if(envt.e[j].rainfire){
							envt.e[i].rainfire = envt.e[j].rainfire;
						}
						// 아니면 캐릭터 크기 변화
						else{
							envt.e[i].whose = envt.e[j].whose;
							if(envt.e[i].whose)	cWater.tscale += 0.125;
							else cFire.tscale -= 0.125;
						}
					}
				}
			}
		}
	}
}

void Stage::Password486(int i, bool write){
	int j;
	if(write==false){
		for(j=180;j<360;j++){
			if( *((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18))*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) == SDL_MapRGB(cFire.cImg->format, 4, 8, 6) ){
				*((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18))*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) = NULL;
			}
			if( *((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18)+1)*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) == SDL_MapRGB(cFire.cImg->format, 4, 8, 6) ){
				*((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18)+1)*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) = NULL;
			}
		}
	}
	else{
		for(j=180;j<360;j++){
			if( *((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18))*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) == NULL ){
				*((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18))*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) = SDL_MapRGB(cFire.cImg->format, 4, 8, 6);
			}
			if( *((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18)+1)*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) == NULL ){
				*((Uint32 *)cFire.cImg->pixels + ((int)(envt.e[i].y + sin(j*PI/180) * 18)+1)*cFire.cImg->pitch/4 + (int)(envt.e[i].x + cos(j*PI/180) * 18)) = SDL_MapRGB(cFire.cImg->format, 4, 8, 6);
			}
		}
	}
}

void Stage::Destroy(){
	int i;
	bgi.Destroy();

	for(i=0;i<10;i++){
		SDL_DestroyTexture(envt.img[i]);
	}

	SDL_DestroyTexture(cFire.img);
	SDL_DestroyTexture(cFire.img_eye);
	SDL_DestroyTexture(cWater.img);
	SDL_DestroyTexture(cWater.img_eye);
	SDL_DestroyTexture(cFire.ball.img);
	SDL_DestroyTexture(cWater.ball.img);
	cFire.ptc.Destroy();
	cWater.ptc.Destroy();
}