//
//  main.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "GameDirector.h"
#include "Global.h"

//IMPORTANT NOTE:
//  SWITCHED COMPILER FLAG
//  C++ Standard Library = C++11
//  IF THINGS START BREAKING SET IT BACK TO COMPILER DEFAULT

//TODO:
// -> Make background have parallax effect!
// -> Try and discover what the ifstream/ofstream bug is in level generation.
//
// -> Delete all levels and Gen them again, just in case!
// -> Look at game speed! Feels too fast on tonight now, made me dizzy. Check if 4 is too much.
// -> Improve Note generation
// -> Profile game again
// -> Implement some effects when hitting note or losing multiplier

// -> Consider looking at brightness to act as valence - just need it to vary green channel -> Green down = darker, green up = lighter.


int main( int argc, char* args[] )
{
    //srand( (int)time(NULL) );// Uncomment for debugging
    
    Global::SharedGlobal()->Initialisation();
    
	GameDirector *director = new GameDirector(); 
    
    director->Run();
    
    delete director;
    
    Global::SharedGlobal()->CleanUp();
	
	return 0;

}
