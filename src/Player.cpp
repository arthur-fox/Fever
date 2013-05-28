//
//  Player.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//


#include "Player.h"
#include "Global.h"
#include "GameDirector.h"
#include <iostream>

// NOT SURE HOW TO SET THIS VALUE CORRECTLY
const float MAX_YVEL = 1000;

Player::Player( Floor* pFloor )
{	
	//Initialise the player offsets
	m_x = WALL_WIDTH;
	m_y = 0; 
	
	m_xVel = 0;
	m_yVel = 0;
    
	m_pImage = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
	
    m_pFloor = pFloor;
	m_jump = INIT_JUMP;      
    m_gravity = INIT_GRAVITY; 
	//m_walk = INIT_WALK;
}

Player::~Player()
{
    SDL_FreeSurface( m_pImage );
}

//Handle the input accordingly so that the update can perform correctly
void Player::HandleInput( float deltaTicks )
{	
	Uint8* pKeystates = SDL_GetKeyState( NULL );
    
    bool onFloor = m_pFloor->OnFloor( *this );
	
    //Jump
	if ( onFloor && pKeystates[SDLK_UP] && m_yVel >= 0 )
    {
		m_yVel = -m_jump;
    }
}

//Updates the player's state taking time elapsed into account
bool Player::Update(float deltaTicks)
{
	m_x += m_xVel * (deltaTicks / 1000.f);
	
	m_y += m_yVel * (deltaTicks / 1000.f);
	m_yVel += m_gravity * (deltaTicks / 1000.f);
    m_yVel = std::min(MAX_YVEL, m_yVel);

	if ( m_yVel >= 0 )
    {
        //If player is going through the floor this returns the height of the floor
		float floorHeight = m_pFloor->ThroughFloor( *this, deltaTicks );

		if ( floorHeight >= 0 )
			m_y = floorHeight - PLAYER_HEIGHT;                      
	}
	
	return true;	
}


bool Player::End(SDL_Surface* pScreen)
{
    m_xVel = INIT_WALK;
    
    Update( FIXED_TIME_STEP );
    Render( pScreen );
    
    return ( m_x < SCREEN_WIDTH );
}

//Method which creates a collision box for the player, if deltaTicks -1 Time is not taken into account
const GameObject::CollisionBox Player::MyCollisionBox(float deltaTicks)
{
	CollisionBox col = CollisionBox();
	col.x1 = m_x;				
	col.x2 = m_x + PLAYER_WIDTH;
    col.y1 = m_y;
	col.y2 = m_y + PLAYER_HEIGHT;
	
    //CHECK: IS THE PART BELOW CORRECT
	//If we are dealing with Time then the bottom has moved, otherwise set it to what it currently is
	if ( deltaTicks > 0 )
    {
		col.y1 = (m_y + PLAYER_HEIGHT) - m_yVel * ( deltaTicks / 1000.f );          // TOP PART = BOTTOM PART BEFORE MOVING 
    }

	return col;
}