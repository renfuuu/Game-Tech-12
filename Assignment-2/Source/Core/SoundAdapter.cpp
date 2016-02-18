
#include "SoundAdapter.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <cstdlib>
#include <iostream>
 
//-------------------------------------------------------------------------------------
SoundAdapter::SoundAdapter(void)
{
	bool success = true;

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
	gameSounds[0] = NULL;
	gameSounds[1] = NULL;
	gameSounds[2] = NULL;
	gameSounds[3] = NULL;
	gameSounds[4] = Mix_LoadWAV( "scratch.wav" );
	if( gameSounds[4] == NULL ) {
		printf( "Failed to load clickTest sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

}
//-------------------------------------------------------------------------------------
SoundAdapter::~SoundAdapter(void) {
}
 
/* Play a sound based on the soundID. (Sound IDs are identified in the header) */
void SoundAdapter::playSound(int soundID) {
	if ( gameSounds[soundID] != NULL )
		Mix_PlayChannel( -1, gameSounds[soundID], 0 );
	else
		std::cout << "No sound file defined for soundID: " << soundID << std::endl;
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