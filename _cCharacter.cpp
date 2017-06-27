#pragma once
#include "_stdafx.h"

void Character::Print(SDL_Renderer* screen){
	int i;

	// ��ƼŬ 
	ptc.Print(screen);

	SDL_SetTextureAlphaMod(img,alpha);
	SDL_SetTextureAlphaMod(img_eye,alpha);

	// �� ������
	if(eye<10 && rand()%100+1==5) eye = 19;
	else if(eye>0) eye--;

	int tmpy=0, tmotion=0;
	// �սǵս�
	tmpy = (int)(y + 6*sin( (double)motion/30 ));
	// ĳ���ͺ� ���
	if(name==false) tmotion = int(motion/10)%8;
	else tmotion = int(motion/10)%9;
	// ������ ����
	if(dir==false){
		apply_surface(x, tmpy, img, screen, scale, angle, true, true, &rect[tmotion]);
		if(name==false){
			apply_surface((int)(x-17*scale), (int)(tmpy+3*scale), img_eye, screen, scale, angle, true, true, &rect_eye[eye/10]);
			apply_surface((int)(x+6*scale), (int)(tmpy+7*scale), img_eye, screen, scale, angle, true, true, &rect_eye[eye/10]);
		}
		else{
			apply_surface((int)(x-14*scale), (int)(tmpy+4*scale), img_eye, screen, scale, angle, true, true, &rect_eye[eye/10]);
			apply_surface((int)(x+7*scale), (int)(tmpy+5*scale), img_eye, screen, scale, angle, true, true, &rect_eye[eye/10]);
		}
	}
	// �������� ����
	else{
		apply_surface(x, tmpy, img, screen, scale, angle, false, true, &rect[tmotion]);
		// ĳ���ͺ� ��������
		if(name==false){
			apply_surface((int)(x-6*scale), (int)(tmpy+7*scale), img_eye, screen, scale, angle, false, true, &rect_eye[eye/10]);
			apply_surface((int)(x+17*scale), (int)(tmpy+3*scale), img_eye, screen, scale, angle, false, true, &rect_eye[eye/10]);
		}
		else{
			apply_surface((int)(x-6*scale), (int)(tmpy+5*scale), img_eye, screen, scale, angle, false, true, &rect_eye[eye/10]);
			apply_surface((int)(x+15*scale), (int)(tmpy+4*scale), img_eye, screen, scale, angle, false, true, &rect_eye[eye/10]);
		}
	}

	if(ball.motion > 0){
		// �� ���
		if(!name) apply_surface( ball.x, ball.y, ball.img, screen, 1.0, (ball.motion*4) %360, false, true, &ball.clip[ (int)(ball.motion/10) %4] );
		else apply_surface( ball.x, ball.y, ball.img, screen, 1.0, 0, false, true, &ball.clip[ (int)(ball.motion/10) %4] );
	}
}

// ������ �� �ִ°�?
bool Character::isCanMove(int x, int y, int w, int h){
	if(x>=WIDTH || x<=0) return false;

	if(y+h/2>HEIGHT || y+h/2<=0) return true;

	if(SDL_MapRGB(cImg->format, 0, 0, 0) == *((Uint32 *)cImg->pixels + (y + h/2)*cImg->pitch/4 + x) ){
		onRock = false;
		return false;
	}
	if(SDL_MapRGB(cImg->format, 4, 8, 6) == *((Uint32 *)cImg->pixels + (y + h/2)*cImg->pitch/4 + x) ){
		onRock = true;
		return false;
	}

	return true;
}

// �߷� ����
void Character::Gravity(){
	int i;
	int delta=0, n=0; // ������ �Ÿ�, �ӽ� ��ǥ

	if(ay<=-100) return;

	if(ay<0) delta=-1; // ���ӵ��� ������ ����
	else delta=1; // ����� �Ʒ���

	n = abs((int)ay); // ���ӵ��� ������ �̿�

	if(n<8){ // ������ �ӽ� ��ǥ�� ����
		n=0;
	}
	else if(n<25){
		n=1;
	}
	else if(n<40){
		n=2;
	}
	else{
		n=3;
	}

	for(i=1;i<=n;i++){
		if( isCanMove( x, y+delta, 60, (int)(100*scale) ) && isCanMove( x, (int)(y-60*scale+delta), 60, (int)(100*scale) ) && isCanMove( x, (int)(y-40*scale+delta), 60, (int)(100*scale) ) ){ // y+delta�� �̵�
			DoMove(0,delta);
		}
		else{ // �ٴڰ� ������ �߷� ���ӵ� �ʱ�ȭ
			if(delta>0)	ay=-1000;
			else ay=1;
			break;
		}
	}
}

// �����̱�
void Character::DoMove(int dx, int dy){
	if(dx!=0){ // ��� �ٴ����� ���ϰ�
		if(ay < -100)
			ay=8;
	}
	this->x+=dx;
	this->y+=dy;
}

