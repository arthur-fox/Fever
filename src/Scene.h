//
//  Scene.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Background_h
#define Fever_Background_h

#include "Global.h"
#include "Colour.h"

// Scene class knows how to render different scenes of the game
// - its the directors' responsibility to tell this class when to render

class Scene
{
public:
    
    Scene();
    
    void UpdateInLevel( float deltaTicks, float fps, int sc, int mult );
    void ResetScene();
    
    void RenderInMainMenu( SDL_Surface* pScreen, int option );
    void RenderInLevel( SDL_Surface* pScreen, Colour* cols ); 
    void RenderLevelOver( SDL_Surface* pScreen, Colour* cols );
    
private:
    
    Global* m_pGlobal;
    
    //The offsets of the background 
    float m_bgX, m_bgY;
	SDL_Surface *m_pFrames, *m_pScore, *m_pMult, *m_pPlayer, *m_pMutedIcon, *m_pFeverText, 
                *m_pLoadText, *m_pPlayText, *m_pGenText, *m_pGameOverText, *m_pTryAgainText;
    
    void UpdateLevel(float deltaTicks);
    void UpdateFrameRate(float fps);
    void UpdateMultiplier(int mult);
    void UpdateScore(int sc);
    
    void RenderBackground(SDL_Surface* pScreen, Colour *cols);
};

#endif
