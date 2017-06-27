
#pragma once
#include "_stdafx.h"

int trophy;
Menu menu;

void tEnding(SDL_Renderer *screen, Stage *stg){
	bool quit = false;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();
	double alpha=0.0, alpha2=0.0, alpha3=0.0;
	double y=700.0;
	int i, term = 0;

	Font font0, font1, font2;
	font0.Set(screen,"spring.ttf",60);
	font1.Set(screen,"spring.ttf",40);
	font2.Set(screen,"spring.ttf",30);
	
	SDL_Texture *img = load_image(screen,"image\\black.png");
	SDL_SetTextureBlendMode(img,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img,(Uint8)alpha);

	int wh = 600, num = 0;
	SDL_Texture *img_cut = load_image(screen,"image\\cut.png");
	SDL_SetTextureBlendMode(img_cut,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img_cut,0);

	SDL_Rect clip[4];
	for(i=0;i<4;i++){
		clip[i].x = i*300;
		clip[i].y = 0;
		clip[i].w = 300;
		clip[i].h = 250;
	}

	while(!quit){
		curTicks += Set_Frame(curTicks);

		if(alpha<255 && term==0){
			(*stg).envt.Action();
			GetKey(&(*stg).cFire,false,false,false,true);
			(*stg).Print(screen);
			alpha+=0.5;
			(*stg).cFire.Move(32);
			// ���Ҹ� ����
			FMOD_Channel_SetVolume((*stg).envt.channel[0], (float)(1.0-alpha/255));
			FMOD_Channel_SetVolume((*stg).channel, (float)(1.0-alpha/255));
		}else{
			term++;
			if(term==450){
				for(i=0;i<5;i++) (*stg).cWater.ptc.Add(1,436,180,0.5,rand()%360*PI/180);
			}
			if(term==800) (*stg).cWater.ptc.Add(1,436,180,0.5,70*PI/180);
			if(term==850) (*stg).cWater.ptc.Add(1,440,185,0.5,100*PI/180);
			if(term==890) (*stg).cWater.ptc.Add(1,444,195,0.5,80*PI/180);
			if(term==920) (*stg).cWater.ptc.Add(1,448,210,0.5,90*PI/180);
			if(term==940) (*stg).cWater.ptc.Add(1,452,230,0.5,95*PI/180);
			if(term==960) (*stg).cWater.ptc.Add(1,456,255,0.5,75*PI/180);
			if(term==970) (*stg).cWater.ptc.Add(1,456,280,0.5,90*PI/180);
			if(term==980) (*stg).cWater.ptc.Add(1,456,305,0.5,90*PI/180);
			if(term==990) (*stg).cWater.ptc.Add(1,456,330,0.5,90*PI/180);
			if(term==1000) (*stg).cWater.ptc.Add(1,456,355,0.5,90*PI/180);
			if(term==1300){
				alpha=0.0;
				(*stg).cFire.ax=0.0;
				(*stg).cWater.x = 470;
				(*stg).cWater.y = 365;
				(*stg).cWater.scale = (*stg).cWater.tscale = 0.5;
				(*stg).cWater.alpha = (*stg).cWater.talpha = 255;
				(*stg).cWater.dir=false;
				break;
			}
		}

		SDL_SetTextureAlphaMod(img,(Uint8)alpha);
		apply_surface(0,0,img,screen,1.0,0.0,false,false);

		if(term>=200){
			(*stg).cWater.ptc.Print(screen);
		}

		SDL_RenderPresent(screen);

		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_MOUSEMOTION){
				printf("%d %d\n",event.motion.x,event.motion.y);
			}
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}

	font0.Destroy();
	font1.Destroy();
	font2.Destroy();
	SDL_DestroyTexture(img);
	SDL_DestroyTexture(img_cut);
	return;
}

