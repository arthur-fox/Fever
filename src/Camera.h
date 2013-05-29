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

class Camera
{
public:
    
    Camera();
    
    inline int GetX() const {return camera.x;}
    inline int GetY() const {return camera.y;}
    inline void SetX( float x ) {camera.x = x;}
    inline void SetY( float y ) {camera.y = y;}
    
private:
    
    SDL_Rect camera;
};

#endif
