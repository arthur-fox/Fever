//
//  Coin.cpp
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include <iostream>
#include "Coin.h"
#include "Global.h"

SDL_Surface *Coin::ms_pCoinImg(0);

#pragma mark -
#pragma mark CoinFunctions

//Initialise platform variables
Coin::Coin( int vel, int initialX, int initialY, Floor* pFloor)
{	
    if ( ms_pCoinImg == 0 )
        InitCoinImages();
    
    m_x = initialX - COIN_WIDTH;
    m_y = initialY - COIN_HEIGHT;
    
    m_pFloor = pFloor;
    
	m_yVel = 0;
	m_xVel = vel;
    
    m_pImage = ms_pCoinImg;
}

Coin::~Coin()
{
    //SDL_FreeSurface(m_pImage);
}

//Initialise all the static surfaces for the different types of platforms
void Coin::InitCoinImages()
{
    ms_pCoinImg  = Global::SharedGlobal()->LoadImage( RESOURCE_COIN );
    
    if ( ms_pCoinImg == NULL )
    {
		printf( "Could not load coin images correctly: %s\n", SDL_GetError() );
		SDL_Quit();
		exit(1);
	}
}

//Updates the platforms position
bool Coin::Update( float deltaTicks )
{
	m_x -= m_xVel * ( deltaTicks / 1000.f );
    
    float heightDiff = m_pFloor->GetNextHeight() - m_pFloor->GetHeight();
    
    m_y += heightDiff/3; // * ( deltaTicks / 1000.f );
    
	return true;
}

//Returns true if Coin is within screen dimensions
bool Coin::IsVisible()
{
	return ( m_x >=0 && m_x <=SCREEN_WIDTH); // && m_y >= 0 && m_y <= SCREEN_HEIGHT );
}

#pragma mark -
#pragma mark CollisionDetection

//Method which creates a collision box for the coin
const GameObject::CollisionBox Coin::MyCollisionBox( float deltaTicks )
{
	CollisionBox col = CollisionBox();
	col.x1 = m_x;                       
	col.x2 = m_x + this->GetWidth();	
	col.y1 = m_y;						
	col.y2 = m_y + this->GetHeight();   
	
	return col;
}

//Returns true if the object is on this platform - very abstract, as it compares collision boxes which are abstract concepts
bool Coin::TouchCoin( GameObject& rObject )
{
	CollisionBox objCol = rObject.MyCollisionBox();
	CollisionBox coinCol = this->MyCollisionBox();
	
	//Check if object box is inside the coin box
    //THIS SEEMS LIKE TOO MANY CHECKS, CAN I SIMPLIFY?
	return ( ((coinCol.y2 >= objCol.y1 && coinCol.y1 <= objCol.y1) ||
              (coinCol.y2 <= objCol.y2 && coinCol.y1 >= objCol.y1)||
              (coinCol.y1 <= objCol.y2 && coinCol.y2 >= objCol.y2)) &&
             ((coinCol.x2 >= objCol.x1 && coinCol.x1 <= objCol.x1) ||
              (coinCol.x2 <= objCol.x2 && coinCol.x1 >= objCol.x1)||
              (coinCol.x1 <= objCol.x2 && coinCol.x2 >= objCol.x2)) );
}
