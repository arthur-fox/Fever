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
#include "NoteManager.h"
#include "SceneManager.h"
#include "ColourManager.h"
#include "EffectsManager.h"
#include "Floor.h"
#include "Timer.h"
#include "Colour.h"
#include "Camera.h"
#include <map>


//A NEW ONE CREATED EVERYTIME A PERSON SELECTS A LEVEL

// NEED TO SETTLE ON GAME CLASS ARCHITECTURE
// CURRENTLY THINKING:
// This class looks after playing a level
// So it knows about:
// 1. The player
// 2. The floor
// 3. The background
// 4. The notes
// 5. The score
// Basically this class takes over the playing role that the GameDirector has...
// Once a level has been played it gives complete control back over to the GameDirector!
// --
// The level must make all the decisions based on a file that the GameDirector gives it!
// Hopefully decouple everything!

const int EXTRA_TIME = 3000;  //This should really be related to levelSpeed
const int MULTIPLIER_COINS_NEEDED = 10; // DOES THIS BELONG HERE?

// LevelDirector knows how to play the level it is initialised with
class LevelDirector
{
public:
    
	LevelDirector( const std::string &pFilepath, SDL_Surface* pScreen );
    ~LevelDirector();
    
    bool Run();
    
    static bool GenLevel( std::string lvlpath, std::string songpath, int option);
    
private:	
    
    static Global* ms_pGlobal; //Needs to be static because of the Event Filters
    
    SceneManager* m_pSceneManager;
    SDL_Event m_event;
	SDL_Surface *m_pScreen, *m_pPause;
    
	float m_fps;
    int m_score, m_mult;
    
    float m_signature, m_levelSpeed, m_songDuration, m_noteFreq;
    std::string m_filepath, m_songpath, m_levelname;
    
    static int PauseEventFilter( const SDL_Event *pEvent );
    static int EndEventFilter( const SDL_Event *pEvent );
    
    static bool GenLevelMatlab(std::string songpath, int option);
    
    void OncePerSecond(float delay, NoteManager& rNotes);
    
    bool Update(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects, int dt, float& dtAccumulator);
    bool UpdateGame(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects, float dt);
    void UpdateScore(Player& rPlayer, NoteManager& rNotes, EffectsManager& rEffects);
 
    bool Render(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects);
    bool EndSequence(Camera& rCamera, Player& rPlayer, Floor& rFloor, ColourManager& rColours);
    void UpdateHighScores();
    std::map<std::string,int> ScoresToMap();
    
    void PauseGame(bool& game, bool& playing);
};

#endif
