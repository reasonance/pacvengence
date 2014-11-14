#include "bricks.h"

//=============================================================================
// default constructor
//=============================================================================
Brick::Brick() : Entity()
{
    spriteData.width = brickNS::WIDTH;           
    spriteData.height = brickNS::HEIGHT;
    spriteData.x = brickNS::X;                   // location on screen
    spriteData.y = brickNS::Y;
    spriteData.rect.bottom = brickNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = brickNS::WIDTH;
    
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = brickNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -brickNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 100;
	targetSensed = false;
}

bool Brick::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Brick::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Brick::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Brick::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
/*	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
	{
		setPosition(D3DXVECTOR2(0,getPositionY()));
	}
	if (getPositionX() < 0)
	{
		setPosition(D3DXVECTOR2(GAME_WIDTH-Image::getWidth()*Image::getScale(),getPositionY()));
	}
	if (getPositionY() + Image::getHeight()*Image::getScale() > GAME_HEIGHT)
	{
		setPosition(D3DXVECTOR2(getPositionX(),0));
	}
	if (getPositionY() < 0)
	{
		setPosition(D3DXVECTOR2(getPositionX(),GAME_WIDTH-Image::getHeight()*Image::getScale()));
	}
	*/
	velocity = D3DXVECTOR2(0,0);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
}

void Brick::evade()
{
	//add code here
	VECTOR2 dir = *getCenter() - *targetEntity.getCenter();
	D3DXVec2Normalize(&dir,&dir);
	setVelocity(dir);
	return;
}

void Brick::deltaTrack()
{
	VECTOR2 tar = *targetEntity.getCenter();
	tar -= *getCenter();
	if(tar.x < 0)
		tar.x = -1;
	else
		tar.x = 1;
	if(tar.y < 0)
		tar.y = -1;
	else
		tar.y = 1;
	D3DXVec2Normalize(&tar,&tar);
	setVelocity(tar);
	//add code here
}
void Brick::vectorTrack()
{
	VECTOR2 dir = *targetEntity.getCenter() - *getCenter();
	D3DXVec2Normalize(&dir,&dir);
	setVelocity(dir);
	//add code here
}

void Brick::sense()
{
	VECTOR2 dir = *targetEntity.getCenter() - *getCenter();
	float distSq = D3DXVec2LengthSq(&dir);
	
	if(distSq < (GAME_WIDTH/4)*(GAME_WIDTH/4))
	{
		targetSensed = true;
	}
}

void Brick::ai(float time, Entity &t)
{ 
	targetEntity = t;
	//deltaTrack();
	//evade();
	sense();
	if(targetSensed)
	{
		vectorTrack();
	}
	return;
}