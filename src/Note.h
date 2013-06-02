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

// Coin is the superclass that any notes in the game must inherit from
// BUT currently its the only note, so its concrete not abstract

class Note : public GameObject
{	
public:
    
	Note();
    Note( int vel, int initialX, int initialY, Floor* pFloor);
    ~Note();
    
    void InitNoteImages();
    
	void HandleInput( float dt );
	bool virtual Update( float dt ); //Return false if its gameover
	bool IsVisible();
    inline void SetSpeed( float vel ) {m_xVel = vel;}
    
	const CollisionBox virtual MyCollisionBox( float dt = 0 );
    bool TouchCoin( GameObject& rObject );
    
private:
	
    static SDL_Surface *ms_pNoteImg1, *ms_pNoteImg2, *ms_pNoteImg3, *ms_pClefImg;
    
    Floor* m_pFloor;
};

#endif
