// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"

//=============================================================================
// Constructor
//=============================================================================
CollisionTypes::CollisionTypes()
{
	//nothing here, move on
}

//=============================================================================
// Destructor
//=============================================================================
CollisionTypes::~CollisionTypes()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CollisionTypes::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError


    if (!paddleTM.initialize(graphics,PADDLE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

   if (!brickTM.initialize(graphics,PUCK_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));

	if (!paddle.initialize(this, paddleTM.getWidth(), paddleTM.getHeight(), 0,&paddleTM))
		throw(GameError(gameErrorNS::WARNING, "Paddle not initialized"));
	paddle.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    paddle.setCollisionType(entityNS::BOX);
    paddle.setEdge(COLLISION_BOX_PADDLE);
    paddle.setCollisionRadius(COLLISION_RADIUS);
	paddle.setScale(.5);

	for(int i=0; i<MAX_GHOSTS; i++)
	{
		if (!bricks[i].initialize(this, 0, 0, 0,&brickTM))
			throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		bricks[i].setPosition(VECTOR2(0,0));
		bricks[i].setCollision(entityNS::BOX);
		bricks[i].setEdge(COLLISION_BOX_PUCK);
		bricks[i].setX(bricks[i].getPositionX());
		bricks[i].setY(bricks[i].getPositionY());
		bricks[i].setScale(.5);
	}

	//patternsteps
	patternStepIndex = 0;
	for (int i = 0; i< maxPatternSteps; i++)
	{
		patternSteps[i].initialize(&bricks[0]);
		patternSteps[i].setActive();
	}
	patternSteps[0].setAction(RIGHT);
	patternSteps[0].setTimeForStep(1);

	patternSteps[1].setAction(DOWN);
	patternSteps[1].setTimeForStep(1);

	patternSteps[2].setAction(LEFT);
	patternSteps[2].setTimeForStep(1);

	patternSteps[3].setAction(UP);
	patternSteps[3].setTimeForStep(1);
	


	gameState = PLAY;
	timeInState = 0;

	return;
}

//==============================
void CollisionTypes::reset()
{

}

//===========================================================================
void CollisionTypes::gameStateUpdate()
{
	if(gameState == WIN && timeInState >= 0.5 && input->isKeyDown(ENTER_KEY))
	{
		gameState = PLAY;
		reset();
	}
	if(gameState == LOSE && timeInState >= 0.5 && input->isKeyDown(ENTER_KEY))
	{
		gameState = PLAY;
		reset();
	}
}
//=============================================================================
// Update all game items
//=============================================================================
void CollisionTypes::update()
{
	if(input->isKeyDown(ESC_KEY))
		exitGame();

	switch(gameState)
	{
	case PLAY:
		if(input->isKeyDown(PADDLE_LEFT))
			paddle.left();
		if(input->isKeyDown(PADDLE_RIGHT))
			paddle.right();
		if(input->isKeyDown(PADDLE_UP))
			paddle.up();
		if(input->isKeyDown(PADDLE_DOWN))
			paddle.down();
		paddle.update(frameTime);
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			bricks[i].update(frameTime);
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	if(gameState == PLAY)
	{
		if (patternStepIndex == maxPatternSteps)
			return;
		if (patternSteps[patternStepIndex].isFinished())
			patternStepIndex++;
		patternSteps[patternStepIndex].update(frameTime);
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			bricks[i].ai(frameTime, paddle);
		}
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void CollisionTypes::collisions()
{
	if(gameState == PLAY)
	{
	    collisionVector = D3DXVECTOR2(0,0);
		collision = false;
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			if(bricks[i].collidesWith(paddle,collisionVector))
			{
				collision = true;
				audio->playCue(BEEP1);
			}
		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	
	switch(gameState)
	{
	case PLAY:
		paddle.draw();
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			bricks[i].draw();
		}
		break;
	case WIN:
		break;
	case LOSE:
		break;
	}
	
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CollisionTypes::releaseAll()
{
	paddleTM.onLostDevice();
	puckTM.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CollisionTypes::resetAll()
{
	paddleTM.onResetDevice();
	puckTM.onResetDevice();
    Game::resetAll();
    return;
}
