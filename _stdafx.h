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

// ȯ��
struct Envt{
	int n; // ����

	struct Entity{
		int type;
		int oy; // �߽� ��ǥ | �������� ��� (�սǵս� ȿ��)
		int x, y; // ���� ���� ��ǥ
		Uint8 w, h; // ����
		double cx, cy; // ���� �Ǽ� ��ǥ
		double ax, ay; // ���ӵ� ��ǥ
		double gy; // �߷� ���ӵ� | �񿡸� ��� (�߷� ���� ȿ��)
		int motion; // ��� ����
		int die; // �Ҹ� ��� ����
		bool whose; // � ĳ���Ϳ��� ���ԵǾ�����
		bool rainfire; // õ���� ���� ���� ���� �ҵ� ũ�Ⱑ �Ⱥ��Ѵ�
		double angle; // ����, ũ��
		bool use; // �����?
		bool crash; // �ε��� | ������ ���
		bool erase; // �������� ����
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
		int x, y; // ���� ��ǥ
		int ox, oy; // ���� �߽�
		double angle; // ����
		double scale; // ũ��
		double lscale; // ���� ũ��
		double speed; // �ӵ�
		Uint8 time; // ���� �� ����ð�
	}ptc[500];

	SDL_Texture *img;

	void Set(SDL_Renderer *screen);
	void Destroy();
	void Add(int type, int x, int y, double speed, double angle = 0);
	void Print(SDL_Renderer* screen);
};

struct Character{
	bool name; // ĳ���� �̸�
	int x, y;
	int motion, eye; // ���
	double ax, ay; // ���ӵ�
	double ac; // ���� ����
	double an; // �Ǽ� ���ӵ��� ���� ��ǥ�� �ٲ��ִ� ����
	bool dir; // 0:��, 1:��
	double angle; // ����... ����� ����
	double scale; // ũ��
	double tscale; // Ÿ�� ������
	Uint8 alpha; // ����
	Uint8 talpha; // Ÿ�� ���İ�
	bool onRock; // �� ���� �ִ°�?

	Particle ptc; // ��ƼŬ

	struct Ball{
		int x, y;
		bool dir; // ����
		int motion; // ���
		SDL_Texture *img;
		SDL_Rect clip[4];
	}ball;

	SDL_Texture *img;
	SDL_Texture *img_eye;
	SDL_Rect rect[10], rect_eye[2];
	SDL_Surface *cImg; // �浹 üũ�� �� �̹���

	void Print(SDL_Renderer *screen);
	bool isCanMove(int x, int y, int w, int h);
	void DoMove(int dx, int dy);
	void Accel(int state);
	void MoveBall();
	void Move(int stagenum);
	void Gravity();
};

struct BackGroundImage{
	int type, ltype; // Ÿ��, ���� Ÿ��

	SDL_Texture *img_bgi[10]; // �޹��
	SDL_Texture *img_bgl[10][10]; // �޹�� ����
	SDL_Texture *img_glow; // �۷ο�

	SDL_Surface *img_maps; // �չ�� ���ǽ�
	SDL_Surface *img_textures; // �չ�� �ؽ��� ���ǽ�
	SDL_Texture *img_texture; // �չ�� �ؽ���
	SDL_Texture *img_shadow; // �չ�� �׸���

	double glow1, glow2;

	char text_title[200]; // ����
	char text_desc[3][200]; // ����
	int line; // ���� �����̳�
	int bgnum, bgtnum; // ���� ���� �ؽ��� ����
	struct EndPoint{
		int x, y;
		double motion;
		SDL_Texture *img1, *img2;
		Particle ptc;
	}end;

	int alpha; // ���� ȭ�� ��ȯ�� ���� ���İ�
	
	SDL_Texture* img_light;

	int lp; // �� ȿ��
	int lpow; // �� ȿ�� �ð� ����

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
	// Ŀ�� ��ġ, ũ��
	SDL_Rect info;

