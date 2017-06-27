#pragma once
#include "_stdafx.h"
#define TARGET_FPS 60

SDL_Texture *load_image(SDL_Renderer *renderer,char *filename)
{
	SDL_Surface *Loaded_IMG = NULL;
	Loaded_IMG = IMG_Load(filename);
	if(Loaded_IMG==NULL)
	{
		printf("%s is not loaded\n",filename);
		return NULL;
	}

	SDL_SetSurfaceRLE(Loaded_IMG,1);
	SDL_SetColorKey(Loaded_IMG,SDL_TRUE,SDL_MapRGB(Loaded_IMG->format,0,255,255));

	SDL_Texture *Return_IMG = NULL;
	Return_IMG = SDL_CreateTextureFromSurface(renderer,Loaded_IMG);
	SDL_FreeSurface(Loaded_IMG);
	return Return_IMG;
}

void apply_surface(int x, int y, SDL_Texture *source, SDL_Renderer *destination, double scale, double angle, bool flip, bool center, SDL_Rect *clip){
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_QueryTexture(source,NULL,NULL,&offset.w,&offset.h);
	if(clip != NULL){ // 스프라이트 이미지이면 오프셋을 나눠준다.
		offset.w = clip->w;
		offset.h = clip->h;
	}
	offset.w = (int)( offset.w * scale );
	offset.h = (int)( offset.h * scale );

	if(center==true){
		offset.x -= offset.w/2;
		offset.y -= offset.h/2;
	}

	if(angle==0 && flip==false){
		SDL_RenderCopy(destination,source,clip,&offset);
	}
	else{
		if(flip==false) SDL_RenderCopyEx(destination,source,clip,&offset,angle,NULL,SDL_FLIP_NONE);
		else SDL_RenderCopyEx(destination,source,clip,&offset,angle,NULL,SDL_FLIP_HORIZONTAL);
	}
}

Uint32 Set_Frame(Uint32 curTicks){ // 프레임 설정
	Uint32 elapsedTicks = SDL_GetTicks() - curTicks, elapsedTicks2 = SDL_GetTicks() - curTicks;
	if (1000 > TARGET_FPS * elapsedTicks)
		SDL_Delay(1000 / TARGET_FPS - elapsedTicks);

	elapsedTicks = SDL_GetTicks() - curTicks;
	printf("%3d ms, %6.1f fps\n", elapsedTicks, 1000.0 / (float)elapsedTicks);

	return elapsedTicks2;
}