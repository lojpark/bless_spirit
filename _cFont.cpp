#pragma once
#include "_stdafx.h"
#include "han2unicode.h"

void Font::Set(SDL_Renderer *screen, char *font_name, int size){
	this->screen = screen;

	font = TTF_OpenFont(font_name,size);
}
void Font::Destroy(){
	TTF_CloseFont(font);
}

void Font::Print(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, bool center){
	Uint16 temp[1000];
	SDL_Color color = {R,G,B};

	han2unicode(text, temp);
	messages = TTF_RenderUNICODE_Blended(font, temp, color);

	message = SDL_CreateTextureFromSurface(screen,messages);

	apply_surface(x,y,message,screen,1.0,0,false,center);

	SDL_DestroyTexture(message);
	SDL_FreeSurface(messages);
}
void Font::PrintInput(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, bool center){
	SDL_Color color = {R,G,B};
	messages = TTF_RenderUTF8_Blended(font, text, color);

	message = SDL_CreateTextureFromSurface(screen,messages);

	apply_surface(x,y,message,screen,1.0,0,false,center);

	SDL_DestroyTexture(message);
	SDL_FreeSurface(messages);
}
void Font::PrintAlpha(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha, bool center){
	SDL_Color color = {R,G,B};
	messages = TTF_RenderUTF8_Blended(font, text, color);

	message = SDL_CreateTextureFromSurface(screen,messages);

	SDL_SetTextureBlendMode(message,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(message,alpha);
	apply_surface(x,y,message,screen,1.0,0,false,center);

	SDL_DestroyTexture(message);
	SDL_FreeSurface(messages);
}
void Font::PrintAlpha2(int x, int y, char *text, Uint8 R, Uint8 G, Uint8 B, Uint8 alpha, bool center){
	Uint16 temp[1000];
	SDL_Color color = {R,G,B};

	han2unicode(text, temp);
	messages = TTF_RenderUNICODE_Blended(font, temp, color);

	message = SDL_CreateTextureFromSurface(screen,messages);

	SDL_SetTextureBlendMode(message,SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(message,alpha);
	apply_surface(x,y,message,screen,1.0,0,false,center);

	SDL_DestroyTexture(message);
	SDL_FreeSurface(messages);
}