// ����
void Character::Accel(int state){
	if(state==0){ // ���� ���¶�� ���ӵ��� ������ ����
		if(ax<0) ax+=0.025;
		else if(ax>0) ax-=0.025;
		if(ax>=-0.01 && ax<=0.01) ax = 0;
	}
	else if(state<0){ // �ƴϸ� ������ �ش�.
		ax-=0.025;
	}
	else if(state>0){
		ax+=0.025;
	}
	if(ax>=ac) ax=ac; // ���� ���Ѽ�
	else if(ax<=-ac) ax=-ac;

	if(ay > -100) // �����̸� �߷� ����
		ay++;
}

void Character::MoveBall(){
	int i;
	// �� ������
	if(ball.motion > 0){
		ball.motion ++;
		if(ball.dir) ball.x +=2;
		else ball.x -=2;
		if( !isCanMove(ball.x,ball.y,0,0) ){
			ball.motion = 0;

			for(i=0;i<10;i++){
				ptc.Add( 0 , ball.x , ball.y , 0.5, (rand()%360)*PI/180 );
			}
		}
	}
}

// �����̱�
void Character::Move(int stagenum){
	int i;
	// ���� ����
	if(stagenum>28){
		if(ax>0.5) ax = 0.5;
		if(ax<-0.5) ax = -0.5;
		if(stagenum>31){
			if(ax>0.3) ax = 0.3;
			if(ax<-0.3) ax = -0.3;
		}
	}
	motion++;

	int j;
	int dx = this->x, dy = this->y; // �����̱� �� x, y ��ǥ
	int delta, n = 0; // ������ �Ÿ�, �ӽ� ��ǥ

	if(ax<0) delta = -1; // ���ӵ� ���⿡ ���� ��Ÿ���� ���� ����
	else delta = 1; 

	// �Ǽ� ���ӵ��� ������ �ٲ��ش�.
	an += abs(ax);
	while(1){
		if(an>=1.0){
			an -= 1.0;
			n ++;
		}
		else break;
	}

	for(i=1;i<=n;i++){
		if( isCanMove(x+delta,y-1,60,(int)(100*scale)) == false ){ // �������̸� ����
			ac = 0.8;
		}else ac = 1.0;

		for(j=(int)(-60*scale);j<=0;j++){
			if( isCanMove(x+delta,y+j,60,(int)(100*scale)) == false ){
				if(j>=-2) j=j+1;
				else j=-486;
				break;
			}
		}
		if(j!=-486){
			DoMove(delta,j-1);
		}/*
		else if( isCanMove(x+delta,y-1,60,(int)(100*scale)) == true && isCanMove(x+delta,(int)(y-60*scale),0,(int)(100*scale)) == true )
			DoMove(delta,-1);
		else if( isCanMove(x+delta,y-2,60,(int)(100*scale)) == true && isCanMove(x+delta,(int)(y-60*scale),0,(int)(100*scale)) == true )
			DoMove(delta,-2);
		else if( isCanMove(x+delta,y-3,60,(int)(100*scale)) == true && isCanMove(x+delta,(int)(y-60*scale),0,(int)(100*scale)) == true )
			DoMove(delta,-3);*/
		else // �����ٸ�
			ax=0; // ���ӵ� �ʱ�ȭ
	}

	// �߷� �ֱ�
	Gravity();

	// ���� ��ȭ
	if(talpha<alpha){
		if(alpha>1) alpha-=2;
		else alpha=0;
	}

	// ũ�� ��ȭ
	if(tscale<0.5) tscale=0.0;
	if(tscale>1.0) tscale=1.0;
	if(1.0-scale<0.01) scale=1.0;
	if(scale-tscale>0.01){
		scale-=0.01;
		// �߷� ����
		if(ay<-100) ay=8;
		// ��ƼŬ �ֱ�
		if( ptc.ptc[ ptc.n ].time>=5 || ptc.n==0 ){
			for(i=0;i<3;i++) ptc.Add( 0 , x , y , 1.0 , (rand()%360)*PI/180 );
		}
	}
	else if(scale-tscale<-0.01){
		scale+=0.01;
		y-=1;
		// �߷� ����
		if(ay<-100) ay=8;
		// ��ƼŬ �ֱ�
		if( ptc.ptc[ ptc.n ].time>=5 || ptc.n==0 ){
			for(i=0;i<3;i++) ptc.Add( 0 , x , y , 1.0, (rand()%360)*PI/180 );
		}
	}

	// �������� ��� ��ƼŬ �ֱ�
	if( (x != dx || y != dy) && (ptc.ptc[ ptc.n ].time>=5 || ptc.n==0)){
		ptc.Add( 0 , rand()%40-20 + x , rand()%40-20 + y , 0.5, (rand()%60 - 30)*PI/180 + atan2((double)y-dy,(double)x-dx) );
		// ptc[ ++ptcn ].Set( 0 , x , y , atan2((double)y-dy,(double)x-dx) , img_particle );
	}
}