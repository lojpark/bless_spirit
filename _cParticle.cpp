#pragma once
#include "_stdafx.h"

// 파티클 기본 값 세팅
void Particle::Set(SDL_Renderer *screen){
	img = load_image(screen,"image\\star.png");
	n = 0;
}
void Particle::Destroy(){
	SDL_DestroyTexture(img);
}
void Particle::Add(int type, int x, int y, double speed, double angle){
	n++;
	ptc[n].x = ptc[n].ox = x;
	ptc[n].y = ptc[n].oy = y;
	// ptc[n].angle = (rand()%60 - 30)*PI/180 + angle;
	ptc[n].speed = speed;
	ptc[n].angle = angle;
	if(type==1) ptc[n].scale = ptc[n].lscale = 0.5;
	else ptc[n].scale = ptc[n].lscale = 1.0;
	ptc[n].time = 0;
}

bool Move(Particle *ptc, int i){
	(*ptc).ptc[i].time+=2;
	if( (*ptc).ptc[i].time >= 254 ){
		return 0;
	}
	(*ptc).ptc[i].scale = (*ptc).ptc[i].lscale-((double)(*ptc).ptc[i].time/255)*(*ptc).ptc[i].lscale;

	(*ptc).ptc[i].x = (int)((*ptc).ptc[i].ox + cos(((*ptc).ptc[i].angle)) * -1*(*ptc).ptc[i].time/2*(*ptc).ptc[i].speed);
	(*ptc).ptc[i].y = (int)((*ptc).ptc[i].oy + sin(((*ptc).ptc[i].angle)) * -1*(*ptc).ptc[i].time/2*(*ptc).ptc[i].speed);

	return 1;
}

void Particle::Print(SDL_Renderer* screen){
	int i, j;
	for(i=1;i<=n;i++){
		// 없어질 때가 된 파티클은
		if(!Move(this,i)){
			// 없애버린다
			for(j=i;j<n;j++){
				ptc[j] = ptc[j+1];
			}
			n--;
			i--;
		}
		// 아니면 출력
		else{
			SDL_SetTextureAlphaMod( img , 255-ptc[i].time );
			apply_surface(ptc[i].x, ptc[i].y, img, screen, ptc[i].scale, ptc[i].angle, false, true);
			SDL_SetTextureAlphaMod( img , 255 );
		}
	}
}