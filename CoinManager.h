//
//  CoinManager.h
//  Fever
//
//  Created by Arthur Fox on 02/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_CoinManager_h
#define Fever_CoinManager_h

#include <vector>
#include "GameObject.h"
#include "Coin.h"
#include "Floor.h"

class CoinManager
{
public:
    
	CoinManager( int speed, int width, int height, Floor* pFloor );
	
	inline int GetSpeed() const {return m_speed;}
	inline int GetTouched() const {return m_numTouched;}
    void SetSpeed( int s );
    
	void Update( float delta );
	void Render( SDL_Surface* pScreen );
    
    //Adds coins to platforms vector, returns true if coin added, false otherwise
	bool AddCoin( int initialX = -1, int initialY = -1 );
    int TouchCoin( GameObject& rObject );
    Coin Remove( int num );
	void ClearInvisible();
    
private:	
	//Vector of coins
	std::vector<Coin> coins;
    
    //Iterator for coins
    std::vector<Coin>::iterator m_it;
	
    //Reference to the level's floor
    Floor* m_pFloor;
    
	//Coins' speed
	int m_speed, m_width, m_height, m_numTouched;
	
	//Creates new platform and adds to platform vector
	void NewCoin( int initialX = -1, int initialY = -1 );
};

#endif
