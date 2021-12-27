#pragma once

#include <vector>
#include "Window.h"
#include "GameObject.h"
#include "MainMenu.h"
#include "Board.h"

using std::vector;



class GameController {

public:
	
	GameController();

	void run();
    bool isRunning();
	void handleKey();

private:
    bool running = true;
    MainMenu m_mainMenu;
	Window m_window;

//		vector of all the objects in the game
	vector <GameObject*> m_gameObjects;

};