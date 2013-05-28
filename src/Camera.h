//
//  Camera.h
//  Fever
//
//  Created by Arthur Fox on 28/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Camera_h
#define Fever_Camera_h

class Camera
{
public:
    
    Camera();
    
    inline int GetX() const {return m_x;}
    inline int GetY() const {return m_y;}
    inline void SetX( float x ) {m_x = x;}
    inline void SetY( float y ) {m_y = y;}
    
private:
    
    float m_x, m_y;
};

#endif
