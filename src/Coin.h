//
//  Coin.h
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Coin_h
#define Fever_Coin_h

#include "GameObject.h"
#include "Floor.h"

//TODO: Finish

// Coin is the superclass that any coins in the game must inherit from
// BUT currently its the only coin, so its concrete not abstract

class Coin : public GameObject
{	
public:
    
	Coin();
    Coin( int vel, int initialX, int initialY, Floor* pFloor);
    ~Coin();
    
    void InitCoinImages();
    
	void HandleInput( float deltaTicks );
	bool virtual Update( float deltaTicks ); //Return false if its gameover
	bool IsVisible();
    inline void SetSpeed( float vel ) {m_xVel = vel;}
    
	const CollisionBox virtual MyCollisionBox( float deltaTicks = 0 );
    bool TouchCoin( GameObject& rObject );
    
private:
	
    static SDL_Surface *ms_pCoinImg;
    
    Floor* m_pFloor;
};

#endif
