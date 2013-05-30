//
//  ColourManager.cpp
//  Fever
//
//  Created by Arthur Fox on 30/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "ColourManager.h"

ColourManager::ColourManager(float levelSpeed)
{
//    m_cols = Colour[SCREEN_WIDTH];
    m_col = Colour(120);
    m_up = true;
    m_levelSpeed = levelSpeed;
}

// For updating the background - DOES THIS BELONG HERE?
//  -- Still need to make this relate to the song
bool ColourManager::Update( float dt )
{
    static float dtCol = 0;
    
    if (!VARIABLE_TIME_STEP)
        dt = FIXED_TIME_STEP;
    
    Colour tempCol = m_col;
    bool tempUp = m_up;
    
    // Update cols array
    for ( int i = 0; i < SCREEN_WIDTH; i++ )
    {
        if (i % COLOUR_BAND_WIDTH == 0)
        {
            UpdateCol(tempCol, tempUp);
        }
        // CHECK: POSSIBLE MEMORY LEAK!
        m_cols[i] = tempCol;
    }
    
    // Set the col and up variables correctly for next loop
    dtCol += (m_levelSpeed * dt/1000.f);    //USING VARIABLE DT GIVES CRAZY RESULTS WHEN FRAME RATE DIPS!
    if (dtCol > 1)
    {
        int speed = (int) dtCol;
        int next_colour = speed*COLOUR_BAND_WIDTH;
        dtCol -= speed;
        
        if ( m_col.LessThan(m_cols[next_colour]) )
        {
            m_up = true;
        }
        else
        {
            m_up = false;
        }
        
        m_col = m_cols[next_colour];
    }
    
    return true;
}

//Update col variable according to UP bool
void ColourManager::UpdateCol( Colour &rCol, bool &rUp )
{
    // Increment or decrement colour depending on up
    rUp ? rCol.Inc(): rCol.Dec();
    
    // Update boolean up after updating col
    if (rCol.GreaterThan(UPPER_COLOUR) )
    {
        rUp = false;
    }
    else if (rCol.LessThan(LOWER_COLOUR) )
    {
        rUp = true;
    }
}

void ColourManager::Render( SDL_Surface* pScreen )
{
    Uint32 scanline[SCREEN_WIDTH];
    
    // Save pixel values for background scanline
    for (int i = 0; i <  SCREEN_WIDTH; i++)
    {
        scanline[i] = SDL_MapRGB( pScreen->format, m_cols[i].GetR(), m_cols[i].GetG(), m_cols[i].GetB() );
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
