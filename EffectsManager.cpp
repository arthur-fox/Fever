//
//  EffectsManager.cpp
//  Fever
//
//  Created by Arthur Fox on 31/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_gfxBlitFunc.h"
#include "EffectsManager.h"

//FIX: This should be a variable!
const int EFFECTS_ALPHA = 150;
const int EFFECT_DAMPENING_FACTOR = 2;//2*LEVEL_SPEED_FACTOR;
const int FLASH_MAX = 255;
const int FLASH_MIN = 150;

EffectsManager::EffectsManager(float levelSpeed)
{
    m_levelSpeed = levelSpeed;
    
    m_dtAccum = 0;
    m_animFlash = false;
    m_flashColour = Colour();
    
    
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//    m_pEffectsScreen = SDL_CreateRGBSurface(SDL_HWSURFACE,SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
//#else
//    m_pEffectsScreen = SDL_CreateRGBSurface(SDL_HWSURFACE,SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
//#endif
//    SDL_SetAlpha( m_pEffectsScreen, 0, SDL_ALPHA_OPAQUE );
//    m_pEffectsScreen = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
//	if ( m_pEffectsScreen == NULL ) {
//		printf( "Effects Manager could init screen: %s\n", SDL_GetError() );
//		SDL_Quit();
//		exit(1);
//	}
}

EffectsManager::~EffectsManager()
{
    SDL_FreeSurface(m_pEffectsScreen);
}

bool EffectsManager::Update( float dt )
{
    if (m_animFlash)
    {
        m_flashColour -= (m_levelSpeed/EFFECT_DAMPENING_FACTOR * dt/1000.f);
        
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

//        SDL_gfxBlitRGBA(m_pEffectsScreen, NULL, pScreen, NULL);
        
//        boxRGBA(pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_flashColour.GetR(), m_flashColour.GetG(), m_flashColour.GetB(), m_flashColour.GetA());
        
//        Global::SharedGlobal()->ApplySurface( 0, 0, m_pEffectsScreen, pScreen );
        
        
//        Uint32 pixels[SCREEN_WIDTH*SCREEN_HEIGHT];
//        
//        // Save pixel values for background
//        for (int i = 0; i <  SCREEN_WIDTH*SCREEN_HEIGHT; i++)
//        {
//            pixels[i] = SDL_MapRGBA( pScreen->format, m_pColours[i].GetR(), m_pColours[i].GetG(), m_pColours[i].GetB(), m_pColours[i].GetA());
//        }
//        
//        // Manually copy background pixel data over!
//        int pixelSize = sizeof(Uint32);
//        SDL_LockSurface(pScreen);
//        Uint32* pPixelData = static_cast<Uint32*> (pScreen->pixels);
//        memcpy(pPixelData, pixels, SCREEN_WIDTH*SCREEN_HEIGHT*pixelSize);
//        SDL_UnlockSurface(pScreen);
    }
}