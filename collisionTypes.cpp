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
	
    if (!paddleTM.initialize(graphics,PLAYER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

   if (!brickTM.initialize(graphics,GHOST_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));

   if (!player.initialize(this, paddleNS::WIDTH, paddleNS::HEIGHT, 2, &paddleTM))
		throw(GameError(gameErrorNS::WARNING, "Paddle not initialized"));

   if (!wallTM.initialize(graphics, "pictures\\wall.png"))
	   throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

   int wallpos[] = {100,200,300,200};
   int wallrotation[] = {90,90};
   for (int i=0;i<NUM_WALLS;i++){
	   if (!walls[i].initialize(this, WallNS::WIDTH, WallNS::HEIGHT, 0, &wallTM))
		   throw(GameError(gameErrorNS::WARNING, "wall not initialized"));
	   walls[i].setRadians(wallrotation[i]/180.0*PI);
	   walls[i].setX(wallpos[i*2]);
	   walls[i].setY(wallpos[i*2+1]);
   }

	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_PADDLE);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(.5);

	for(int i=0; i<MAX_GHOSTS; i++)
	{
		if (!ghosts[i].initialize(this, brickNS::WIDTH, brickNS::HEIGHT, 2, &brickTM))
			throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		ghosts[i].setPosition(VECTOR2(0,0));
		ghosts[i].setCollision(entityNS::BOX);
		ghosts[i].setEdge(COLLISION_BOX_PUCK);
		ghosts[i].setX(ghosts[i].getPositionX());
		ghosts[i].setY(ghosts[i].getPositionY());
		ghosts[i].setCurrentFrame(i);
		ghosts[i].setScale(.5f);
	}

	//patternsteps
	patternStepIndex = 0;
	for (int i = 0; i< maxPatternSteps; i++)
	{
		patternSteps[i].initialize(&ghosts[0]);
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
			player.left();
		if(input->isKeyDown(PADDLE_RIGHT))
			player.right();
		if(input->isKeyDown(PADDLE_UP))
			player.up();
		if(input->isKeyDown(PADDLE_DOWN))
			player.down();
		player.update(frameTime);
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			ghosts[i].update(frameTime);
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
			patternStepIndex = 0;
		if (patternSteps[patternStepIndex].isFinished())
			patternStepIndex++;
		patternSteps[patternStepIndex].update(frameTime);
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			ghosts[i].ai(frameTime, player);
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
			if(ghosts[i].collidesWith(player,collisionVector))
			{
				collision = true;
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
		for (int i=0;i<NUM_WALLS;i++){
			walls[i].draw();
		}
		player.draw();
		for(int i=0; i<MAX_GHOSTS; i++)
		{
			ghosts[i].draw();
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
