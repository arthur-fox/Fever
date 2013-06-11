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
#include "Floor.h"

//ColourManager class takes care of updating the background colour values in a level
class ColourManager
{
public:
    
    ColourManager(float levelSpeed, Floor* pFloor);
    ~ColourManager();
    
    bool Update( float dt );
    void Render( SDL_Surface* pScreen );
    
private:
    
    float m_levelSpeed;
    Floor* m_pFloor;
    
    Colour* m_pColours; // MAKE INTO VECTOR??
    Colour m_currColour;
    bool m_primaryUp, m_secondaryUp; //Primary up impacts All channels, Secondary up impacts m_currChannel
    float m_dtColour;
    int m_numUpChanged, m_necessaryUpChanges, m_currChannel;
    bool m_availableChannels[NUM_COLOUR_CHANNELS];
    
    void UpdateColour( Colour &rColour, bool &rUp, int col, int qty );
    void UpdateChannel(int &currChannel);
};

#endif /* defined(__Fever__ColourManager__) */