	SDL_Renderer *screen;
	SDL_Texture *img_cursor; // ���� Ŀ��
	SDL_Surface *img_cursors; // ���� Ŀ�� ���ǽ�
	SDL_Texture *img_cursorchip[100]; // Ŀ�� Ĩ
	SDL_Surface *img_cursorchips[100]; // Ŀ�� Ĩ (���ǽ�)

	void Update(int type); // Ŀ�� ���� ������Ʈ
	void Print(); // Ŀ�� ���
	void Set(SDL_Renderer *screen); // Ŀ�� ����
	void Destroy();
};

// �޴�
struct Menu{
	SDL_Texture *img;
	int x, y;
	int select;
	double motion;
	bool now;

	SDL_Renderer *screen;

	SDL_Texture *minibg; // ��� ������
	SDL_Texture *minibgt; // ���� �ؽ��� ������
	SDL_Texture *vol_bar, *vol_button; // �Ҹ� ũ�� ��, ��ư
	SDL_Rect clip_bg[8], clip_bgt[5];
	SDL_Texture *arrow, *arrowhite; // ��� ������

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

	SDL_Texture *img_map; // ���� ��
	SDL_Surface *img_maps; // ���� �� ���ǽ�
	SDL_Texture *img_bgi[8]; // �޹��

	SDL_Texture *img_load; // �ε� �̹���
	SDL_Texture *img_tool; // ���� ����̹���
	SDL_Texture *img_toolSize; // ���� ���� ������ ��� �̹���
	SDL_Texture *img_tbdr; // ���� ���� ���
	SDL_Texture *img_tbdr2; // ������ ���� ���
	SDL_Texture *img_woe; // ���� / ���찳
	SDL_Texture *img_soh; // ��� ���̱� / ������
	SDL_Texture *img_toolR; // ������ ���� ��������Ʈ
	
	int bgnum, bgtnum; // ���, ���� �ؽ��� Ÿ��
	bool woe; // ���� ���� / ����� ����
	bool rwoe; // ������ Ŭ������ �ݴ��� ���¸� ���� ��
	bool soh; // ��� ���̱� / ������
	int toolL[10]; // ���� �� ũ�� ���� ����
	int toolLX[10]; // ���� �� ũ�� �� x ��ġ
	double toolLT[10]; // ���� �� ũ�� �� x ��ġ�� ������ �� ��Ÿ ��
	bool toolLE[10]; // ���� �� ũ�� �� ������ ����

	int listn;
	int locationX, locationY;
	
	char text_title[200]; // ����
	char text_desc[3][200]; // ����
	int len_title;
	int len_desc[3];
	int line, cursor_title, cursor_desc[3];

	bool write; // �ʿ� �׸��� ��
	struct Stack{
		int now, top;
		bool smap[10][HEIGHT+10][WIDTH+10];
		void Push(bool map[][WIDTH+10]);
		void Pop(bool map[][WIDTH+10]);
		void Erase(bool map[][WIDTH+10]);
	}stack;

	Envt envt;

	Cursor cCursor;

	void Set(); // ����
	void Destroy(); // ����
	void Clear(); // �ʱ�ȭ
	char* Main(FMOD_CHANNEL *channel, float *volume, float *volume_e, bool isLoad = false); // ����
	void Undo(); // ���� ���
	void Print(double fscale, double wscale); // ���
	void SetTool(); // �� ����
	void CloseTool(); // �� �ݱ�
	void SetToolSize(); // ���� ���� ũ�� ���� ����
	void Draw(int x, int y); // �ʿ� �� ���
	void DrawShape(int x, int y); // �ʿ� ������ �׸���
	bool Save(double fscale, double wscale, SDL_Texture *img_menu); // ���̺�
	void Load(double *fscale, double *wscale); // �ε�
	void ClearEvent(SDL_Event* event); // �̺�Ʈ �ʱ�ȭ
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