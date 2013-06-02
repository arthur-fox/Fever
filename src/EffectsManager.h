//
//  EffectsManager.h
//  Fever
//
//  Created by Arthur Fox on 31/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef __Fever__EffectsManager__
#define __Fever__EffectsManager__

#include "Global.h"
#include "Colour.h"

//EffectsManager class takes care of playing effects
//NOTE: Currently only one effect exists - namely screenflash
class EffectsManager
{
public:
    
    EffectsManager(float levelSpeed);
    ~EffectsManager();
    
    bool Update( float dt );
    void PlayScreenFlash();
    inline bool IsPlayingFlash() {return m_animFlash;}
    inline bool HasPlayedFlash() {return m_playedFlash;}
    inline void ResetPlayedFlash() {m_playedFlash = false;}
    
    void Render( SDL_Surface* pScreen );
    
private:
    
    float m_levelSpeed;
    
    float m_dtAccum;
    bool m_animFlash;
    bool m_playedFlash;
    Colour m_flashColour;
};

#endif /* defined(__Fever__EffectsManager__) */
