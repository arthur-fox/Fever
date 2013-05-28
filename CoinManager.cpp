//
//  CoinManager.cpp
//  Fever
//
//  Created by Arthur Fox on 02/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "CoinManager.h"
#include "Global.h"

const int DISTANCE_BETWEEN_COINS = 250; 
// THIS IS TEMPORARY BEFORE COINS ARE SET BY THE FILE


CoinManager::CoinManager(int speed, int width, int height, Floor* pFloor)
{
	m_speed = speed;
    m_width = width;
    m_height = height;
    m_pFloor = pFloor;
    
    m_numTouched = 0;
    
    NewCoin(m_width, m_height);
}


void CoinManager::NewCoin(int initialX, int initialY)
{
	Coin coin(m_speed, initialX, initialY);
	coins.push_back(coin);
}


void CoinManager::SetSpeed(int s)
{
	m_speed = s;
	
	for (m_it=coins.begin(); m_it < coins.end(); m_it++)
		m_it->SetSpeed(m_speed);
}


void CoinManager::Update(float deltaTicks)
{	
	for (m_it=coins.begin(); m_it < coins.end(); m_it++)
		m_it->Update(deltaTicks);
    
    //TODO: THIS NEEDS TO DONE IN THE FILE GENERATION PART!
    //      JUMP value will be higher for more difficult songs
    //      and DISTANCE_BETWEEN_COINS will vary too
    // --> as long as seed is same this will always provide consistent results
    int JUMP = 200;
    int coinFloorHeight = m_pFloor->GetLastHeight();
    
    int diffInJumpAndFLoor = coinFloorHeight - JUMP;
    int range = (coinFloorHeight - abs(diffInJumpAndFLoor));                
    
    if (range == 0) range = 1;
    int randHeight = (rand() % range) + abs(diffInJumpAndFLoor);
    
    //while (randHeight < COIN_HEIGHT) randHeight += COIN_HEIGHT/2;
    //while (randHeight + COIN_HEIGHT >= coinFloorHeight) randHeight -= COIN_HEIGHT/2;
    if (randHeight < COIN_HEIGHT) randHeight = COIN_HEIGHT; //Last ditch attempt!
       
    AddCoin(m_width, randHeight);
}


void CoinManager::Render(SDL_Surface* pScreen)
{	
	for (m_it=coins.begin(); m_it < coins.end(); m_it++)
		m_it->Render(pScreen);
}

//Only public access method which adds a new coin to the coins vector 
bool CoinManager::AddCoin(int initialX, int initialY)
{
	if (coins.empty() || (coins.back().GetX()<= SCREEN_WIDTH - DISTANCE_BETWEEN_COINS) )
    {
		NewCoin(initialX, initialY);
		return true;
	}
	return false;
}

//Returns true if the object has touched a coin
int CoinManager::TouchCoin(GameObject& rObject)
{
    int ret = 0;
    m_it=coins.begin();
    std::vector<Coin>::iterator end = coins.end();
    
    while (m_it != end && !m_it->IsVisible()) // Only need to check the first visible coin
    {
        ret++;
        m_it++;
    }
    
    if (m_it != end && m_it->TouchCoin(rObject))
    {
        m_numTouched++;
        return ret;
    }
	
	return -1;
}

Coin CoinManager::Remove(int num)
{
    if (!coins.empty())
    {
        m_it=coins.begin();
        for (int i = 0; i < num; i++)
        {
            m_it++;
        }
        
        Coin coin = coins[num];
        coins.erase(m_it);
        return coin;
    }
    else
    {
        printf("Tried popping an element from an empty coins list");
        exit(1);
    }
}

//Removes coins which are not visible
// clearing coins means some have been missed, hence numTouched is reset
void CoinManager::ClearInvisible()
{	
	m_it=coins.begin();
    
	while (m_it < coins.end())
    {
		if (!m_it->IsVisible())
        {
			m_it = coins.erase(m_it);
            m_numTouched = 0;
        }
        
		else
            m_it++;
	}
}