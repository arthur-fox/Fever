//
//  Colour.cpp
//  Fever
//
//  Created by Arthur Fox on 01/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "Colour.h"
#include <algorithm>

const int MAX_RGB = 255;
const int MIN_RGB = 0;

Colour::Colour()
{
    m_r = m_g = m_b = m_a = MIN_RGB;
}

Colour::Colour( int rgb )
{
    m_r = m_g = m_b = rgb;
    m_a = MAX_RGB;
}

Colour::Colour( int r, int g, int b )
{
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = MAX_RGB;
}

Colour::Colour( int r, int g, int b, int a )
{
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
}

int Colour::Get(int colour) const
{
    switch ( colour )
    {
        case COLOUR_RED:
            return m_r;
            
        case COLOUR_GREEN:
            return m_g;
            
        case COLOUR_BLUE:
            return m_b;
            
        case COLOUR_ALPHA:
            return m_a;
            
        default:
            return -1;
    }
}

bool operator>= (const Colour& lhs, const int& val)
{
//    return lhs.m_r >= val || lhs.m_g >= val || lhs.m_b >= val;
    return (lhs.m_r+lhs.m_g+lhs.m_b)/3 >= val;
}

bool operator<= (const Colour& lhs, const int& val)
{
//    return lhs.m_r <= val || lhs.m_g <= val || lhs.m_b <= val;
    return (lhs.m_r+lhs.m_g+lhs.m_b)/3 <= val;
}

bool operator== (const Colour& lhs, const int& val)
{
    //    return lhs.m_r <= val || lhs.m_g <= val || lhs.m_b <= val;
    return (lhs.m_r+lhs.m_g+lhs.m_b)/3 == val;
}

bool operator>= (const Colour& lhs, const Colour& rhs)
{
    return (lhs.m_r >= rhs.m_r) && (lhs.m_g >= rhs.m_g) && (lhs.m_b >= rhs.m_b);
    //return (r+g+b)/3 > (c.getR()+c.getG()+c.getB())/3;
}

bool operator<= (const Colour& lhs, const Colour& rhs)
{
    return lhs.m_r <= rhs.m_r && lhs.m_g <= rhs.m_g && lhs.m_b <= rhs.m_b;
    //return (r+g+b)/3 < (c.getR()+c.getG()+c.getB())/3;
}

bool operator== (const Colour& lhs, const Colour& rhs)
{
    return lhs.m_r == rhs.m_r && lhs.m_g == rhs.m_g && lhs.m_b == rhs.m_b;
    //return (r+g+b)/3 < (c.getR()+c.getG()+c.getB())/3;
}


Colour& Colour::operator+=(const int& rhs)
{
    m_r += rhs;
    m_g += rhs;
    m_b += rhs;
    m_a -= rhs;
    return *this;
}

Colour& Colour::operator-=(const int& rhs)
{
    m_r -= rhs;
    m_g -= rhs;
    m_b -= rhs;
    m_a -= rhs;
    return *this;
}

//NOTE: Might make sense to give functions other than
//      inc() and dec() for changing values by more
void Colour::Inc(int colour, int qty)
{
    switch ( colour )
    {
        case COLOUR_RED:
            m_r = std::min(m_r+qty,MAX_RGB);
            break;
            
        case COLOUR_GREEN:
            m_g = std::min(m_g+qty,MAX_RGB);
            break;
            
        case COLOUR_BLUE:
            m_b = std::min(m_b+qty,MAX_RGB);
            break;
            
        case COLOUR_ALPHA:
            m_a = std::min(m_a+qty,MAX_RGB);
            break;
            
        default:
            m_r = std::min(m_r+qty,MAX_RGB);
            m_g = std::min(m_g+qty,MAX_RGB);
            m_b = std::min(m_b+qty,MAX_RGB);
            break;
    }
}

void Colour::Dec(int colour, int qty)
{
    switch ( colour )
    {
        case COLOUR_RED:
            m_r = std::max(m_r-qty,MIN_RGB);
            break;
            
        case COLOUR_GREEN:
            m_g = std::max(m_g-qty,MIN_RGB);
            break;
            
        case COLOUR_BLUE:
            m_b = std::max(m_b-qty,MIN_RGB);
            break;
            
        case COLOUR_ALPHA:
            m_a = std::max(m_a-qty,MIN_RGB);
            break;
            
        default:
            m_r = std::max(m_r-qty,MIN_RGB);
            m_g = std::max(m_g-qty,MIN_RGB);
            m_b = std::max(m_b-qty,MIN_RGB);
            break;
    }
}