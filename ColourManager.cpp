//
//  ColourManager.cpp
//  Fever
//
//  Created by Arthur Fox on 30/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "ColourManager.h"

const int INIT_COLOUR = 120;
const int SPEED_DAMPENING = 2;
const int UP_CHANGE_FACTOR = 5000;
const int BASE_TEMPO = 50;

// TODO: CHANGE THESE BOUNDARIES BASED ON SOME CRITERIA
const int UPPER_COLOUR = 220;
const int LOWER_COLOUR = 35;
const int COLOUR_BAND_WIDTH = 5;    // DECREASE makes variation in background colour LARGER

ColourManager::ColourManager(float levelSpeed)
{
    m_levelSpeed = levelSpeed/ SPEED_DAMPENING;
    m_necessaryUpChanges = UP_CHANGE_FACTOR / levelSpeed;
    
    m_colours = new Colour[SCREEN_WIDTH];
    m_currColour = Colour(INIT_COLOUR);
    m_primaryUp = m_secondaryUp = true;
    m_dtColour = 0;
    m_numUpChanged = 0;
    
    
    //TODO: MAKE CHANNELS UNAVAILABLE BASED ON SOME CRITERIA
    //      OTHER THAN LEVEL_SPEED
    m_currChannel = COLOUR_RED;
    for (int i = 0; i < NUM_COLOUR_CHANNELS; i++)
    {
        if ( levelSpeed/LEVEL_SPEED_FACTOR > BASE_TEMPO*(i+1) )
        {
            m_availableChannels[i] = true;
        }
        else
        {
            m_availableChannels[i] = false;
        }
    }
}

ColourManager::~ColourManager()
{
    delete m_colours;
}

//NOTE: This algorithm is very hacky/messy
//      I basically try to fluctuate ALL Channels using primaryUp
//      and fluctuate one seperate channel using secondaryUp
//FIX: STILL need to make this relate to the song!!!
bool ColourManager::Update( float dt )
{
    Colour tempCol = m_currColour;
    bool tempUp = m_primaryUp;
    
    for ( int i = 0; i < SCREEN_WIDTH; i++ )
    {
        if (i % COLOUR_BAND_WIDTH == 0)
            UpdateColour(tempCol, tempUp, COLOUR_ALL);
        
        m_colours[i] = tempCol;
    }
    
    // Set colour, channel and both up variables correctly for next loop
    m_dtColour += (m_levelSpeed * dt/1000.f);
    if (m_dtColour > 2) //Gurantees we don't screw the UP variables
    {
        int next_colour = m_dtColour*COLOUR_BAND_WIDTH;
        bool newPrimaryUp = m_primaryUp;
        
        if ( m_currColour <= m_colours[next_colour] )
        {
            newPrimaryUp = true;
        }
        else
        {
            newPrimaryUp = false;
        }

        if (newPrimaryUp != m_primaryUp)
        {
            m_numUpChanged++;
            UpdateChannel(m_currChannel);
        }
        
        m_dtColour = 0;
        m_primaryUp = newPrimaryUp;
        m_currColour = m_colours[next_colour];
        
        
        UpdateColour(m_currColour, m_secondaryUp, m_currChannel);
    }
    
    return true;
}

//Update colour according to UP flag
void ColourManager::UpdateColour( Colour &rColour, bool &rUp, int col )
{
    rUp ? rColour.Inc(col): rColour.Dec(col);
    
    if ( col == COLOUR_ALL)
    {
        if (rColour >= UPPER_COLOUR)
        {
            rUp = false;
        }
        else if (rColour <= LOWER_COLOUR )
        {
            rUp = true;
        }
    }
    else
    {
        if (rColour.Get(col) >= UPPER_COLOUR)
        {
            rUp = false;
        }
        else if (rColour.Get(col) <= LOWER_COLOUR)
        {
            rUp = true;
        }
    }
}


void ColourManager::UpdateChannel(int &currChannel)
{
    if (m_numUpChanged % m_necessaryUpChanges == 0)
    {
        for(int i = 0; i < NUM_COLOUR_CHANNELS; i++)
        {
            currChannel = (currChannel + 1) % NUM_COLOUR_CHANNELS;
            if( m_availableChannels[currChannel])
                break;
        }   
    }
}


void ColourManager::Render( SDL_Surface* pScreen )
{
    Uint32 scanline[SCREEN_WIDTH];
    
    // Save pixel values for background scanline
    for (int i = 0; i <  SCREEN_WIDTH; i++)
    {
        scanline[i] = SDL_MapRGB( pScreen->format, m_colours[i].GetR(), m_colours[i].GetG(), m_colours[i].GetB() );
    }
    
    // Manually copy background pixel data over!
    SDL_LockSurface(pScreen);
    Uint32* pPixelData = static_cast<Uint32*> (pScreen->pixels);
    for (int j = 0; j < SCREEN_HEIGHT; j++)
    {
        memcpy(pPixelData+j*SCREEN_WIDTH, scanline, SCREEN_WIDTH*4);
    }
    SDL_UnlockSurface(pScreen);
}

// Colour the background
//for ( int i = 0; i < SCREEN_WIDTH; i++ )
//{
//
//    Uint32 color = SDL_MapRGB( pScreen->format, cols[i].GetR(), cols[i].GetG(), cols[i].GetB() );
//
//    SDL_Rect rect;
//    rect.x = i;
//    rect.w = 1;
//    rect.y = 0;
//    rect.h = SCREEN_HEIGHT;
//
//    //FIX: THIS IS VERY SLOW
//    SDL_FillRect( m_pTemp, &rect , color );
//}
//m_pGlobal->ApplySurface(0, 0, m_pTemp, pScreen);
