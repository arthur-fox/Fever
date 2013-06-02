//
//  EffectsManager.cpp
//  Fever
//
//  Created by Arthur Fox on 31/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "SDL.h"
#include "EffectsManager.h"

const int EFFECT_DURATION = 100; // SMALLER -> LONGER
//const int EFFECT_DAMPENING_FACTOR = 2;//2*LEVEL_SPEED_FACTOR;
const int FLASH_MAX = 255;
const int FLASH_MIN = 200;

EffectsManager::EffectsManager(float levelSpeed)
{
    m_levelSpeed = levelSpeed;
    
    m_dtAccum = 0;
    m_animFlash = false;
    m_flashColour = Colour();
}

EffectsManager::~EffectsManager()
{
}

bool EffectsManager::Update( float dt )
{
    if (m_animFlash)
    {
        m_flashColour -= (EFFECT_DURATION * dt/1000.f); // m_levelSpeed/EFFECT_DAMPENING_FACTOR * dt/1000.f
        
        if (m_flashColour <= FLASH_MIN)
        {
            m_animFlash = false;
        }
    }
    
    return true;
}

void EffectsManager::PlayScreenFlash()
{
    if (!m_animFlash)
    {
        m_animFlash = true;
        m_playedFlash = true;
        m_flashColour = Colour(255,255,255);//Colour(FLASH_MAX);
    }
}

void EffectsManager::Render( SDL_Surface* pScreen )
{
    if (m_animFlash)
    {   
        SDL_FillRect(pScreen, NULL, SDL_MapRGBA( pScreen->format, m_flashColour.GetR(), m_flashColour.GetG(), m_flashColour.GetB(), 0));//m_flashColour.GetA()));
    }
}