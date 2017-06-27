#pragma once
#include "_stdafx.h"

void BackGroundImage::Set(SDL_Renderer *screen){
	img_bgi[0] = load_image(screen,"image\\black.png"); // µÞ¹è°æ
	img_bgi[1] = load_image(screen,"image\\background\\back1.png"); // µÞ¹è°æ
	img_bgl[1][1] = load_image(screen,"image\\background\\light\\backlight11.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[1][1],SDL_BLENDMODE_ADD);
	img_bgl[1][2] = load_image(screen,"image\\background\\light\\backlight12.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[1][2],SDL_BLENDMODE_ADD);
	img_bgl[1][3] = load_image(screen,"image\\background\\light\\backlight13.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[1][3],SDL_BLENDMODE_ADD);
	img_bgi[2] = load_image(screen,"image\\background\\back2.png"); // µÞ¹è°æ
	img_bgl[2][1] = load_image(screen,"image\\background\\light\\backlight21.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[2][1],SDL_BLENDMODE_BLEND);
	img_bgl[2][2] = load_image(screen,"image\\background\\light\\backlight22.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[2][2],SDL_BLENDMODE_ADD);
	img_bgi[3] = load_image(screen,"image\\background\\back3.png"); // µÞ¹è°æ
	img_bgl[3][1] = load_image(screen,"image\\background\\light\\backlight31.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[3][1],SDL_BLENDMODE_ADD);
	img_bgl[3][2] = load_image(screen,"image\\background\\light\\backlight32.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[3][2],SDL_BLENDMODE_ADD);
	img_bgl[3][3] = load_image(screen,"image\\background\\light\\backlight33.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[3][3],SDL_BLENDMODE_ADD);
	img_bgi[4] = load_image(screen,"image\\background\\back4.png"); // µÞ¹è°æ
	SDL_SetTextureBlendMode(img_bgi[4],SDL_BLENDMODE_BLEND);
	img_bgl[4][1] = load_image(screen,"image\\background\\light\\backlight41.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[4][1],SDL_BLENDMODE_ADD);
	img_bgl[4][2] = load_image(screen,"image\\background\\light\\backlight42.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[4][2],SDL_BLENDMODE_ADD);
	img_bgl[4][3] = load_image(screen,"image\\star.png"); // º°
	SDL_SetTextureBlendMode(img_bgl[4][3],SDL_BLENDMODE_ADD);
	img_bgi[5] = load_image(screen,"image\\background\\back5.png"); // µÞ¹è°æ
	img_bgl[5][1] = load_image(screen,"image\\background\\light\\backlight51.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[5][1],SDL_BLENDMODE_ADD);
	img_bgi[6] = load_image(screen,"image\\background\\back6.png"); // µÞ¹è°æ
	img_bgl[6][1] = load_image(screen,"image\\background\\light\\backlight61.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[6][1],SDL_BLENDMODE_ADD);
	img_bgi[7] = load_image(screen,"image\\background\\back7.png"); // µÞ¹è°æ
	img_bgl[7][1] = load_image(screen,"image\\background\\light\\backlight71.png"); // µÞ¹è°æ Á¶¸í
	SDL_SetTextureBlendMode(img_bgl[7][1],SDL_BLENDMODE_ADD);

	img_glow = load_image(screen,"image\\glow.png"); // ±Û·Î¿ì È¿°ú
	SDL_SetTextureBlendMode(img_glow,SDL_BLENDMODE_MOD);

	end.img1 = load_image(screen,"image\\glow1.png"); // ¿£µå Æ÷ÀÎÆ®
	end.img2 = load_image(screen,"image\\glow2.png");
	SDL_SetTextureBlendMode(end.img1,SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(end.img2,SDL_BLENDMODE_ADD);
	SDL_SetTextureColorMod(end.img1,96,255,255);
	SDL_SetTextureColorMod(end.img2,128,255,255);
	end.ptc.Set(screen);
	SDL_SetTextureColorMod(end.ptc.img,39,127,255);
	SDL_SetTextureBlendMode(end.ptc.img,SDL_BLENDMODE_ADD);

	// ¹ø°³
	img_light = load_image(screen,"image\\light.png");
	SDL_SetTextureBlendMode(img_light,SDL_BLENDMODE_BLEND);

	glow1=glow2=0.0;
}

void BackGroundImage::Print_Back(SDL_Renderer* screen, bool blur){
	static double x=0;
	static int star[3][5]={{0,0,0,0,0},{100,200,500,600,700},{100,200,150,200,80}};
	int i;
	
	// ºÎµå·¯¿î È­¸é ÀüÈ¯
	if(alpha<255){
		if(alpha==0) SDL_SetTextureAlphaMod(img_bgi[ltype],255);
		apply_surface(0, 0, img_bgi[ltype], screen, 1.0, 0, false, false);

		if(type==4 && blur){
			if(alpha<192){
				apply_surface(0, 0, img_bgi[0], screen, 1.0, 0, false, false);
				SDL_SetTextureAlphaMod(img_bgi[type],alpha);
				alpha+=2;
			}
		}
		else{
			SDL_SetTextureAlphaMod(img_bgi[type],alpha);
			alpha+=2;
		}
	}

	apply_surface(0, 0, img_bgi[type], screen, 1.0, 0, false, false);

	switch(type){
	case 1:
		SDL_SetTextureAlphaMod(img_bgl[1][1],(Uint8)(155+100*sin(x/60))*alpha/255);
		SDL_SetTextureAlphaMod(img_bgl[1][2],(Uint8)(155+100*sin(x/50))*alpha/255);
		SDL_SetTextureAlphaMod(img_bgl[1][3],(Uint8)(195+60*sin(x/80))*alpha/255);
		apply_surface((int)(10+10*sin(x/50)), 0, img_bgl[1][1], screen, 1.0, 0, false, false);
		apply_surface((int)(5+10*sin(x/60)), 0, img_bgl[1][2], screen, 1.0, 0, false, false);
		apply_surface(0, 0, img_bgl[1][3], screen, 1.0, 0, false, false);
		break;
	case 2:
		SDL_SetTextureAlphaMod(img_bgl[2][1],(Uint8)(155+100*sin(x/60))*alpha/255);
		apply_surface((int)(50*sin(x/50)), -25, img_bgl[2][1], screen, 1.0, 0, false, false);
		SDL_SetTextureAlphaMod(img_bgl[2][1],(Uint8)(155+100*sin(x/40))*alpha/255);
		apply_surface((int)(70*sin(x/50)), 50, img_bgl[2][1], screen, 1.0, 0, false, false);
		SDL_SetTextureAlphaMod(img_bgl[2][2],(Uint8)(155+100*sin(x/50))*alpha/255);
		apply_surface(0, 0, img_bgl[2][2], screen, 1.0, 0, false, false);
		break;
	case 3:
		SDL_SetTextureAlphaMod(img_bgl[3][1],(Uint8)(155+100*sin(x/60))*alpha/255);
		SDL_SetTextureAlphaMod(img_bgl[3][2],(Uint8)(155+100*sin(x/50))*alpha/255);
		SDL_SetTextureAlphaMod(img_bgl[3][3],(Uint8)(195+60*sin(x/80))*alpha/255);
		apply_surface((int)(10+10*sin(x/50)), 0, img_bgl[3][1], screen, 1.0, 0, false, false);
		apply_surface((int)(5+10*sin(x/60)), 0, img_bgl[3][2], screen, 1.0, 0, false, false);
		apply_surface(0, 0, img_bgl[3][3], screen, 1.0, 0, false, false);
		break;
	case 4:
		if(alpha<255) SDL_SetTextureAlphaMod(img_bgl[4][1],(Uint8)((double)alpha/1.5));
		SDL_SetTextureAlphaMod(img_bgl[4][2],(Uint8)(225+30*sin(x/100))*alpha/255);
		apply_surface((int)(50*sin(x/100)), 0, img_bgl[4][1], screen, 1.0, 0, false, false);
		apply_surface(0, 0, img_bgl[4][2], screen, 1.0, 0, false, false);
		// º°
		for(i=0;i<5;i++){
			if(star[0][i]>0){
				apply_surface(star[1][i], star[2][i], img_bgl[4][3], screen, (double)star[0][i]/100.0, star[0][i]*3, false, true);
				star[0][i]--;
			}
			else{
				if(rand()%100+1==10){
					star[0][i]=100;
				}
			}
		}
		break;
	case 5:
		SDL_SetTextureAlphaMod(img_bgl[5][1],(Uint8)(110+75*sin(x/60))*alpha/255);
		apply_surface(0, 0, img_bgl[5][1], screen, 1.0, 0, false, false);
		break;
	case 6:
		SDL_SetTextureAlphaMod(img_bgl[6][1],(Uint8)(55+50*sin(x/60))*alpha/255);
		apply_surface((int)(10*sin(x/50)), 0, img_bgl[6][1], screen, 1.0, 0, false, false);
		break;
	case 7:
		SDL_SetTextureAlphaMod(img_bgl[7][1],(Uint8)(180+75*sin(x/60))*alpha/255);
		apply_surface(0, 0, img_bgl[7][1], screen, 1.0, 0, false, false);
		break;
	}

	x++;
}

void BackGroundImage::Print_Shadow(SDL_Renderer* screen){
	apply_surface(5, 5, img_shadow, screen, 1.0, 0, false, false);
}

void BackGroundImage::Print_Texture(SDL_Renderer* screen){
	apply_surface(0, 0, img_texture, screen, 1.0, 0, false, false);
}

void BackGroundImage::light(SDL_Renderer *screen){
	lp -= lpow;
	if(lp<=0){
		return;
	}

	SDL_SetTextureAlphaMod(img_light, lp);
	apply_surface(0, 0, img_light, screen, 1.0, 0.0, false, false);
	SDL_SetTextureAlphaMod(img_light, 255);
}

void BackGroundImage::Print_Endpoint(SDL_Renderer* screen){
	int i;

	end.motion+=0.75;
	SDL_SetTextureAlphaMod(end.img1,(Uint8)(223 + 32*sin(end.motion/30)));
	SDL_SetTextureAlphaMod(end.img2,(Uint8)(223 + 32*sin(end.motion/30)));

	if(end.ptc.ptc[end.ptc.n].time>=250 || end.ptc.n==0){
		for(i=0;i<5;i++){
			end.ptc.Add( 0 , end.x , end.y , 0.5, (rand()%360)*PI/180 );
		}
	}

	apply_surface(end.x, end.y, end.img1, screen, 1.0, end.motion, false, true);
	apply_surface(end.x, end.y, end.img2, screen, 0.75, end.motion*(-1), false, true);
	end.ptc.Print(screen);
}

void BackGroundImage::Destroy(){
	SDL_DestroyTexture(img_bgi[0]);
	SDL_DestroyTexture(img_bgi[1]);
	SDL_DestroyTexture(img_bgl[1][1]);
	SDL_DestroyTexture(img_bgl[1][2]);
	SDL_DestroyTexture(img_bgl[1][3]);
	SDL_DestroyTexture(img_bgi[2]);
	SDL_DestroyTexture(img_bgl[2][1]);
	SDL_DestroyTexture(img_bgl[2][2]);
	SDL_DestroyTexture(img_bgi[3]);
	SDL_DestroyTexture(img_bgl[3][1]);
	SDL_DestroyTexture(img_bgl[3][2]);
	SDL_DestroyTexture(img_bgl[3][3]);
	SDL_DestroyTexture(img_bgi[4]);
	SDL_DestroyTexture(img_bgl[4][1]);
	SDL_DestroyTexture(img_bgl[4][2]);
	SDL_DestroyTexture(img_bgl[4][3]);
	SDL_DestroyTexture(img_bgi[5]);
	SDL_DestroyTexture(img_bgl[5][1]);
	SDL_DestroyTexture(img_bgi[6]);
	SDL_DestroyTexture(img_bgl[6][1]);
	SDL_DestroyTexture(img_bgi[7]);
	SDL_DestroyTexture(img_bgl[7][1]);
	SDL_FreeSurface(img_maps);
	SDL_FreeSurface(img_textures);
	SDL_DestroyTexture(img_texture);
	SDL_DestroyTexture(img_shadow);

	SDL_DestroyTexture(img_glow);
	SDL_DestroyTexture(end.img1);
	SDL_DestroyTexture(end.img2);
	end.ptc.Destroy();

	SDL_DestroyTexture(img_light);
}