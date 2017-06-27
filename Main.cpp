#include "_stdafx.h"

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#pragma comment(lib,"SDL2_image.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"fmodex_vc.lib")

HWND hMutex;

/*변수*/
SDL_Window *window;
SDL_Renderer *screen;
SDL_Event event;

Stage stg;
BackGroundImage bgi;
Cursor cCursor;

MapInfo info;

int main(int argc, char *argv[])
{
	hMutex = CreateMutex(NULL, FALSE, L_"Bless Spirit");
	if(GetLastError() == ERROR_ALREADY_EXIST){
		return 0;
	}

	int i=0, j=0, k=0;
	int result1 = 0, result2 = 0, result3 = 0;
	float volume = 1.0, volume_e = 1.0;
	bool quit = false;
	char filename[20]={"music\\1.mp3"};

	ShowWindow( GetConsoleWindow(), SW_HIDE );

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	FMOD_SYSTEM		*g_System;
	stg.channel = NULL;

	window = SDL_CreateWindow("Bless Spirit",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,0);
	screen = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	//printf( "%s", SDL_GetError());
	//SDL_RendererInfo info2;
	//SDL_GetRendererInfo(screen, &info2);
	//printf("%s",info2.name);

	// 커서 숨기기
	SDL_ShowCursor(0);

	// 로딩
	Font font_load;
	font_load.Set(screen,"spring.ttf",20);
	font_load.Print(340,270,"소리 불러오는 중.",255,255,255,false); SDL_RenderPresent(screen);

	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);

	// BGM 로딩
	for(i=1;i<=5;i++){
		filename[6]=i+'0';
		Sound_Load(&g_System,filename,&stg.sound[i]);
	}
	font_load.Print(340,270,"소리 불러오는 중..",255,255,255,false); SDL_RenderPresent(screen);
	Sound_Load(&g_System,"music\\intro.mp3",&stg.sound[0],false);
	Sound_Load(&g_System,"music\\select.mp3",&stg.sound[6]);
	Sound_Load(&g_System,"music\\make.mp3",&stg.sound[7]);
	Sound_Load(&g_System,"music\\end.mp3",&stg.sound[8]);

	// 효과음 로딩
	font_load.Print(340,270,"소리 불러오는 중...",255,255,255,false); SDL_RenderPresent(screen);
	Sound_Load(&g_System,"sound\\fireball.ogg",&stg.sound_e[0],false);
	Sound_Load(&g_System,"sound\\waterball.ogg",&stg.sound_e[1],false);
	Sound_Load(&g_System,"sound\\paper.ogg",&stg.sound_e[2],false);
	Sound_Load(&g_System,"sound\\paper2.ogg",&stg.sound_e[3],false);
	Sound_Load(&g_System,"sound\\button.ogg",&stg.sound_e[4],false);

	// 환경 효과음 로딩
	strcpy_s(filename,"sound\\envt0.ogg");
	for(i=0;i<=6;i++){
		if(i==3 || i==4 || i==5) continue;
		filename[10]=i+'0';
		Sound_Load(&g_System,filename,&stg.envt.sound[i]);
	}
	Sound_Load(&g_System,"sound\\envt3.ogg",&stg.envt.sound[3],false); // 나무는 탈때만 치직거린다
	Sound_Load(&g_System,"sound\\envt9.ogg",&stg.envt.sound[9],false); // 번개는 부딪힐 때만 친다

	font_load.Print(340,310,"그림 불러오는 중.",255,255,255,false); SDL_RenderPresent(screen);

	// 커서 초기화
	cCursor.Set(screen);
	cCursor.Update(0);
	cCursor.fullscreen = false;

	FMOD_Channel_SetVolume(stg.channel,(float)1.0);
	FMOD_Channel_SetVolume(stg.channel_e,(float)1.0);

	font_load.Print(340,310,"그림 불러오는 중..",255,255,255,false); SDL_RenderPresent(screen);
	bgi.Set(screen);
	font_load.Print(340,310,"그림 불러오는 중...",255,255,255,false); SDL_RenderPresent(screen);
	stg.bgi = bgi;
	stg.Set(screen);

	font_load.Destroy();

	Sound_Play(&g_System,&stg.channel,&stg.sound[0]);
	Mode_Intro(screen,cCursor,g_System,&stg);
	Sound_Stop(&stg.channel_e);

	FMOD_Channel_SetVolume(stg.channel,(float)1.0);
	FMOD_Channel_SetVolume(stg.channel_e,(float)1.0);

	// 트로피 정보 로드
	FILE *in;
	fopen_s(&in,"data\\mapinfo.txt","r");
	if(in!=NULL){
		fscanf_s(in,"%d\n",&info.n);
		for(i=0;i<info.n;i++){
			fscanf_s(in,"%d ",&info.trophy[i]);
		}
		fclose(in);
	}else{
		info.n=0;
		info.trophy[0]=0;
	}

	result3 = 0;
	while(1){
		Sound_Play(&g_System,&stg.channel,&stg.sound[6]);
		result3 = Mode_Main(window,screen,g_System,stg.channel_e,stg.sound_e[4],bgi,cCursor,info.n);

		if(result3==0) break;
		if(result3==1){
			// 이야기 방식
			result2 = 0;
			while(1){
				// 맵 선택
				Sound_Play(&g_System,&stg.channel,&stg.sound[6]);
				FMOD_Channel_SetVolume(stg.channel,(float)volume);
				FMOD_Channel_SetVolume(stg.channel_e,(float)volume_e);
				result2 = Mode_SelectMap(window,screen,cCursor,&info,g_System,&stg,&volume,&volume_e);
				// 나가기
				if(result2==486) break;

				stg.Set2(screen,result2,false,false);

				if(result2<=9) Sound_Play(&g_System,&stg.channel,&stg.sound[1]);
				else if(result2<=15) Sound_Play(&g_System,&stg.channel,&stg.sound[2]);
				else if(result2<=25) Sound_Play(&g_System,&stg.channel,&stg.sound[3]);
				else if(result2<=28) Sound_Play(&g_System,&stg.channel,&stg.sound[4]);
				else Sound_Play(&g_System,&stg.channel,&stg.sound[5]);
				result1 = 0;
				while(1){
					// 스테이지 시작
					result1 = Mode_Stage(window,screen,g_System,stg,cCursor,result2,&volume,&volume_e,false);
					// 클리어
					if(result1<100){
						if(info.trophy[result2]<result1) info.trophy[result2] = result1;
						if(info.n==result2) info.n++;
						break;
					}
					// 클리어 후 다시하기
					if(result1<200 && result1!=100){
						if(info.trophy[result2]<result1-100) info.trophy[result2] = result1-100;
						if(info.n==result2) info.n++;
						stg.Set2(screen,result2,true,false);
						continue;
					}
					// 나가기
					if(result1 == 200){
						break;
					}
					// 재미 없어
					if(result1 == 300){
						break;
					}
					stg.Set2(screen,result2,true,false);
				}
			}
		}
		if(result3==2){
			// 사용자 제작 방식
			result2 = 0;
			while(1){
				// 맵 선택
				Sound_Play(&g_System,&stg.channel,&stg.sound[7]);
				FMOD_Channel_SetVolume(stg.channel,(float)volume);
				FMOD_Channel_SetVolume(stg.channel_e,(float)volume_e);
				result2 = Mode_SelectCreateMap(window,screen,cCursor,g_System,&stg,&volume,&volume_e);
				// 나가기
				if(result2==486) break;

				stg.Set2(screen,result2,false,true);

				Sound_Play(&g_System,&stg.channel,&stg.sound[rand()%5+1]);
				FMOD_Channel_SetVolume(stg.channel,(float)volume);
				FMOD_Channel_SetVolume(stg.channel_e,(float)volume_e);
				result1 = 0;
				while(1){
					// 스테이지 시작
					result1 = Mode_Stage(window,screen,g_System,stg,cCursor,result2,&volume,&volume_e,true);
					// 클리어
					if(result1<100) break;
					// 클리어 후 다시하기
					if(result1<200 && result1!=100){
						stg.Set2(screen,result2,true,true);
						continue;
					}
					// 나가기
					if(result1 == 200) break;
					// 재미 없어
					if(result1 == 300) break;
					stg.Set2(screen,result2,true,true);
				}
			}
		}
	}

	stg.Destroy();
	bgi.Destroy();

	FILE *out;
	fopen_s(&out,"data\\mapinfo.txt","w");
	fprintf_s(out,"%d\n",info.n);
	for(i=0;i<info.n;i++){
		fprintf_s(out,"%d ",info.trophy[i]);
	}
	fclose(out);

	SDL_Quit();
	return 0;
}