//
//  ColourManager.h
//  Fever
//
//  Created by Arthur Fox on 30/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef __Fever__ColourManager__
#define __Fever__ColourManager__

#include <iostream>
#include "Global.h"
#include "Colour.h"

// Defines colour boundaries
const int UPPER_COLOUR = 220;
const int LOWER_COLOUR = 35;
const int COLOUR_BAND_WIDTH = 5;    // DECREASE makes variation in background colour LARGER

//ColourManager class takes care of updating the background colour values in a level
class ColourManager
{
public:
    
    ColourManager(float levelSpeed);
    
    bool Update( float dt );
    void UpdateCol( Colour &rCol, bool &rUp );
    void Render( SDL_Surface* pScreen );
    
private:
    
    Global* m_pGlobal;
    
    Colour m_cols[SCREEN_WIDTH]; // MAKE INTO VECTOR??
    Colour m_col;
    bool m_up;
    float m_levelSpeed;
};

#endif /* defined(__Fever__ColourManager__) */
