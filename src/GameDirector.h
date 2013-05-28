//
//  GameDirector.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//


#ifndef Fever_GameDirector_h
#define Fever_GameDirector_h

#include "Global.h"

// GameDirector knows how to run the game and when other directors should be delegated

class GameDirector 
{
public:
    
    GameDirector();
    ~GameDirector();
	void Run();

private:

	SDL_Surface *m_pScreen;
};

#endif
