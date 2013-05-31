//
//  Colour.h
//  Fever
//
//  Created by Arthur Fox on 01/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Colour_h
#define Fever_Colour_h

const int COLOUR_RED = 0;
const int COLOUR_GREEN = 1;
const int COLOUR_BLUE = 2;
const int COLOUR_ALL = 3;
const int NUM_COLOUR_CHANNELS = 3;

class Colour
{
public:
    
    Colour();
    Colour( int rgb );
    Colour( int r, int g, int b );

    //NOTE: These operators have been defined Weirdly
    //      They are currently VERY Specific to this application!
    friend bool operator>= (const Colour& lhs, const int& val   );
    friend bool operator<= (const Colour& lhs, const int& val   );
    friend bool operator>= (const Colour& lhs, const Colour& rhs);
    friend bool operator<= (const Colour& lhs, const Colour& rhs);
    
    void Inc(int colour);
    void Dec(int colour);
    
    inline int GetR() const {return m_r;}
    inline int GetG() const {return m_g;}
    inline int GetB() const {return m_b;}
    int Get(int colour) const;
    
private:
    
    int m_r, m_g, m_b;
};

#endif
