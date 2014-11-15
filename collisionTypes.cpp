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
	
	if(!dotTM.initialize(graphics,"pictures\\dot.png"))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dot textures"));

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
	player.setScale(.5);

	for(int i=0; i<MAX_GHOSTS; i++)
	{
		if (!ghosts[i].initialize(this, brickNS::WIDTH, brickNS::HEIGHT, 2, &brickTM))
			throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		ghosts[i].setPosition(VECTOR2(0,0));
		ghosts[i].setX(ghosts[i].getPositionX());
		ghosts[i].setY(ghosts[i].getPositionY());
		ghosts[i].setCurrentFrame(i);
		ghosts[i].setScale(.5f);
		ghosts[i].aiType = i;
	}
	ghosts[0].setPosition(VECTOR2(0,0));
	ghosts[1].setPosition(VECTOR2(GAME_WIDTH - brickNS::WIDTH/2,0));
	ghosts[2].setPosition(VECTOR2(GAME_WIDTH - brickNS::WIDTH/2,GAME_HEIGHT - brickNS::HEIGHT/2));
	ghosts[3].setPosition(VECTOR2(0,GAME_HEIGHT - brickNS::HEIGHT/2));


	for(int i=0; i<MAX_DOTS; i++)
	{
		if (!dots[i].initialize(this, dotNS::WIDTH, dotNS::HEIGHT, 0, &dotTM))
			throw(GameError(gameErrorNS::WARNING, "dot not initialized"));
		dots[i].setActive(true);
	}
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			dots[4*i+j].setX((i+1)*GAME_WIDTH/5);
			dots[4*i+j].setY((j+1)*GAME_HEIGHT/5);
		}
	}

	if (!winTM.initialize(graphics, "pictures\\win.png"))
	   throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));
	
	if(!win.initialize(graphics,GAME_WIDTH,GAME_HEIGHT,0,&winTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));
	win.setX(0);
	win.setY(0);

	if (!loseTM.initialize(graphics, "pictures\\lose.png"))
	   throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing win texture"));

	if(!lose.initialize(graphics,GAME_WIDTH,GAME_HEIGHT,0,&loseTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lose texture"));
	lose.setX(0);
	lose.setY(0);

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
	
	graphics->setBackColor(graphicsNS::CYAN);

	gameState = PLAY;
	timeInState = 0;
	time = 0;
	ghostsActive = false;

	return;
}

//==============================
void CollisionTypes::reset()
{
	ghosts[0].setPosition(VECTOR2(0,0));
	ghosts[1].setPosition(VECTOR2(GAME_WIDTH - brickNS::WIDTH/2,0));
	ghosts[2].setPosition(VECTOR2(GAME_WIDTH - brickNS::WIDTH/2,GAME_HEIGHT - brickNS::HEIGHT/2));
	ghosts[3].setPosition(VECTOR2(0,GAME_HEIGHT - brickNS::HEIGHT/2));

	time = 0;
	ghostsActive = false;
	patternStepIndex = 0;
	for(int i=0; i<MAX_GHOSTS; i++)
	{
		ghosts[i].setPosition(VECTOR2(0,0));
		ghosts[i].setX(ghosts[i].getPositionX());
		ghosts[i].setY(ghosts[i].getPositionY());
	}
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			dots[4*i+j].setPositionX((i+1)*GAME_WIDTH/5);
			dots[4*i+j].setPositionY((j+1)*GAME_HEIGHT/5);
			dots[4*i+j].setActive(true);
		}
	}

	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
	player.setVelocity(VECTOR2(0,0);

}

//===========================================================================
void CollisionTypes::gameStateUpdate()
{
	timeInState += frameTime;
	if(gameState == WIN && timeInState >= 0.25 && input->isKeyDown(ENTER_KEY))
	{
		gameState = PLAY;
		reset();
	}
	if(gameState == LOSE && timeInState >= 0.25 && input->isKeyDown(ENTER_KEY))
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
	gameStateUpdate();
	if(input->isKeyDown(ESC_KEY))
		exitGame();

	time += frameTime;
	if(time >= ACTIVATE_TIME)
	{
		ghostsActive = true;
	}

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
		check = false;
		for(int i=0; i<MAX_DOTS; i++)
		{
			dots[i].update(frameTime);
			if(dots[i].getActive())
			{
				check = true;
			}
		}
		if(check == false)
		{
			gameState = WIN;
			timeInState = 0;
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
	if(gameState == PLAY && ghostsActive)
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

		for (int i=0;i< NUM_WALLS;i++){
			if (walls[i].collidesWith(player, collisionVector)){
				//player.setX(player.getX() - player.getVelocity().x *frameTime);
				//player.setY(player.getY() - player.getVelocity().y *frameTime);
				player.setPositionX(player.getPositionX() - player.getVelocity().x* frameTime);
				player.setPositionY(player.getPositionY() - player.getVelocity().y* frameTime);
			}

		}


		for(int i=0; i<MAX_GHOSTS; i++)
		{
			if(ghosts[i].collidesWith(player,collisionVector))
			{
				collision = true;
				gameState = LOSE;
				timeInState = 0;
			}
		}
		for(int i=0; i<MAX_DOTS; i++)
		{
			if(dots[i].getActive() && dots[i].collidesWith(player,collisionVector))
			{
				collision = true;
				dots[i].setActive(false);
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
		for(int i=0; i<MAX_DOTS; i++)
		{
			if(dots[i].getActive())
			{
				dots[i].draw();
			}
		}
		break;
	case WIN:
		win.draw();
		break;
	case LOSE:
		lose.draw();
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
	loseTM.onLostDevice();
	winTM.onLostDevice();
	wallTM.onLostDevice();
	dotTM.onLostDevice();
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
	loseTM.onResetDevice();
	winTM.onResetDevice();
	wallTM.onResetDevice();
	dotTM.onResetDevice();
	paddleTM.onResetDevice();
	puckTM.onResetDevice();
    Game::resetAll();
    return;
}
