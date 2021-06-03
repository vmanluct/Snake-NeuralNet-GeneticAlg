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
	this->population = new Population(50);
}

Game::Game()
{
	this->initWindow();
	this->initPopulation();
	this->prevBestFitness = 0;
	this->count = 0;
}

Game::~Game()
{
}

void Game::updatePopulation()
{
	this->population->updateAlive();
	if (this->population->done()) {
		this->population->calculateFitness();
		if (this->population->globalBestFitness > this->prevBestFitness) {
			this->prevBestFitness = this->population->globalBestFitness;
		}
		std::cout << this->population->gen <<  " Global best fitness is: ";
		std::cout << this->population->globalBestFitness << " ";
		std::cout << " Global best score is " <<this->population->globalBest << '\n';
		this->population->naturalSelection();
	}
}

void Game::update()
{
	if (Keyboard::isKeyPressed(Keyboard::A)) this->population->drawAll = true;
	if (Keyboard::isKeyPressed(Keyboard::S)) this->population->drawAll = false;

	if (this->population->gen % 20 == 0 && this->population->globalBestFitness > this->prevBestFitness) {
		this->population->snakes[0].brain.writeRecordToFile("BestSnakeBrain/DNA.txt");
	}

	/*if (this->population->gen == 1) {
		for (int i = 0; i < 5; i++) {
			this->population->snakes[i].brain.readFiletoNetwork("BestSnakeBrain/DNA.txt");
			this->population->snakes[i].mutate(this->population->globalMutationRate / 5);
		}
	}*/

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
