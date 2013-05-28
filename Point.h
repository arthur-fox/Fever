//
//  Point.h
//  Fever
//
//  Created by Arthur Fox on 06/04/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Point_h
#define Fever_Point_h

// Simple 2D point class

class Point
{
public:
    
    Point();
    Point( float x, float y );
    
    inline int GetX() const {return m_x;}
    inline int GetY() const {return m_y;}
    inline void SetX( float x ) {m_x = x;}
    inline void SetY( float y ) {m_y = y;}
    
private:
    
    float m_x, m_y;
};

#endif
