#pragma once
#include "_stdafx.h"

void Envt::Print(SDL_Renderer *screen, bool dynamic){
	int i;

	// ��� ���� ���� �������
	if(dynamic){
		for(i=1;i<=n;i++){
			// ��
			if( e[i].type == 0 ){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ 0 ] );
			}
		}
	}
	// ȯ�� ���
	for(i=1;i<=n;i++){
		if(e[i].type==7 || e[i].type==8) continue; // ���� ����Ʈ�� ������� �ʴ´�
		// ���� ���
		if(dynamic){
			// ��
			if(e[i].type == 1){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ 0 ][ (int)(e[i].motion/12) %4 ] );
			}
			// ��
			else if(e[i].type == 2){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ (int)(e[i].motion/10) %8 ] );
			}
			// ����
			else if(e[i].type == 3){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true, &clip[e[i].type][ (int)(e[i].die/10) ][ 0 ] );
			}
			// ����
			else if( e[i].type == 4 || e[i].type == 5 ){
				// ������ �������� ���̸� ������ ȿ���� �ش�
				if(e[i].motion<300)	SDL_SetTextureAlphaMod(img[e[i].type],(Uint8)((double)e[i].motion/300*255));

				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true);

				if(e[i].motion<300)	SDL_SetTextureAlphaMod(img[e[i].type],255);
			}
			// ������
			else if(e[i].type==6){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, e[i].angle, false, true);
			}
			// ��Ÿ : ����
			else if(e[i].type!=0){
				apply_surface(e[i].x, e[i].y, img[ e[i].type ], screen, 1.0, 0, false, true);
			}
		}

		// ���� ���
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
		// ��
		if(e[i].type == 1){
			e[i].motion++;
			// ȯ���� �������� ���̶��
			if(e[i].die>0){
				// �Ҹ� ���� ����
				e[i].die++;
			}
			// ������ ��������
			if(e[i].die>=1){
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// ��
		else if(e[i].type == 2){
			e[i].motion++;
			// ȯ���� �������� ���̶��
			if(e[i].die>0){
				// �Ҹ� ���� ����
				e[i].die++;
			}
			// ������ ��������
			if(e[i].die>=60){
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// ����
		else if(e[i].type == 3){
			e[i].motion++;
			// ȯ���� �������� ���̶��
			if(e[i].die>0){
				// �Ҹ� ���� ����
				e[i].die++;
			}
			// ������ ��������
			if(e[i].die>=70){
				// �ҷ� �ٲ��..!
				e[i].type=1;
				e[i].die=0;	e[i].motion=0; e[i].y += 31;
				continue;
			}
		}
		// ����
		else if( e[i].type == 4 || e[i].type == 5 ){
			e[i].motion++;
			// ��������
			if(e[i].motion<300){
				e[i].cy -= 0.125;
				e[i].oy = (int)e[i].cy;
			}
			else{
				// ������ �������� �ش�.
				if(e[i].type==4) e[i].cx+=0.125;
				else if(e[i].type==5) e[i].cx-=0.125;
				// �սǵս� ȿ��
				e[i].cy = e[i].oy + 3*sin((double)(e[i].motion-300)/50);

				// �� ������~
				if(e[i].motion%10==0){
					n++;
					e[n].type = 0;
					e[n].x = e[i].x + (e[i].motion*3)%80 - 40;
					e[n].y = e[i].oy;
					e[n].gy = 1.0;
					e[n].cx = e[n].x; e[n].cy = e[n].y;	e[n].oy = e[n].y; e[n].die = 0;	e[n].motion = 0;
				}
			}
			// �Ǽ���ǥ�� ������ǥ��
			e[i].x = (int)e[i].cx;
			e[i].y = (int)e[i].cy;

			if( e[i].x < -50 || e[i].x > WIDTH+300 ){ // �� ������ �����
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// ������
		else if( e[i].type == 6 ){
			// �߷� ���ӵ�
			if(e[i].ay != 0) e[i].ay+=0.03;

			// �и��� ���� ���� ���ӵ��� ���� ����
			if(e[i].whose == false){
				e[i].cx+=e[i].ax;
			
				// �ƴϸ� �������� ����
				if(e[i].ax < 0) e[i].ax += 0.005;
				else if(e[i].ax > 0) e[i].ax -= 0.005;
				if( -0.0025 <= e[i].ax && e[i].ax <= 0.0025 ) e[i].ax = 0;
			}

			if(e[i].ax>1.0) e[i].ax=1.0;
			if(e[i].ax<-1.0) e[i].ax=-1.0;

			e[i].cy+=e[i].ay;

			e[i].angle+=e[i].ax * PI;

			if( e[i].y < -16 || e[i].y > HEIGHT+16 ||  e[i].x < -16 || e[i].x > WIDTH+16 ){ // �� ������ �����
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// ����
		else if( e[i].type == 9 ){
			e[i].y+=3;

			if( e[i].y < -27 || e[i].y > HEIGHT+27 || e[i].die>0 ){ // �� ������ ����ų� ���� ������
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
		// ��
		else if( e[i].type == 0 ){
			// ȯ���� �������� ���̶��
			if(e[i].die>0){
				// �Ҹ� ���� ����
				e[i].die++;
			}
			// �� ������
			else{
				e[i].gy+=0.01;
				e[i].cy += e[i].gy;
				e[i].y = (int)e[i].cy;
			}
			// ������ ��������
			if(e[i].die>=30 || e[i].y>HEIGHT+20 || e[i].x>WIDTH || e[i].x<0){
				// ����
				for(j=i;j<n;j++) e[j] = e[j+1];
				n--;
				i--;
				continue;
			}
		}
	}
}