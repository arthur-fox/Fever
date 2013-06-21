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
// -> Add the final fantasy scrolling sounds, for when the player scrolls on the menu!
// -> Add general UI sounds - this will require MUTE to be available in the menu as well!
// -> Add "New Record" sound clip from smash bros when the level finishes and you get a new record!
// -> Delete all levels and Gen them again, just in case!
// -> Look at game speed! Feels too fast on tonight now, made me dizzy.
// -> Make background have parallax effect!
// -> Improve Note generation
// -> Try and discover what the ifstream/ofstream bug is in level generation.
// -> Profile game again
// -> Implement some effects when hitting note or losing multiplier
// -> Check if a levelspeed factor of 4 is too much.

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
