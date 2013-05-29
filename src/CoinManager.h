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
    
	CoinManager( float speed, float freq, int width, int height, Floor* pFloor );
	
	inline int GetSpeed() const {return m_speed;}
	inline int GetTouched() const {return m_numTouched;}
    void SetSpeed( int s );
    
	void Update( float delta );
	void Render( SDL_Surface* pScreen, Camera& rCamera);
    
    //Adds coins to platforms vector, returns true if coin added, false otherwise
	bool AddCoin( int initialX = -1, int initialY = -1 );
    int TouchCoin( GameObject& rObject );
    Coin Remove( int num );
	void ClearInvisible();
    
    inline bool IsCreatingCoins() const {return m_creatingCoins;}
    inline void SetCreatingCoins(bool flag) {m_creatingCoins = flag;}
    
private:	
	//Vector of coins
	std::vector<Coin> coins;
    
    //Iterator for coins
    std::vector<Coin>::iterator m_it;
	
    //Reference to the level's floor
    Floor* m_pFloor;
    
	//Coins' attributes
	int m_width, m_height, m_numTouched;
    
    //Speed and Frequency of coins - corresponds to bpm
    float m_speed, m_coinFreq;
    
    //Flag saying whether coins are being created or not
    bool m_creatingCoins;
	
	//Creates new platform and adds to platform vector
	void NewCoin( int initialX = -1, int initialY = -1 );
};

#endif
