#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Population.h"

using namespace sf;
class Game
{
private:
	//Window
	RenderWindow window;
	VideoMode videoMode;
	Event ev;

	Population* population; //All snakes for evolution

	void initWindow(); //Create window for game
	void initPopulation(); //Add snakes to population

	//Neural Network stuff
	float prevBest;
	int count;

	//Neural Network Graphics
	RenderWindow netWindow;
	VideoMode netMode;
	Event netEb;

	void initNNWindow(); //Create window for neural network

public:
	Game(); //Constructor
	~Game(); //Destructor

	void updatePopulation(); //Update positions of snakes
	void update(); //update everything to the windows
	void renderPopulation(); //Draw all the snakes to the window
	void render(); //Draw game and neural network

	//Getter functions
	const RenderWindow& getWindow();
	const RenderWindow& getNNWindow();
};

