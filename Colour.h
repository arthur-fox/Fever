//
//  Colour.h
//  Fever
//
//  Created by Arthur Fox on 01/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Colour_h
#define Fever_Colour_h

class Colour
{
public:
    
    Colour();
    Colour( int rgb );
    Colour( int r, int g, int b );
    
    //NOTE: Maybe these should be overloade operators?
    //      all comparisons are actually >= and <= not >, <
    void Inc();
    void Dec();
    bool GreaterThan( int val ) const;
    bool LessThan( int val ) const;
    bool GreaterThan( Colour &c ) const;
    bool LessThan( Colour &c ) const;
    
    inline int GetR() const {return m_r;}
    inline int GetG() const {return m_g;}
    inline int GetB() const {return m_b;}
    
private:
    
    int m_r, m_g, m_b; //alpha too?
};

#endif
