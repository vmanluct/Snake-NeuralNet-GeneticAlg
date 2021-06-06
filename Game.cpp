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
	std::ifstream file("BestSnakeBrain/BestFitnessScore.txt");
	if (file.is_open()) {
		while (!file.eof()) {
			file >> this->prevBestFitness;
		}
	}
	else
		this->prevBestFitness = 0;

	this->initWindow();
	this->initPopulation();
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
			std::ofstream file;
			file.open("BestSnakeBrain/BestFitnessScore.txt");
			file << this->prevBestFitness;
			file.close();
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
	if (Keyboard::isKeyPressed(Keyboard::Up)) this->population->snakes[0].Direction = 1;
	if (Keyboard::isKeyPressed(Keyboard::Down)) this->population->snakes[0].Direction = 3;
	if (Keyboard::isKeyPressed(Keyboard::Left)) this->population->snakes[0].Direction = 0;
	if (Keyboard::isKeyPressed(Keyboard::Right)) this->population->snakes[0].Direction = 2;

	if (this->population->globalBestFitness > this->prevBestFitness) {
		this->population->snakes[0].brain.writeRecordToFile("BestSnakeBrain/DNA.txt");
	}

	if (this->population->gen == 1) {
		for (int i = 0; i < 4; i++) {
			this->population->snakes[i].brain.readFiletoNetwork("BestSnakeBrain/DNA.txt");
			this->population->snakes[i].mutate(50);
		}
	}

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
