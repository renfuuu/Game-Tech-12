noinst_HEADERS = Application.h MultiPlatformHelper.h OISManager.h SceneHelper.h CoreConfig.h OgreText.h SoundAdapter.h ScoreManager.h TextOverlay.h GameObject.h Simulator.h BulletContactCallback.h CollisionContext.h OgreMotionState.h Ball.h Paddle.h Wall.h 

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11
assignment2_SOURCES = Application.cpp main.cpp OISManager.cpp SoundAdapter.cpp ScoreManager.cpp Simulator.cpp TextOverlay.cpp GameObject.cpp OgreMotionState.cpp CollisionContext.cpp BulletContactCallback.cpp Ball.cpp Paddle.cpp Wall.cpp 
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -lSDL -lSDL_mixer

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