void nEnding(SDL_Renderer *screen, Stage* stg, bool real){
	bool quit = false;
	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();
	double alpha=0.0, alpha2=0.0, alpha3=0.0;
	double y=700.0;
	int i, term = 0;

	Font font0, font1, font2;
	font0.Set(screen,"spring.ttf",60);
	font1.Set(screen,"spring.ttf",40);
	font2.Set(screen,"spring.ttf",30);
	
	SDL_Texture *img = load_image(screen,"image\\black.png");
	SDL_SetTextureBlendMode(img,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img,(Uint8)alpha);

	int wh = 600, num = 0;
	SDL_Texture *img_cut = load_image(screen,"image\\cut.png");
	SDL_SetTextureBlendMode(img_cut,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img_cut,0);

	SDL_Rect clip[8];
	for(i=0;i<8;i++){
		clip[i].x = i*300;
		clip[i].y = 0;
		clip[i].w = 300;
		clip[i].h = 250;
	}
	if(real) num=4;

	while(!quit){
		curTicks += Set_Frame(curTicks);

		// ȭ�� ������
		if(alpha<255){
			(*stg).envt.Action();
			(*stg).Print(screen);
			alpha+=0.5;
		}
		else{
			// �ణ ��ٷȴٰ�
			term++;
			if(term>=200){
				// �� ���� ��¼�� ���
				if(alpha2<255 && y==700) alpha2+=0.5;
				else{
					// ���� �ö�
					// ��ŵ
					if(GetAsyncKeyState(VK_ESCAPE) < 0) {
						y-=2.0;
					}
					y-=0.5;
					// �� ���� ��¼�� ���ֱ�
					if(alpha2>0) alpha2-=1.0;
				}
			}
		}
		SDL_SetTextureAlphaMod(img,(Uint8)alpha);
		apply_surface(0,0,img,screen,1.0,0.0,false,false);

		if(y>0){
			font0.PrintAlpha2(400,300,"�� ���� ���ɿ��� �ູ��",255,255,255,(Uint8)alpha2);
		}

		font1.Print(200,(int)y,"��ȹ/����/�׸�",255,255,255);
		font2.Print(50,(int)y+100,"�ѱ������й̵�����б�",255,255,255,false);
		font2.Print(50,(int)y+150,"���Ƹ� '������'",255,255,255,false);
		font2.Print(50,(int)y+200,"�ڹ���",255,255,255,false);

		font1.Print(600,(int)y+510,"�����",255,255,255);
		font2.Print(450,(int)y+610,"������� ��� 1~6����",255,255,255,false);
		font2.Print(450,(int)y+660,"��Tension 01",255,255,255,false);
		font2.Print(450,(int)y+710,"��Delight 01",255,255,255,false);
		font2.Print(450,(int)y+760,"�ܻ���� �λ� Remake2",255,255,255,false);
		font2.Print(450,(int)y+830,"������ - SoundHolicK",255,255,255,false);
		font2.Print(450,(int)y+880,"http://sondholick.com",255,255,255,false);
		
		font1.Print(200,(int)y+1190,"ȿ����",255,255,255);
		font2.Print(50,(int)y+1290,"http://freesound.org",255,255,255,false);
		font2.Print(50,(int)y+1340,"http://soundbible.com",255,255,255,false);

		font1.Print(600,(int)y+1650,"���",255,255,255);
		font2.Print(450,(int)y+1750,"SDL 2.0",255,255,255,false);
		font2.Print(620,(int)y+1750,"MSDEV 2010",255,255,255,false);
		font2.Print(450,(int)y+1800,"FMOD_ex",255,255,255,false);
		font2.Print(620,(int)y+1800,"GoldWave",255,255,255,false);
		font2.Print(450,(int)y+1850,"MSPainter",255,255,255,false);
		font2.Print(620,(int)y+1850,"Sai Tools",255,255,255,false);
		font2.Print(450,(int)y+1900,"Photoshop CS",255,255,255,false);
		
		if(real){
			font2.Print(400,(int)y+2210,"����� ������",255,255,255);
			font2.Print(400,(int)y+2260,"����",255,255,255);
			font2.Print(400,(int)y+2360,"�� �ڴ�� ���̹�",255,255,255);
			font2.Print(400,(int)y+2410,"�ڹμ�",255,255,255);
			font2.Print(400,(int)y+2510,"��� ����",255,255,255);
			font2.Print(400,(int)y+2560,"����ö",255,255,255);
			font2.Print(400,(int)y+2660,"�� ���� �׽���",255,255,255);
			font2.Print(400,(int)y+2710,"���ܿ�",255,255,255);
			font2.Print(400,(int)y+2810,"��3���� ������",255,255,255);
			font2.Print(400,(int)y+2860,"������ ������",255,255,255);
			font2.Print(400,(int)y+2960,"��̰� �ְ� ���Ƹ�",255,255,255);
			font2.Print(400,(int)y+3010,"������ �ϵ�",255,255,255);
			font2.Print(400,(int)y+3210,"�׸���",255,255,255);
			font2.Print(400,(int)y+3410,"���",255,255,255);
			if(y+3610<300){
				if((int)y+3565>=0){
					font2.PrintAlpha2(400,300,"�����մϴ�.",255,255,255,(int)y+3565);
				}
			}
			else font2.Print(400,(int)y+3610,"�����մϴ�.",255,255,255);
		}

		if(y>=-1773 && y<=520){
			if(y>=165 && y<=420){
				SDL_SetTextureAlphaMod(img_cut,255-(y-165));
			}else if(y>=-91 && y<=164){
				SDL_SetTextureAlphaMod(img_cut,y+91);
			}if(y>=-347 && y<=-92){
				num = 1; wh = 200;
				if(real) num=5;
				SDL_SetTextureAlphaMod(img_cut,255-(y+347));
			}else if(y>=-603 && y<=-348){
				SDL_SetTextureAlphaMod(img_cut,y+603);
			}if(y>=-1005 && y<=-750){
				num = 2; wh = 600;
				if(real) num=6;
				SDL_SetTextureAlphaMod(img_cut,255-(y+1005));
			}else if(y>=-1261 && y<=-1006){
				SDL_SetTextureAlphaMod(img_cut,y+1261);
			}if(y>=-1517 && y<=-1262){
				num = 3; wh = 200;
				if(real) num=7;
				SDL_SetTextureAlphaMod(img_cut,255-(y+1517));
			}else if(y>=-1773 && y<=-1518){
				SDL_SetTextureAlphaMod(img_cut,y+1773);
			}
			apply_surface(wh,300,img_cut,screen,1.0,0.0,false,true,&clip[num]);
		}

		// ������
		if(real){
			if(y<=-3700){
				font1.PrintAlpha2(400,300,"��",255,255,255,(Uint8)alpha3);
				if(y<=-3955){
					alpha3-=0.5;
					FMOD_Channel_SetVolume((*stg).channel,(float)(alpha3/255));
				}
				else alpha3+=0.5;
				if(alpha3<0) break;
			}
		}
		else{
			if(y<=-1900){
				font1.PrintAlpha2(400,300,"���� ���߸��� ���� �����ϴ�.",255,255,255,(Uint8)alpha3);
				if(y<=-2155){
					alpha3-=0.5;
					FMOD_Channel_SetVolume((*stg).channel,(float)(alpha3/255));
					FMOD_Channel_SetVolume((*stg).envt.channel[0],(float)(alpha3/255));
				}
				else alpha3+=0.5;
				if(alpha3<0) break;
			}
		}

		SDL_RenderPresent(screen);

		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT){// || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}

	font0.Destroy();
	font1.Destroy();
	font2.Destroy();
	SDL_DestroyTexture(img);
	SDL_DestroyTexture(img_cut);
	return;
}

bool Clear(SDL_Renderer *screen, Stage *stg, Cursor cCursor, int stagenum, int ttime){
	
	trophy = Database_trophy(stagenum,ttime);

	// ������ ����
	if(stagenum>=31){
		if(stagenum==31) nEnding(screen, stg, false);
		else if(stagenum==32) nEnding(screen, stg, true);
		return true;
	}

	int nexttime = Database_nexttrophy(stagenum,trophy);
	char text2[10]={"00:00"};
	if(nexttime==0){
		strcpy_s(text2,"��ȣ����!");
	}else{
		text2[1]=nexttime/60+'0';
		text2[3]=(nexttime%60)/10+'0';
		text2[4]=(nexttime%60)%10+'0';
	}

	bool quit = false, cancel = false;
	double scale = 0.0, motion = 0.0, motion2 = 0.0;
	int minute=0, second=0;
	char text[10]={"00:00"};
	Uint8 RGB[2]={0};

	SDL_Event event;
	Uint32 curTicks = SDL_GetTicks();

	Font font_t, font_d, font_b;
	font_t.Set(screen,"spring.ttf",30);
	font_d.Set(screen,"spring.ttf",22);
	font_b.Set(screen,"spring.ttf",25);

	SDL_Texture *img_trophy = load_image(screen,"image\\bronze.png");
	if(trophy==3){
		img_trophy = load_image(screen,"image\\gold.png");
	}
	else if(trophy==2){
		img_trophy = load_image(screen,"image\\silver.png");
	}

	while( !quit ){
		curTicks += Set_Frame(curTicks);

		(*stg).Print(screen);

		// ���� ��
		if(motion/50 < PI/2){
			motion++;
			scale = sin(motion/50);
			apply_surface(400,300,menu.img,screen,scale,90,false,true);
		}
		// �� ��
		else{
			if(ttime!=0 || motion2/40<PI/2)
				motion2++;

			if((int)motion2%5==0 && ttime!=0){
				// �ɸ� �ð� ++
				if(minute*60+second<ttime){
					second++;
					if(second>=60){
						second=0;
						minute++;
						text[1]=minute%10+'0';
						text[0]=minute/10+'0';
					}
					text[4]=second%10+'0';
					text[3]=second/10+'0';
				}
				// Ʈ���� Ű���
				else if(minute*60+second==ttime){
					ttime=0;
					motion2=0;
				}
			}
			apply_surface(400,300,menu.img,screen,scale,90,false,true);
			if(ttime==0) apply_surface(345,280,img_trophy,screen,sin(motion2/40),0.0,false,true);
			font_t.Print(450,220,"���� ����",0,0,0);
			font_d.Print(450,260,"�ɸ� �ð�",0,0,0);
			font_d.Print(450,290,text,0,0,0);
			font_d.Print(450,320,"���� �ܰ����",0,0,0);
			font_d.Print(450,350,text2,0,0,0);
			RGB[0]=RGB[1]=0;
			if(isCrash(cCursor.info.x,cCursor.info.y,345,385,100,25)) RGB[0]=255;
			font_b.Print(340,385,"�ٽ� �ϱ�",RGB[0],RGB[0],RGB[0]);
			if(isCrash(cCursor.info.x,cCursor.info.y,480,385,90,25)) RGB[1]=255;
			font_b.Print(480,385,"������",RGB[1],RGB[1],RGB[1]);
		}

		cCursor.Print();

		SDL_RenderPresent(screen);

		// ���α׷��� �̺�Ʈ�� ���� ��
		if( SDL_PollEvent( &event ) )
		{
			// ���콺 ������ ����
			if( event.type == SDL_MOUSEMOTION ){
				// ���콺 ��ǥ ����
				cCursor.info.x = event.motion.x;
				cCursor.info.y = event.motion.y;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ){
				if(RGB[0]==255){
					font_t.Destroy();font_d.Destroy();font_b.Destroy();SDL_DestroyTexture(img_trophy);
					return false;
				}
				if(RGB[1]==255){
					quit = true;
				}
			}
			if( event.type == SDL_QUIT || GetAsyncKeyState(VK_ESCAPE) < 0) {
				quit = true;
			}
		}
	}
	
	font_t.Destroy();
	font_d.Destroy();
	font_b.Destroy();
	SDL_DestroyTexture(img_trophy);
	return true;
}

void Print_Time(SDL_Renderer *screen, Font font, SDL_Texture *img_time, int minute, int second){
	apply_surface(400,580,img_time,screen,1.0,0.0,false,true);

	char text[10]={"00:00"};
	text[4]=(second%10)+'0';
	text[3]=(second/10)+'0';
	text[1]=(minute%10)+'0';
	text[0]=(minute/10)+'0';

	font.Print(365,570,text,0,0,0,false);
}

int Mode_Stage(SDL_Window *window, SDL_Renderer *screen, FMOD_SYSTEM *g_System, Stage stg, Cursor cCursor, int stagenum, float *volume, float *volume_e, bool create)
{
	bool quit = false;
	SDL_Event event;
	Uint16 alpha = 455;
	Uint32 curTicks = SDL_GetTicks();
	int i, temp = 0;
	int et, st, sst=0;
	int minute = 0, second = 0;

	Ai ai;
	ai.Load(stagenum); //�ΰ�����

	Font font_title, font_desc;
	font_title.Set(screen,"spring.ttf",50);
	font_desc.Set(screen,"spring.ttf",30);

	Font font_time;
	font_time.Set(screen,"spring.ttf",25);
	SDL_Texture *img_time = load_image(screen,"image\\time.png");

	SDL_Texture *img_backf = load_image(screen,"image\\glow1.png");
	SDL_SetTextureBlendMode(img_backf,SDL_BLENDMODE_ADD);

	stg.bgi.alpha = 0;

	menu.Set(400,screen);
	
	// ����� �Ҹ� ����
	FMOD_Channel_SetVolume(stg.channel, (*volume));
	// ȯ�� ���� �÷��� �� �Ҹ� ����
	for(i=0;i<=6;i++){
		if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
		Sound_Play(&g_System,&stg.envt.channel[i],&stg.envt.sound[i]);
		stg.envt.volume[i] = 0.0;
		FMOD_Channel_SetVolume(stg.envt.channel[i], (*volume_e));
	}
	FMOD_Channel_SetVolume(stg.envt.channel[3], (*volume_e));
	FMOD_Channel_SetVolume(stg.envt.channel[9], (*volume_e));
	
	st = clock();
	quit = false;
	ai.Set();
	while( !quit ){
		et = clock();
		curTicks += Set_Frame(curTicks);

		if(stg.cFire.x==210 && stagenum==32){
			tEnding(screen,&stg);
			Sound_Stop(&stg.channel);
		}
		if(stg.cFire.x==470 && stagenum==32){
			Sound_Play(&g_System,&stg.channel,&stg.sound[8]);
		}

		// Ŭ����!!~@
		if((stg.cFire.alpha==0 && stg.cWater.alpha==0) || (stg.cFire.alpha==0 && stagenum==28)){
			if(Clear(screen,&stg,cCursor,stagenum,minute*60+second)){
				for(i=0;i<=6;i++){
					if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
					FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
				}
				// ������ ���� �����
				for(i=1;i<=stg.envt.n;i++){
					if(stg.envt.e[i].type==6)
						stg.Password486(i,false);
				}
				menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
				return trophy;
			}
			else{
				for(i=0;i<=6;i++){
					if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
					FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
				}
				// ������ ���� �����
				for(i=1;i<=stg.envt.n;i++){
					if(stg.envt.e[i].type==6)
						stg.Password486(i,false);
				}
				menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
				return 100+trophy;
			}
		}

		// ȯ�� ����
		for(i=0;i<=6;i++){ // �Ҹ� ũ�� �ʱ�ȭ
			if(i==3) continue; // ������ �Ҹ��� �ȳ�
			stg.envt.volume[i] = 0.0;
		}
		// �Ÿ��� ���� �Ҹ� ũ�� ����
		for(i=1;i<=stg.envt.n;i++){
			if(stg.envt.e[i].type>6 || stg.envt.e[i].type==3 || stg.envt.e[i].type==4 || stg.envt.e[i].type==5) continue;
			// ����������~
			if(stg.envt.e[i].type==6){
				if(stg.envt.e[i].ax!=0) 
					stg.envt.volume[6] = (float)(absdouble(stg.envt.e[i].ax)/1.0);
				continue;
			}

			temp = pow2(stg.cFire.x-stg.envt.e[i].x)+pow2(stg.cFire.y-stg.envt.e[i].y); // ȯ��� �� ������ �Ÿ�
			if( temp <= 22500 ){
				if(stg.envt.volume[stg.envt.e[i].type] < 1.0 - (float)temp/22500.0)
					stg.envt.volume[stg.envt.e[i].type] = (float)(1.0 - (float)temp/22500.0);
			}
			temp = pow2(stg.cWater.x-stg.envt.e[i].x)+pow2(stg.cWater.y-stg.envt.e[i].y); // ȯ��� �� ������ �Ÿ�
			if( temp <= 22500 ){
				if(stg.envt.volume[stg.envt.e[i].type] < 1.0 - (float)temp/22500.0)
					stg.envt.volume[stg.envt.e[i].type] = (float)(1.0 - (float)temp/22500.0);
			}
		}
		for(i=0;i<=6;i++){
			if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
			FMOD_Channel_SetVolume(stg.envt.channel[i], stg.envt.volume[i]*(*volume_e));
		}
		FMOD_Channel_SetVolume(stg.envt.channel[3], (*volume_e));
		FMOD_Channel_SetVolume(stg.envt.channel[9], (*volume_e));
		FMOD_Channel_SetVolume(stg.channel, (*volume));
		FMOD_Channel_SetVolume(stg.channel_e, (*volume_e));
		// ������ź ���Ҹ� X
		if(stg.cFire.x>210 && stagenum==32) FMOD_Channel_SetVolume(stg.envt.channel[0], 0);
		
		// �޴��� ���콺�� ����
		if( menu.isOverMenu(cCursor.info.x,cCursor.info.y) ){
			if(!menu.now){
				Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[2]);
			}
			menu.Open();
			
			// �޴� ����
			if( isCrash(cCursor.info.x,cCursor.info.y,400,menu.y+51+35,130,30) ){
				if(menu.select!=1) Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[4]);
				menu.select = 1;
			}
			else if( isCrash(cCursor.info.x,cCursor.info.y,400,menu.y+51+75,160,30) ){
				if(menu.select!=2) Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[4]);
				menu.select = 2;
			}
			else if( isCrash(cCursor.info.x,cCursor.info.y,400,menu.y+51+115,130,30) ){
				if(menu.select!=3) Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[4]);
				menu.select = 3;
			}
			else if( isCrash(cCursor.info.x,cCursor.info.y,400,menu.y+51+155,130,30) ){
				if(menu.select!=4) Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[4]);
				menu.select = 4;
			}
			else menu.select = 0;

			stg.Print(screen);
		}
		else{
			if(menu.now){
				Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[3]);
			}
			menu.Close();

			// ĳ���� ������
			ai.timed++;
			if(stg.cFire.scale>0.01 && stg.cFire.alpha>0){
				// �ΰ�����
				if(ai.in_f!=NULL){
					if(ai.timed == ai.record_f[2]){
						if(ai.record_f[1]==1) ai.up[0] = (bool)ai.record_f[0];
						if(ai.record_f[1]==2) ai.down[0] = (bool)ai.record_f[0];
						if(ai.record_f[1]==3) ai.left[0] = (bool)ai.record_f[0];
						if(ai.record_f[1]==4) ai.right[0] = (bool)ai.record_f[0];
						fscanf_s(ai.in_f,"%d%d%d",&ai.record_f[0],&ai.record_f[1],&ai.record_f[2]);
					}
					if(stg.cFire.talpha>0) GetKey(&stg.cFire,ai.up[0],ai.down[0],ai.left[0],ai.right[0]);
				}
				// �����
				else if(stg.cFire.talpha>0) GetKey(&stg.cFire,0);
				// �Ұ� �Ҹ�
				if(stg.cFire.ball.motion==1){
					Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[0]);
				}
				stg.cFire.Move(stagenum);
			}
			stg.cFire.MoveBall();
			if(stg.cWater.scale>0.01 && stg.cWater.alpha>0){
				// �ΰ�����
				if(ai.in_w!=NULL){
					if(ai.timed == ai.record_w[2]){
						if(ai.record_w[1]==1) ai.up[1] = (bool)ai.record_w[0];
						if(ai.record_w[1]==2) ai.down[1] = (bool)ai.record_w[0];
						if(ai.record_w[1]==3) ai.left[1] = (bool)ai.record_w[0];
						if(ai.record_w[1]==4) ai.right[1] = (bool)ai.record_w[0];
						fscanf_s(ai.in_w,"%d%d%d",&ai.record_w[0],&ai.record_w[1],&ai.record_w[2]);
					}
					if(stg.cWater.talpha>0) GetKey(&stg.cWater,ai.up[1],ai.down[1],ai.left[1],ai.right[1]);
				}
				// �����
				else if(stg.cWater.talpha>0) GetKey(&stg.cWater,1);
				// ���� �Ҹ�
				if(stg.cWater.ball.motion==1){
					Sound_Play(&g_System,&stg.channel_e,&stg.sound_e[1]);
				}
				stg.cWater.Move(stagenum);
			}
			stg.cWater.MoveBall();

			// ȯ�� ������
			stg.envt.Action();
			stg.Interact(g_System);

			stg.Print(screen);

			stg.bgi.light(screen);

			if((et-st)/1000 != sst){
				second++;
				if(second>=60){
					minute++;
					second = 0;
				}
				sst = (et-st)/1000;
			}
			Print_Time(screen,font_time,img_time,minute,second);
		}
		
		menu.Print(*volume, *volume_e);
		cCursor.Print();

		// ���� �� ���� ǥ��
		if(alpha>0){
			alpha--;
			if(alpha>255){
				font_title.PrintAlpha(400,150,stg.bgi.text_title,255,255,255,255);
				font_desc.PrintAlpha(220,220,stg.bgi.text_desc[0],255,255,255,255,false);
				font_desc.PrintAlpha(220,260,stg.bgi.text_desc[1],255,255,255,255,false);
				font_desc.PrintAlpha(220,300,stg.bgi.text_desc[2],255,255,255,255,false);
			}else{
				font_title.PrintAlpha(400,150,stg.bgi.text_title,255,255,255,alpha);
				font_desc.PrintAlpha(220,220,stg.bgi.text_desc[0],255,255,255,alpha,false);
				font_desc.PrintAlpha(220,260,stg.bgi.text_desc[1],255,255,255,alpha,false);
				font_desc.PrintAlpha(220,300,stg.bgi.text_desc[2],255,255,255,alpha,false);
			}
		}
		// Ʃ�丮��
		if(!create){
			switch(stagenum){
			case 0:
				font_desc.Print(100,425,"��,�� : �̵�",255,255,128);
				font_desc.Print(550,250,"�� : �ٱ�",255,255,128);
				font_desc.Print(240,50,"���� ��",255,255,128);
				break;
			case 1:
				font_desc.Print(10,10,"R : �ٽ��ϱ�",255,255,128,false);
				font_desc.Print(10,60,"ESC : ������",255,255,128,false);
				break;
			case 2:
				font_desc.Print(100,100,"�� : �� �߻�",255,255,128);
				break;
			case 4:
				font_desc.Print(50,80,"�� ���� �Ұ��� �������� �غ�����.",255,255,128,false);
				break;
			case 5:
				font_desc.Print(70,375,"A, D : �̵�",255,255,128);
				break;
			case 6:
				font_desc.Print(70,180,"W : �ٱ�",255,255,128);
				break;
			case 7:
				font_desc.Print(90,80,"S : �� �߻�",255,255,128);
				break;
			case 8:
				font_desc.Print(5,60,"����� ���ÿ� ũ�Ⱑ �۾����� �� ���� ���� ���� �� �� �ֽ��ϴ�.",255,255,128,false);
				break;
			}
		}

		SDL_RenderPresent(screen);

		// ���α׷��� �̺�Ʈ�� ���� ��
		if( SDL_PollEvent( &event ) )
		{
			// ���콺 ������ ����
			if( event.type == SDL_MOUSEMOTION){
				// ���콺 ��ǥ ����
				cCursor.info.x = event.motion.x;
				cCursor.info.y = event.motion.y;

				if(event.button.button == SDL_BUTTON_LEFT){
					// �Ҹ� ũ�� ����
					if(isCrash(cCursor.info.x,cCursor.info.y,435,menu.y+51+205,100,30)){
						(*volume_e) = (float)((cCursor.info.x-395)/(float)80);
						if((*volume_e)<0.0) (*volume_e) = 0.0;
						if((*volume_e)>1.0) (*volume_e) = 1.0;
					}
					if(isCrash(cCursor.info.x,cCursor.info.y,435,menu.y+51+245,100,30)){
						(*volume) = (float)((cCursor.info.x-395)/(float)80);
						if((*volume)<0.0) (*volume) = 0.0;
						if((*volume)>1.0) (*volume) = 1.0;
					}
				}
			}
			if( event.type == SDL_MOUSEBUTTONDOWN ){
				if(menu.motion/50 >= PI/2){ // �޴��� ������ ������
					switch(menu.select){
					case 1:
						// �ٽ� �ϱ�
						menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
						for(i=0;i<=6;i++){
							if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
							FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
						}
						// ������ ���� �����
						for(i=1;i<=stg.envt.n;i++){
							if(stg.envt.e[i].type==6)
								stg.Password486(i,false);
						}
						return 100;
						break;
					case 2:
						// �ٸ��� �ϱ�
						menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
						for(i=0;i<=6;i++){
							if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
							FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
						}
						return 200;
						break;
					case 3:
						// ��� ����
						menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
						for(i=0;i<=6;i++){
							if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
							FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
						}
						return 300;
						break;
					case 4:
						// ��ü ȭ��
						cCursor.fullscreen = !cCursor.fullscreen;
						SDL_SetWindowFullscreen(window, cCursor.fullscreen);
						break;
					}
				}
			}
			if( event.key.type == SDL_KEYDOWN ){
				if(GetKeyState('R') < 0){
					for(i=0;i<=6;i++){
						if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
						FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
					}
					// ������ ���� �����
					for(i=1;i<=stg.envt.n;i++){
						if(stg.envt.e[i].type==6)
							stg.Password486(i,false);
					}
					menu.Destroy();font_desc.Destroy();font_title.Destroy();font_time.Destroy();SDL_DestroyTexture(img_time);SDL_DestroyTexture(img_backf);
					return 100;
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
	if(ai.in_f!=NULL) fclose(ai.in_f);
	if(ai.in_w!=NULL) fclose(ai.in_w);

	for(i=0;i<=6;i++){
		if(i==3 || i==4 || i==5) continue; // ������ ������ �Ҹ��� �ȳ�
		FMOD_Channel_SetVolume(stg.envt.channel[i], 0);
	}
	menu.Destroy();
	font_desc.Destroy();
	font_title.Destroy();
	font_time.Destroy();
	SDL_DestroyTexture(img_time);
	SDL_DestroyTexture(img_backf);
	return 300;
}