//
//  Camera.cpp
//  Fever
//
//  Created by Arthur Fox on 28/05/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "Camera.h"

const int CAMERA_SPEED = SCREEN_HEIGHT*2;
const float CAMERA_UPPER = 1/9.f;
const float CAMERA_LOWER = 2/3.f;

Camera::Camera()
{
    m_x = 0; //UNUSED
    m_y = -SCREEN_HEIGHT/2;
    m_w = SCREEN_WIDTH; //UNUSED
    m_h = SCREEN_HEIGHT;
}

bool Camera::Update(float dt, Point playerPos)
{
    float diffY = playerPos.GetY() - m_y;
    
    if (diffY < m_h * CAMERA_UPPER)
    {   
        if (m_yVel > -CAMERA_SPEED)
            m_yVel -= CAMERA_SPEED * (dt / 1000.f);
    }
    else if (diffY > m_h * CAMERA_LOWER)
    {
        if (m_yVel < CAMERA_SPEED)
            m_yVel += CAMERA_SPEED * (dt / 1000.f);   
    }
    else
    {
        m_yVel = 0;
    }
//    printf("yvel: %f\n", m_yVel);
    
    m_y += m_yVel * (dt / 1000.f);
    
    return true;
}