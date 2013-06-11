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
const int NUM_COLOUR_CHANNELS = 3; //Excludes alpha

const int COLOUR_ALPHA = 10;
const int COLOUR_ALL = 11;

class Colour
{
public:
    
    Colour();
    Colour( int rgb );
    Colour( int r, int g, int b );
    Colour( int r, int g, int b, int a );
    
    inline int GetR() const {return m_r;}
    inline int GetG() const {return m_g;}
    inline int GetB() const {return m_b;}
    inline int GetA() const {return m_a;}
    int Get(int colour) const;

    //NOTE: These operators have been defined Weirdly
    //      They are currently VERY Specific to this application!
    //      Also completely ignore ALPHA channel!
    friend bool operator>= (const Colour& lhs, const int& val   );
    friend bool operator<= (const Colour& lhs, const int& val   );
    friend bool operator== (const Colour& lhs, const int& val   );
    friend bool operator>= (const Colour& lhs, const Colour& rhs);
    friend bool operator<= (const Colour& lhs, const Colour& rhs);
    friend bool operator== (const Colour& lhs, const Colour& rhs);
    
    Colour& operator+=(const int& rhs);
    Colour& operator-=(const int& rhs);
    
    void Inc(int colour, int qty = 1);
    void Dec(int colour, int qty = 1);
    
private:
    
    int m_r, m_g, m_b, m_a;
};

#endif
