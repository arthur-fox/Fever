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
const float MAX_YVEL = 1500;

//NOTE: LevelSpeed was introduced to alter gravity and jump but currently unused
Player::Player( float levelSpeed, Floor* pFloor)
{
    m_levelSpeed = levelSpeed;
    m_pFloor = pFloor;
    
	m_x = WALL_WIDTH;
	m_y = 0; 
	m_xVel = 0;
	m_yVel = 0;
	
	m_jump = INIT_JUMP; 
    m_gravity = INIT_GRAVITY;
	//m_walk = INIT_WALK;
    
    m_pImage = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
}

Player::~Player()
{
    SDL_FreeSurface( m_pImage );
}

//Handle the input accordingly so that the update can perform correctly
void Player::HandleInput( float dt )
{	
	Uint8* pKeystates = SDL_GetKeyState( NULL );
    
    bool onFloor = m_pFloor->OnFloor( *this );
	
    //Jump
	if ( onFloor && pKeystates[SDLK_UP])
    {
		m_yVel = -m_jump;
    }
}

//Updates the player's state taking time elapsed into account
bool Player::Update(float dt)
{
	m_x += m_xVel * (dt / 1000.f);
	
	m_y += m_yVel * (dt / 1000.f);
	m_yVel += m_gravity * (dt / 1000.f);
    m_yVel = std::min(MAX_YVEL, m_yVel);
    
    //If player is going through the floor this returns the height of the floor
    float floorHeight = m_pFloor->ThroughFloor( *this, dt );
    if ( floorHeight >= 0 )
        m_y = floorHeight - PLAYER_HEIGHT;
	
	return true;	
}


bool Player::End(SDL_Surface* pScreen, Camera& rCamera)
{
    m_xVel = INIT_WALK;
    
    Update( FIXED_TIME_STEP );
    Render( pScreen, rCamera );
    
    return ( m_x < SCREEN_WIDTH );
}

//Method which creates a collision box for the player, if dt -1 Time is not taken into account
const GameObject::CollisionBox Player::MyCollisionBox(float dt)
{
	CollisionBox col = CollisionBox();
	col.x1 = m_x;				
	col.x2 = m_x + PLAYER_WIDTH;
    col.y1 = m_y;
	col.y2 = m_y + PLAYER_HEIGHT;
	
    //CHECK: IS THE PART BELOW CORRECT
	//If we are dealing with Time then the bottom has moved, otherwise set it to what it currently is
	if ( dt > 0 )
    {
		col.y1 = (m_y + PLAYER_HEIGHT) - m_yVel * ( dt / 1000.f );          // TOP PART = BOTTOM PART BEFORE MOVING 
    }

	return col;
}