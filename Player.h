//
//  Player.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Player_h
#define Fever_Player_h

#include "GameObject.h"
#include "Floor.h"

// Player represents the avatar (Blob) in the game

class Player : public GameObject
{
public:
    
	Player( Floor* pFloor );
    ~Player();
    
	void HandleInput( float deltaTicks );
	bool virtual Update( float deltaTicks ); //Return false if its gameover
    
    bool End( SDL_Surface* pScreen );
	
	const CollisionBox virtual MyCollisionBox( float deltaTicks = 0 );
    
private:
    
	//jump, walk and gravity - all in terms of pixels
	int m_jump, m_gravity, m_walk;
    
    Floor* m_pFloor;
};

#endif