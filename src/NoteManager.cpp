//
//  CoinManager.cpp
//  Fever
//
//  Created by Arthur Fox on 02/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "NoteManager.h"
#include "Global.h"


NoteManager::NoteManager(float speed, float freq, int width, int height, Floor* pFloor)
{
	m_speed = speed;   //int
    m_noteFreq = freq; //always 180*2?
    m_width = width;
    m_height = height;
    m_pFloor = pFloor;
    
    m_creatingNotes = true;
    m_numTouched = 0;
    
    NewNote(m_width, m_height);
}


void NoteManager::NewNote(int initialX, int initialY)
{
	Note note(m_speed, initialX, initialY, m_pFloor);
	notes.push_back(note);
}


void NoteManager::SetSpeed(int s)
{
	m_speed = s;
	
	for (m_it=notes.begin(); m_it < notes.end(); m_it++)
		m_it->SetSpeed(m_speed);
}


void NoteManager::Update(float dt)
{	
	for (m_it=notes.begin(); m_it < notes.end(); m_it++)
		m_it->Update(dt);
    
    if (m_creatingNotes && (notes.empty() || (notes.back().GetX()<= SCREEN_WIDTH - m_noteFreq)))
    {
        //NOTE: Not random since srand() always init to same seed
        int floorHeight = m_pFloor->GetLastHeight() - PLAYER_HEIGHT/2;
        
//        int jumpHeight = floorHeight - PLAYER_JUMP;
//        int range = (floorHeight - jumpHeight);
        
//        if (range == 0) range = 1; //Avoids division by 0
        int randHeight = floorHeight - (rand() % PLAYER_JUMP);
        
//        while (randHeight < NOTE_HEIGHT) randHeight += NOTE_HEIGHT/2;
//        while (randHeight + NOTE_HEIGHT >= floorHeight) randHeight -= NOTE_HEIGHT/2;
//        if (randHeight < NOTE_HEIGHT) randHeight = NOTE_HEIGHT; //Last ditch attempt - Ensures notes not too far up
        
        NewNote(m_width, randHeight);
    }
}


void NoteManager::Render(SDL_Surface* pScreen, Camera& rCamera)
{	
	for (m_it=notes.begin(); m_it < notes.end(); m_it++)
		m_it->Render(pScreen, rCamera);
}

//Only public access method which adds a new note to the notes vector 
bool NoteManager::AddNote(int initialX, int initialY)
{
	if (notes.empty() || (notes.back().GetX()<= SCREEN_WIDTH - m_noteFreq) )
    {
		NewNote(initialX, initialY);
		return true;
	}
	return false;
}

//Returns true if the object has touched a note
int NoteManager::TouchNote(GameObject& rObject)
{
    int ret = 0;
    m_it=notes.begin();
    std::vector<Note>::iterator end = notes.end();
    
    while (m_it != end && !m_it->IsVisible()) // Only need to check the first visible note
    {
        ret++;
        m_it++;
    }
    
    if (m_it != end && m_it->TouchCoin(rObject))
    {
        m_numTouched++;
        return ret;
    }
	
	return -1;
}

Note NoteManager::Remove(int num)
{
    if (!notes.empty())
    {
        m_it=notes.begin();
        for (int i = 0; i < num; i++)
        {
            m_it++;
        }
        
        Note note = notes[num];
        notes.erase(m_it);
        return note;
    }
    else
    {
        printf("Tried popping an element from an empty notes list");
        exit(1);
    }
}

//Removes notes which are not visible
// clearing notes means some have been missed, hence numTouched is reset
void NoteManager::ClearInvisible()
{	
	m_it=notes.begin();
    
	while (m_it < notes.end())
    {
		if (!m_it->IsVisible())
        {
			m_it = notes.erase(m_it);
            m_numTouched = 0;
        }
        
		else
            m_it++;
	}
}