
#include "SoundAdapter.h"
#ifdef _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#endif
#ifdef __linux__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif
#include <cstdlib>
#include <iostream>
 
//-------------------------------------------------------------------------------------
SoundAdapter::SoundAdapter(void)
{
	bool success = true;

	// Make sure the sounds are initialized to null or your check is useless! (garbage data can be read as !NULL).
	for (int i = 0; i < NUM_SOUNDS; i++) {
		gameSounds[0] = NULL;
	}

	/* Initialize all SDL subsystems */
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		printf( "SDL not initialized! SDL Error: %s\n", Mix_GetError() );
		success = false;
	}

 	/* Initialize SDL_mixer */
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		printf( "SDL_mixer not initialized! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	/* Load sound effects */

		/* Since we havent figured out what sound's we're using yet, I just used a single test sound. 
			The other gameSounds I left as NULL until we find soudns to use. */
#ifdef __linux__
	gameSounds[PADDLE_BOUNCE] = Mix_LoadWAV("../Assets/ball.wav");
	gameSounds[WALL_BOUNCE] = Mix_LoadWAV("../Assets/wall.wav");
#endif
#ifdef _WIN32
	gameSounds[PADDLE_BOUNCE] = Mix_LoadWAV("../../../Game-Tech-12/Assignment-2/Assets/ball.wav");
	gameSounds[WALL_BOUNCE] = Mix_LoadWAV("../../../Game-Tech-12/Assignment-2/Assets/wall.wav");
#endif
	/*for ( int i = 0 ; i < NUM_SOUNDS ; i++ ) {
		if( gameSounds[i] == NULL ) {
			printf( "Failed to load clickTest sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
			success = false;
		}
	}*/

}
//-------------------------------------------------------------------------------------
SoundAdapter::~SoundAdapter(void) {
}
 
/* Play a sound based on the soundID. (Sound IDs are identified in the header) */
void SoundAdapter::playSound(int soundID) {
	if ( gameSounds[soundID] != NULL ) {
		Mix_PlayChannel( -1, gameSounds[soundID], 0 );
	}
	else {
		std::cout << "No sound file defined for soundID: " << soundID << std::endl;
	}

	std::cout << "Sound Played" << std::endl;
}

/* Free resources */
void SoundAdapter::destroy(void) {
	/* Free Sounds */
	for ( int i = 0 ; i < NUM_SOUNDS ; i++ ) {
		if ( gameSounds[i] != NULL )
			Mix_FreeChunk( gameSounds[i] );
		gameSounds[i] = NULL;
	}
	//Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}