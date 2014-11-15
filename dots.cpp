#include "dots.h"

//=============================================================================
// default constructor
//=============================================================================
Dot::Dot() : Entity()
{
	spriteData.width = dotNS::WIDTH;           
	spriteData.height = dotNS::HEIGHT;
	spriteData.x = dotNS::X;                   // location on screen
	spriteData.y = dotNS::Y;
	spriteData.rect.bottom = dotNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = dotNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
	startFrame = 0;              // first frame of ship animation
	endFrame = 0;              // last frame of ship animation
	currentFrame = startFrame;
	radius = dotNS::WIDTH/2.0;                 // collision radius
	//collisionType = entityNS::BOX;
	setCollisionType(entityNS::CIRCLE);
	setCollisionRadius(radius);
	edge.bottom = dotNS::HEIGHT/2;
	edge.top = -dotNS::HEIGHT/2;
	edge.right = dotNS::WIDTH/2;
	edge.left = -dotNS::WIDTH/2;
	spriteData.scale = 1;
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Dot::update(float frameTime)
{

}