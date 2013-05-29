//
//  LevelDirector.h
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_LevelDirector_h
#define Fever_LevelDirector_h

#include "Global.h"
#include "Player.h"
#include "CoinManager.h"
#include "Floor.h"
#include "Scene.h"
#include "Timer.h"
#include "Colour.h"
#include "Camera.h"


//TODO: FINISH!
//A NEW ONE CREATED EVERYTIME A PERSON SELECTS A LEVEL

// NEED TO SETTLE ON GAME CLASS ARCHITECTURE
// CURRENTLY THINKING:
// This class looks after playing a level
// So it knows about:
// 1. The player
// 2. The floor
// 3. The background
// 4. The coins
// 5. The score
// Basically this class takes over the playing role that the GameDirector has...
// Once a level has been played it gives complete control back over to the GameDirector!
// --
// The level must make all the decisions based on a file that the GameDirector gives it!
// Hopefully decouple everything!

// Defines colour boundaries
const int UPPER_COLOUR = 220;
const int LOWER_COLOUR = 35;
const int COLOUR_BAND_WIDTH = 5;    // DECREASE makes variation in background colour LARGER
const int EXTRA_TIME = 3000;    //Add 3 seconds so that level ends properly.

const int MULTIPLIER_COINS_NEEDED = 10; // DOES THIS BELONG HERE?


// LevelDirector knows how to play the level it is initialised with
class LevelDirector
{
public:
    
	LevelDirector( const std::string &pFilepath, SDL_Surface* pScreen );
    ~LevelDirector();
    
    bool Run();
    
    static bool GenLevel( std::string lvlpath, std::string songpath );
    
private:	
    
    static Global* ms_pGlobal; //Needs to be static because of the Event Filters
    
    Scene* m_pScene;
    SDL_Event m_event;
	SDL_Surface *m_pScreen, *m_pPause;
    
	float m_fps;
    int m_score, m_mult;
    
    float m_levelSpeed, m_songDuration, m_coinFreq;
    std::string m_filepath;
    std::string m_songpath;
    
    static int PauseEventFilter( const SDL_Event *pEvent );
    static int EndEventFilter( const SDL_Event *pEvent );
    
    static bool GenLevelMatlab(std::string songpath);
    
    void OncePerSecond(float delay, CoinManager& rCoin);
    
    bool Update(Camera& rCamera, Player& rPlayer, Floor& rFloor, int dt, float& dtAccumulator, Colour &rCol, Colour* pCols, bool &up, CoinManager& rCoins);
    bool UpdateGame(Camera& rCamera, Player& rPlayer, Floor& rFloor, CoinManager& rCoins, int dt = 0);
    bool UpdateColours(Colour &col, Colour* pCols, bool &up, int dt = 0);
    void UpdateCol(Colour &col, bool &up);
    void UpdateScore(Player& rPlayer, CoinManager& rCoins);
    
    bool Render(Camera& rCamera, Player& rPlayer, Floor& rFloor, Colour* pCols, CoinManager& rCoins);
    
    bool EndSequence(Camera& rCamera, Player& rPlayer, Floor& rFloor, Colour* pCols);
    
    void PauseGame(bool& game, bool& playing);
};

#endif
