/* Frank Cerasoli - University of Texas - CS354r
	This class is an SDL sound adapter for use with ogre3d.
	Sound files are loaded in through this class so that they can be played by calling "playSound(id)" 
	  on the desired sound id. */

#ifndef __SoundAdapter_h_
#define __SoundAdapter_h_

#include <SDL/SDL_mixer.h>

#define NUM_SOUNDS 5  //WallBounce, PaddleBounce, GameWin, GameLoss, ClickTesting

class SoundAdapter {
	public:
		SoundAdapter(void);
		~SoundAdapter(void);
		void playSound(int);
		void destroy(void);
	private:
		Mix_Chunk *gameSounds[NUM_SOUNDS];

};
 
#endif // #ifndef __SoundAdapter_h_