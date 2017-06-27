#pragma once
#include "_stdafx.h"

void Envt::Print(SDL_Renderer *screen, bool dynamic){
	int i;

	// 비는 가장 먼저 출력하자
	if(dynamic){
		for(i=1;i<=n;i++){
			// 비
			if( e[i].type == 0 ){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ 0 ] );
			}
		}
	}
	// 환경 출력
	for(i=1;i<=n;i++){
		if(e[i].type==7 || e[i].type==8) continue; // 시작 포인트는 출력하지 않는다
		// 동적 출력
		if(dynamic){
			// 불
			if(e[i].type == 1){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ 0 ][ (int)(e[i].motion/12) %4 ] );
			}
			// 물
			else if(e[i].type == 2){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ (int)(e[i].motion/10) %8 ] );
			}
			// 나무
			else if(e[i].type == 3){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ 0 ] );
			}
			// 구름
			else if( e[i].type == 4 || e[i].type == 5 ){
				// 구름이 떠오르는 중이면 투명한 효과를 준다
				if(e[i].motion<300)	SDL_SetTextureAlphaMod(img[e[i].type],(Uint8)((double)e[i].motion/300*255));

				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true);

				if(e[i].motion<300)	SDL_SetTextureAlphaMod(img[e[i].type],255);
			}
			// 돌멩이
			else if(e[i].type==6){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, e[i].angle, false, true);
			}
			// 기타 : 번개
			else if(e[i].type!=0){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true);
			}
		}

		// 정적 출력
		else{
			if(e[i].erase) SDL_SetTextureColorMod(img[e[i].type],255,128,128);
			apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true);
			if(e[i].erase) SDL_SetTextureColorMod(img[e[i].type],255,255,255);
		}
	}
}

void Envt::Action(){
	int i, j;
	for(i=1;i<=n;i++){
		// 불
		if(e[i].type == 1){
			e[i].motion++;
			// 환경이 없어지는 중이라면
			if(e[i].die>0){
				// 소멸 변수 증가
				e[i].die++;
			}
			// 완전히 없어지면
			if(e[i].die>=1){
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// 물
		else if(e[i].type == 2){
			e[i].motion++;
			// 환경이 없어지는 중이라면
			if(e[i].die>0){
				// 소멸 변수 증가
				e[i].die++;
			}
			// 완전히 없어지면
			if(e[i].die>=60){
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// 나무
		else if(e[i].type == 3){
			e[i].motion++;
			// 환경이 없어지는 중이라면
			if(e[i].die>0){
				// 소멸 변수 증가
				e[i].die++;
			}
			// 완전히 없어지면
			if(e[i].die>=70){
				// 불로 바뀐다..!
				e[i].type=1;
				e[i].die=0;	e[i].motion=0; e[i].y += 31;
				continue;
			}
		}
		// 구름
		else if( e[i].type == 4 || e[i].type == 5 ){
			e[i].motion++;
			// 떠오르기
			if(e[i].motion<300){
				e[i].cy -= 0.125;
				e[i].oy = (int)e[i].cy;
			}
			else{
				// 구름에 움직임을 준다.
				if(e[i].type==4) e[i].cx+=0.125;
				else if(e[i].type==5) e[i].cx-=0.125;
				// 둥실둥실 효과
				e[i].cy = e[i].oy + 3*sin((double)(e[i].motion-300)/50);

				// 비가 내린다~
				if(e[i].motion%10==0){
					n++;
					e[n].type = 0;
					e[n].x = e[i].x + (e[i].motion*3)%80 - 40;
					e[n].y = e[i].oy;
					e[n].gy = 1.0;
					e[n].cx = e[n].x; e[n].cy = e[n].y;	e[n].oy = e[n].y; e[n].die = 0;	e[n].motion = 0;
				}
			}
			// 실수좌표를 정수좌표로
			e[i].x = (int)e[i].cx;
			e[i].y = (int)e[i].cy;

			if( e[i].x < -50 || e[i].x > WIDTH+300 ){ // 맵 밖으로 벗어나면
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// 돌멩이
		else if( e[i].type == 6 ){
			// 중력 가속도
			if(e[i].ay != 0) e[i].ay+=0.03;

			// 밀리고 있을 때는 가속도를 주지 마라
			if(e[i].whose == false){
				e[i].cx+=e[i].ax;
			
				// 아니면 마찰력을 주자
				if(e[i].ax < 0) e[i].ax += 0.005;
				else if(e[i].ax > 0) e[i].ax -= 0.005;
				if( -0.0025 <= e[i].ax && e[i].ax <= 0.0025 ) e[i].ax = 0;
			}

			if(e[i].ax>1.0) e[i].ax=1.0;
			if(e[i].ax<-1.0) e[i].ax=-1.0;

			e[i].cy+=e[i].ay;

			e[i].angle+=e[i].ax * PI;

			if( e[i].y < -16 || e[i].y > HEIGHT+16 ||  e[i].x < -16 || e[i].x > WIDTH+16 ){ // 맵 밖으로 벗어나면
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// 벼락
		else if( e[i].type == 9 ){
			e[i].y+=3;

			if( e[i].y < -27 || e[i].y > HEIGHT+27 || e[i].die>0 ){ // 맵 밖으로 벗어나거나 땅에 닿으면
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// 비
		else if( e[i].type == 0 ){
			// 환경이 없어지는 중이라면
			if(e[i].die>0){
				// 소멸 변수 증가
				e[i].die++;
			}
			// 비를 내리자
			else{
				e[i].gy+=0.01;
				e[i].cy += e[i].gy;
				e[i].y = (int)e[i].cy;
			}
			// 완전히 없어지면
			if(e[i].die>=30 || e[i].y>HEIGHT+20 || e[i].x>WIDTH || e[i].x<0){
				// 제거
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
	}
}