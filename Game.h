#pragma once
#include <SFML/Graphics.hpp>
#include "Population.h"

using namespace sf;
class Game
{
private:
	//Window
	RenderWindow window;
	VideoMode videoMode;
	Event ev;

	Population* population;

	void initWindow();

	void initPopulation();

	float prevBestFitness;
	int count;

public:
	Game();
	~Game();

	void updatePopulation();
	void update();
	void renderPopulation();
	void render();
	const RenderWindow& getWindow();
};

