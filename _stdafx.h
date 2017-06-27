#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <windows.h>
//#include "han2unicode.h"

#define PI 3.141592
#define WIDTH 800
#define HEIGHT 600

struct Font{
	SDL_Renderer *screen;
	
	SDL_Surface *messages;
	SDL_Texture *message;
	TTF_Font    *font;

	void Set(SDL_Renderer *screen, char *font_name, int size);
	void Destroy();
	void Print(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, bool center = true);
	void PrintInput(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, bool center = true);
	void PrintAlpha(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha, bool center = true);
	void PrintAlpha2(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha, bool center = true);
};

// 환경
struct Envt{
	int n; // 개수

	struct Entity{
		int type;
		int oy; // 중심 좌표 | 구름에만 사용 (둥실둥실 효과)
		int x, y; // 실제 정수 좌표
		Uint8 w, h; // 길이
		double cx, cy; // 계산용 실수 좌표
		double ax, ay; // 가속도 좌표
		double gy; // 중력 가속도 | 비에만 사용 (중력 가속 효과)
		int motion; // 모션 변수
		int die; // 소멸 모션 변수
		bool whose; // 어떤 캐릭터에게 포함되었는지
		bool rainfire; // 천둥이 물을 끄면 물도 불도 크기가 안변한다
		double angle; // 각도, 크기
		bool use; // 사용중?
		bool crash; // 부딪힘 | 돌에만 사용
		bool erase; // 지워지기 직전
	}e[1000];

	SDL_Texture *img[10];
	SDL_Rect clip[10][10][10];

	FMOD_CHANNEL *channel[10];
	FMOD_SOUND *sound[10];
	float volume[7];

	void Print(SDL_Renderer *screen, bool dynamic);
	void Action();
};

struct Particle{
	int n;
	struct Particles{
		int type;
		int x, y; // 현재 좌표
		int ox, oy; // 원의 중심
		double angle; // 각도
		double scale; // 크기
		double lscale; // 원래 크기
		double speed; // 속도
		Uint8 time; // 생성 후 경과시간
	}ptc[500];

	SDL_Texture *img;

	void Set(SDL_Renderer *screen);
	void Destroy();
	void Add(int type, int x, int y, double speed, double angle = 0);
	void Print(SDL_Renderer* screen);
};

struct Character{
	bool name; // 캐릭터 이름
	int x, y;
	int motion, eye; // 모션
	double ax, ay; // 가속도
	double ac; // 가속 변수
	double an; // 실수 가속도를 정수 좌표로 바꿔주는 변수
	bool dir; // 0:왼, 1:오
	double angle; // 각도... 비운의 변수
	double scale; // 크기
	double tscale; // 타겟 스케일
	Uint8 alpha; // 투명도
	Uint8 talpha; // 타겟 알파값
	bool onRock; // 돌 위에 있는가?

	Particle ptc; // 파티클

	struct Ball{
		int x, y;
		bool dir; // 방향
		int motion; // 모션
		SDL_Texture *img;
		SDL_Rect clip[4];
	}ball;

	SDL_Texture *img;
	SDL_Texture *img_eye;
	SDL_Rect rect[10], rect_eye[2];
	SDL_Surface *cImg; // 충돌 체크용 맵 이미지

	void Print(SDL_Renderer *screen);
	bool isCanMove(int x, int y, int w, int h);
	void DoMove(int dx, int dy);
	void Accel(int state);
	void MoveBall();
	void Move(int stagenum);
	void Gravity();
};

struct BackGroundImage{
	int type, ltype; // 타입, 이전 타입

	SDL_Texture *img_bgi[10]; // 뒷배경
	SDL_Texture *img_bgl[10][10]; // 뒷배경 조명
	SDL_Texture *img_glow; // 글로우

	SDL_Surface *img_maps; // 앞배경 서피스
	SDL_Surface *img_textures; // 앞배경 텍스쳐 서피스
	SDL_Texture *img_texture; // 앞배경 텍스쳐
	SDL_Texture *img_shadow; // 앞배경 그림자

	double glow1, glow2;

	char text_title[200]; // 제목
	char text_desc[3][200]; // 설명
	int line; // 설명 몇줄이냐
	int bgnum, bgtnum; // 배경과 지형 텍스쳐 종류
	struct EndPoint{
		int x, y;
		double motion;
		SDL_Texture *img1, *img2;
		Particle ptc;
	}end;

