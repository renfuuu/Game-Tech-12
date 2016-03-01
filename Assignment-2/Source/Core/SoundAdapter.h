/* Frank Cerasoli - University of Texas - CS354r
	This class is an SDL sound adapter for use with ogre3d.
	Sound files are loaded in through this class so that they can be played by calling "playSound(id)" 
	  on the desired sound id. */
#pragma once

#ifdef _WIN32
#include <SDL_mixer.h>
#endif
#ifdef __linux__
#include <SDL/SDL_mixer.h>
#endif

class SoundAdapter {
	public:
		enum { WALL_BOUNCE, PADDLE_BOUNCE, GAME_WIN, GAME_LOSS, HEADSHOT, NUM_SOUNDS } sounds;
		SoundAdapter(void);
		~SoundAdapter(void);
		void playSound(int);
		void startMusic(void);
		void mute(void);
		void muteMusic(bool mute);
		void muteSounds(bool mute);
		void destroy(void);
	private:
		bool muted; //overall muted
		bool mutedM; //music
		bool mutedS; //sounds
		Mix_Music *music = NULL;
		Mix_Chunk *gameSounds[NUM_SOUNDS];

};