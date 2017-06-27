#pragma once
#include "_stdafx.h"

void GetKey(Character *c, int type){
	int key[2][4] = {
		{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT},
		{'W', 'S', 'A', 'D'}
	};
	static bool key_state[2] = {false, false};
	
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	
	if( GetKeyState(key[type][0]) < 0 ) up = true;
	if( GetKeyState(key[type][2]) < 0 ) left = true;
	if( GetKeyState(key[type][3]) < 0 ) right = true;
	
	if( GetKeyState(key[type][1]) >= 0 ) key_state[type] = false;
	if( GetKeyState(key[type][1]) < 0 && key_state[type] == false ){
		down = true;
		key_state[type] = true;
	}
	
	if( up == true ){ // 점프
		if(c->ay < -100){ // 공중이 아닐 경우
			c->ay=-40; // y 가속도를 준다.
		}
	}
	if( down == true && c->ball.motion == 0 ){ // 발사
		if(c->name) c->tscale-=0.125;
		c->ball.motion = 1;
		c->ball.x = c->x;
		c->ball.y = c->y;
		c->ball.dir = c->dir;
	}

	if( left == true && right == true ){ // 왼쪽 오른쪽 동시에 누르고 있으면
		c->Accel(0); // 정지 상태로
	}
	else if( left == true ){ // 왼쪽을 누르고 있으면
		c->dir=false;
		c->Accel(-1);
	}
	else if( right == true ){ // 오른쪽을 누르고 있으면
		c->dir=true;
		c->Accel(1);
	}
	else{ // 아무 키도 안 누르고 있으면 정지 상태로
		c->Accel(0);
	}
}

// 컴퓨터가 조종
void GetKey(Character *c, bool up, bool down, bool left, bool right){
	if( up == true ){ // 점프
		if(c->ay < -100){ // 공중이 아닐 경우
			c->ay=-40; // y 가속도를 준다.
		}
	}
	if( down == true && c->ball.motion == 0 ){ // 발사
		if(c->name) c->tscale-=0.125;
		c->ball.motion = 1;
		c->ball.x = c->x;
		c->ball.y = c->y;
		c->ball.dir = c->dir;
	}

	if( left == true && right == true ){ // 왼쪽 오른쪽 동시에 누르고 있으면
		c->Accel(0); // 정지 상태로
	}
	else if( left == true ){ // 왼쪽을 누르고 있으면
		c->dir=false;
		c->Accel(-1);
	}
	else if( right == true ){ // 오른쪽을 누르고 있으면
		c->dir=true;
		c->Accel(1);
	}
	else{ // 아무 키도 안 누르고 있으면 정지 상태로
		c->Accel(0);
	}
}

void GetAi(Ai *ai, int type){
	int key[2][4] = {
		{VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT},
		{'W', 'S', 'A', 'D'}
	};

	(*ai).time++;
	if(GetKeyState(key[type][0]) < 0){
		if(!(*ai).up[type]){
			(*ai).up[type] = true;
			(*ai).record[0][(*ai).n] = 1;
			(*ai).record[1][(*ai).n] = 1;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][0]) >= 0){
		if((*ai).up[type]){
			(*ai).up[type] = false;
			(*ai).record[0][(*ai).n] = 0;
			(*ai).record[1][(*ai).n] = 1;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][1]) < 0){
		if(!(*ai).down[type]){
			(*ai).down[type] = true;
			(*ai).record[0][(*ai).n] = 1;
			(*ai).record[1][(*ai).n] = 2;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][1]) >= 0){
		if((*ai).down[type]){
			(*ai).down[type] = false;
			(*ai).record[0][(*ai).n] = 0;
			(*ai).record[1][(*ai).n] = 2;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][2]) < 0){
		if(!(*ai).left[type]){
			(*ai).left[type] = true;
			(*ai).record[0][(*ai).n] = 1;
			(*ai).record[1][(*ai).n] = 3;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][2]) >= 0){
		if((*ai).left[type]){
			(*ai).left[type] = false;
			(*ai).record[0][(*ai).n] = 0;
			(*ai).record[1][(*ai).n] = 3;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][3]) < 0){
		if(!(*ai).right[type]){
			(*ai).right[type] = true;
			(*ai).record[0][(*ai).n] = 1;
			(*ai).record[1][(*ai).n] = 4;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
	if(GetKeyState(key[type][3]) >= 0){
		if((*ai).right[type]){
			(*ai).right[type] = false;
			(*ai).record[0][(*ai).n] = 0;
			(*ai).record[1][(*ai).n] = 4;
			(*ai).record[2][(*ai).n] = (*ai).time;
			(*ai).n++;
		}
	}
}