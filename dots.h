#define WIN32_LEAN_AND_MEAN
#ifndef DOT_H                 // Prevent multiple definitions if this 
#define DOT_H   
class Brick;

#include "entity.h"
#include "constants.h"
#include "graphics.h"

namespace dotNS
{
    const int WIDTH = 32;                   // image width
    const int HEIGHT = 32;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED_X = 0;                
	const float SPEED_Y = -0;
 
}

// inherits from Entity class
class Dot : public Entity
{
private:

public:
	Dot();
	void update(float frameTime);
};
#endif
