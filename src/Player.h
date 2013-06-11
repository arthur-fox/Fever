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

// Player represents the avatar in the game

class Player : public GameObject
{
public:
    
	Player( float levelSpeed, Floor* pFloor );
    ~Player();
    
	void HandleInput( float dt );
	bool virtual Update( float dt );
    
    bool End( SDL_Surface* pScreen, Camera& rCamera);
	
	const CollisionBox virtual MyCollisionBox( float dt = 0 );
    
private:
    
	//jump, walk and gravity - all in terms of pixels
	int m_jump, m_gravity, m_walk;
    
    float m_levelSpeed; //COMPLETELY UNUSED
    Floor* m_pFloor;
};

#endif