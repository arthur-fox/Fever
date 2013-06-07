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
// -> Give different forms of level generation option.
// -> Expand the High Scores menu to show all scores.
// -> Change game ICN to have Mozart instead.
// -> Display level name at the top?
// -> Add the final fantasy scrolling sounds, for when the player scrolls on the menu!
// -> Add general UI sounds - this will require MUTE to be available in the menu as well!
// -> Add "New Record" sound clip from smash bros when the level finishes and you get a new record!
// -> Try and discover what the ifstream/ofstream bug is in level generation.
// -> Improve Note generation
// -> Improve Colour generation
// -> Profile game again

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