	int alpha; // 멋진 화면 전환을 위한 알파값
	
	SDL_Texture* img_light;

	int lp; // 빛 효과
	int lpow; // 빛 효과 시간 강도

	void Set(SDL_Renderer *screen);
	void Print_Back(SDL_Renderer *screen, bool blur);
	void Print_Shadow(SDL_Renderer *screen);
	void Print_Texture(SDL_Renderer *screen);
	void Print_Endpoint(SDL_Renderer *screen);
	void light(SDL_Renderer *screen);
	void Destroy();
};

struct Stage{
	Envt envt;
	Character cFire, cWater;
	BackGroundImage bgi;
	FMOD_CHANNEL *channel, *channel_e;
	FMOD_SOUND *sound[10], *sound_e[5];

	void Set(SDL_Renderer *screen);
	void Set2(SDL_Renderer *screen, int stagenum, bool reset, bool create);
	void Set_Char(SDL_Renderer *screen);
	void Destroy_Char();
	void Interact(FMOD_SYSTEM *g_System);
	void Password486(int i, bool write);
	void Print(SDL_Renderer* screen);
	void Destroy();
};

struct Cursor{
	int type;
	double scale;
	bool fullscreen;
	// 커서 위치, 크기
	SDL_Rect info;

	SDL_Renderer *screen;
	SDL_Texture *img_cursor; // 현재 커서
	SDL_Surface *img_cursors; // 현재 커서 서피스
	SDL_Texture *img_cursorchip[100]; // 커서 칩
	SDL_Surface *img_cursorchips[100]; // 커서 칩 (서피스)

	void Update(int type); // 커서 정보 업데이트
	void Print(); // 커서 출력
	void Set(SDL_Renderer *screen); // 커서 세팅
	void Destroy();
};

// 메뉴
struct Menu{
	SDL_Texture *img;
	int x, y;
	int select;
	double motion;
	bool now;

	SDL_Renderer *screen;

	SDL_Texture *minibg; // 배경 설정용
	SDL_Texture *minibgt; // 지형 텍스쳐 설정용
	SDL_Texture *vol_bar, *vol_button; // 소리 크기 바, 버튼
	SDL_Rect clip_bg[8], clip_bgt[5];
	SDL_Texture *arrow, *arrowhite; // 배경 설정용

	Font font1, font2;

	void Set(int x, SDL_Renderer *screen);
	void Open();
	void Close();
	void Print(float volume, float volume_e);
	void Destroy();
	void Print(int bgnum, int bgtnum, float volume, float volume_e, bool possible);
	void Stage(float volume, float volume_e);
	void Map(int bgnum, int bgtnum, float volume, float volume_e, bool possible);
	bool isOverMenu(int cx, int cy);
};

struct MakeMap{
	bool map[HEIGHT+10][WIDTH+10];
	FILE *in;
	FILE *out;

	SDL_Renderer *screen;
	SDL_Window *window;

	SDL_Texture *img_map; // 만들 맵
	SDL_Surface *img_maps; // 만들 맵 서피스
	SDL_Texture *img_bgi[8]; // 뒷배경

	SDL_Texture *img_load; // 로딩 이미지
	SDL_Texture *img_tool; // 툴바 배경이미지
	SDL_Texture *img_toolSize; // 왼쪽 툴바 사이즈 배경 이미지
	SDL_Texture *img_tbdr; // 왼쪽 툴바 경계
	SDL_Texture *img_tbdr2; // 오른쪽 툴바 경계
	SDL_Texture *img_woe; // 연필 / 지우개
	SDL_Texture *img_soh; // 배경 보이기 / 가리기
	SDL_Texture *img_toolR; // 오른쪽 툴바 스프라이트
	
	int bgnum, bgtnum; // 배경, 지형 텍스쳐 타입
	bool woe; // 쓰기 상태 / 지우기 상태
	bool rwoe; // 오른쪽 클릭으로 반대의 상태를 만들 때
	bool soh; // 배경 보이기 / 가리기
	int toolL[10]; // 왼쪽 툴 크기 현재 상태
	int toolLX[10]; // 왼쪽 툴 크기 바 x 위치
	double toolLT[10]; // 왼쪽 툴 크기 바 x 위치에 가속을 줄 세타 값
	bool toolLE[10]; // 왼쪽 툴 크기 바 나오나 들어가나

