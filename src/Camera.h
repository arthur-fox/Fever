//
//  Camera.h
//  Fever
//
//  Created by Arthur Fox on 28/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Camera_h
#define Fever_Camera_h

#include "Global.h"
#include "Point.h"

class Camera
{
public:
    
    Camera();
    
    bool Update(float dt, Point playerPosition);
    
    inline float GetX() const {return m_x;}
    inline float GetY() const {return m_y;}
//    inline void SetX( float x ) {m_x = x;}
//    inline void SetY( float y ) {m_y = y;}
    
private:
    
    float m_x, m_y, m_w, m_h;
    float m_xVel, m_yVel;
};

#endif
