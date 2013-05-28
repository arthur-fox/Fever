//
//  Colour.cpp
//  Fever
//
//  Created by Arthur Fox on 01/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "Colour.h"

Colour::Colour()
{
    m_r = m_g = m_b = 0;
}

Colour::Colour( int rgb )
{
    m_r = m_g = m_b = rgb;
}

Colour::Colour( int r, int g, int b )
{
    m_r = r;
    m_g = g;
    m_b = b;
}

//NOTE: Might make sense to give functions other than 
//      inc() and dec() for changing values by more
void Colour::Inc()
{
    m_r++;
    m_g++;
    m_b++;
}

void Colour::Dec()
{
    m_r--;
    m_g--;
    m_b--;
}

bool Colour::GreaterThan(int val) const
{
    return m_r >= val && m_g >= val && m_b >= val;
    //return (r+g+b)/3 > val;
}

bool Colour::LessThan(int val) const
{
    return m_r <= val && m_g <= val && m_b <= val;
    //return (r+g+b)/3 < val;
}

bool Colour::GreaterThan(Colour &c) const
{
    return m_r >= c.GetR() && m_g >= GetG() && m_b >= GetB();
    //return (r+g+b)/3 > (c.getR()+c.getG()+c.getB())/3;
}

bool Colour::LessThan(Colour &c) const
{
    return m_r <= c.GetR() && m_g <= GetG() && m_b <= GetB();
    //return (r+g+b)/3 < (c.getR()+c.getG()+c.getB())/3;
}