	int listn;
	int locationX, locationY;
	
	char text_title[200]; // 제목
	char text_desc[3][200]; // 설명
	int len_title;
	int len_desc[3];
	int line, cursor_title, cursor_desc[3];

	bool write; // 맵에 그리는 중
	struct Stack{
		int now, top;
		bool smap[10][HEIGHT+10][WIDTH+10];
		void Push(bool map[][WIDTH+10]);
		void Pop(bool map[][WIDTH+10]);
		void Erase(bool map[][WIDTH+10]);
	}stack;

	Envt envt;

	Cursor cCursor;

	void Set(); // 셋팅
	void Destroy(); // 없애
	void Clear(); // 초기화
	char* Main(FMOD_CHANNEL *channel, float *volume, float *volume_e, bool isLoad = false); // 메인
	void Undo(); // 실행 취소
	void Print(double fscale, double wscale); // 출력
	void SetTool(); // 툴 셋팅
	void CloseTool(); // 툴 닫기
	void SetToolSize(); // 왼쪽 툴바 크기 지정 툴바
	void Draw(int x, int y); // 맵에 점 찍기
	void DrawShape(int x, int y); // 맵에 모양따라 그리기
	bool Save(double fscale, double wscale, SDL_Texture *img_menu); // 세이브
	void Load(double *fscale, double *wscale); // 로드
	void ClearEvent(SDL_Event* event); // 이벤트 초기화
};

struct MapButton{
	int n;
	SDL_Texture *img;
	SDL_Rect clip[5];

	struct Buttons{
		int x, y;
		int type;
	}b[50];
};

struct Ai{
	bool left[2];
	bool right[2];
	bool up[2];
	bool down[2];
	int time, timed;
	int n, ned;
	int record[3][1000];
	int record_f[3], record_w[3];
	FILE *in_f, *in_w;
	void Set();
	void Load(int stagenum);
	//void Save();
};

struct MapInfo{
	int n;
	Uint8 trophy[40];
};

// SDL_Setting
SDL_Texture *load_image(SDL_Renderer *renderer,char *filename);
void apply_surface(int x, int y, SDL_Texture *source, SDL_Renderer *destination, double scale, double angle, bool flip, bool center, SDL_Rect *clip = NULL);
Uint32 Set_Frame(Uint32 curTicks);

// GetEvent
void GetKey(Character *c, int type);
void GetKey(Character *c, bool up, bool down, bool left, bool right);
void GetAi(Ai* ai, int type);

// Function
bool isCrash(int x1, int y1, int x2, int y2, int w, int h);
bool isCrash2(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
bool isCrash2_l(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
bool isCrash2_r(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void CreateFilename(char *filename, int num);
void CreateFilename2(char *filename, int num);
int pow2(int number);
double absdouble(double number);

// Sound
void Sound_Load(FMOD_SYSTEM **g_System, char *filename, FMOD_SOUND **sound, bool loop = true);
void Sound_Play(FMOD_SYSTEM **g_System, FMOD_CHANNEL **channel, FMOD_SOUND **sound);
void Sound_Stop(FMOD_CHANNEL **channel);

// Mode
void Mode_Intro(SDL_Renderer *screen, Cursor cCursor, FMOD_SYSTEM *g_System, Stage *stg);
int Mode_Main(SDL_Window *window, SDL_Renderer *screen, FMOD_SYSTEM *g_System, FMOD_CHANNEL *channel, FMOD_SOUND *sound, BackGroundImage bgi, Cursor cCursor, int n);
int Mode_SelectMap(SDL_Window *window, SDL_Renderer *screen, Cursor cCursor, MapInfo *info, FMOD_SYSTEM *g_System, Stage *stg, float *volume, float *volume_e);
int Mode_SelectCreateMap(SDL_Window *window, SDL_Renderer *screen, Cursor cCursor, FMOD_SYSTEM *g_System, Stage *stg, float *volume, float *volume_e);
int Mode_Stage(SDL_Window *window, SDL_Renderer *screen, FMOD_SYSTEM *g_System, Stage stg, Cursor cCursor, int stagenum, float *volume, float *volume_e, bool create);

// Database
int Database_nexttrophy(int stagenum, int trophy);
int Database_trophy(int stagenum, int time);