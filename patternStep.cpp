#include "patternStep.h"

PatternStep::PatternStep()
{
    active = false;                 
	timeInStep = 0;
	entity = NULL;  
	timeForStep = 0;
	action = NONE;
}

void PatternStep::initialize(Brick *e)
{
	entity = e;
}

void PatternStep::update(float frameTime)
{
	if (!active) return;
	timeInStep += frameTime;
	
	switch (action)
	{
	case NONE:
		entity->setVelocity(D3DXVECTOR2(0,0));
		break;
	case UP:
		entity->setVelocity(D3DXVECTOR2(0, -1));
		if(entity->getY() < 0)
		{
			timeInStep = 0;
			active = false;
		}
		break;
	case DOWN:
		entity->setVelocity(D3DXVECTOR2(0, 1));
		if(entity->getY() + entity->getScale()*entity->getHeight() > GAME_HEIGHT)
		{
			timeInStep = 0;
			active = false;
		}
		break;
	case RIGHT:
		entity->setVelocity(D3DXVECTOR2(1,0));
		if(entity->getX() + entity->getScale()*entity->getWidth() > GAME_WIDTH)
		{
			timeInStep = 0;
			active = false;
		}
		break;
	case LEFT:
		entity->setVelocity(D3DXVECTOR2(-1,0));
		if(entity->getX() < 0)
		{
			timeInStep = 0;
			active = false;
		}
		break;
	case UPLEFT:
		entity->setVelocity(D3DXVECTOR2(-1,-1));
		break;
	case TRACK:
		entity->vectorTrack();
		break;
	}
}