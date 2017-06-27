#pragma once
#include "_stdafx.h"

void Sound_Load(FMOD_SYSTEM **g_System, char *filename, FMOD_SOUND **sound, bool loop){
	if(loop) FMOD_System_CreateSound(*g_System,filename,FMOD_LOOP_NORMAL,0,sound);
	else FMOD_System_CreateSound(*g_System,filename,FMOD_DEFAULT,0,sound);
}
void Sound_Play(FMOD_SYSTEM **g_System, FMOD_CHANNEL **channel, FMOD_SOUND **sound){
	FMOD_Channel_Stop(*channel);
	FMOD_System_PlaySound(*g_System, FMOD_CHANNEL_FREE, *sound, 0, channel);
}
void Sound_Stop(FMOD_CHANNEL **channel){
	FMOD_Channel_Stop(*channel);
}