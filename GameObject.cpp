//
//  GameObject.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "GameObject.h"
#include "Global.h"

GameObject::CollisionBox::CollisionBox()
{
	x1 = x2 = y1 = y2 = 0;
}

void GameObject::Render( SDL_Surface* pScreen ) const
{
    Global::SharedGlobal()->ApplySurface( m_x, m_y, m_pImage, pScreen );
}