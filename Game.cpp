#include "Game.h"

void Game::initWindow()
{
	this->videoMode.height = 320;
	this->videoMode.width = 480;
	this->window.create(this->videoMode, "Snake AI");
	this->window.setFramerateLimit(20);
}

void Game::initPopulation()
{
	this->population = new Population(10);
}

Game::Game()
{
	this->initWindow();
	this->initPopulation();
}

Game::~Game()
{
}

void Game::updatePopulation()
{
	this->population->updateAlive();
	if (this->population->done()) {
		this->population->calculateFitness();
		std::cout << this->population->gen <<  " Global best fitness is: ";
		std::cout << this->population->globalBestFitness << '\n';
		this->population->naturalSelection();
	}
}

void Game::update()
{
	if (Keyboard::isKeyPressed(Keyboard::A)) this->population->drawAll = true;
	if (Keyboard::isKeyPressed(Keyboard::S)) this->population->drawAll = false;

	this->updatePopulation();
}

void Game::renderPopulation()
{
	this->population->render(this->window);
}

void Game::render()
{
	this->window.clear();
	this->renderPopulation();
	this->window.display();
}

const RenderWindow& Game::getWindow()
{
	return this->window;
}
