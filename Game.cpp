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
	this->population = new Population(50); //Create population of 50 snakes
}

void Game::initNNWindow()
{
	this->netMode.height = 600;
	this->netMode.width = 900;
	this->netWindow.create(this->netMode, "Neural Network");
	this->netWindow.setPosition({ 700,600 });
}

Game::Game()
{
	//Find what the best fitness was before program ended
	std::ifstream file("BestSnakeBrain/BestFitnessScore.txt");
	if (file.is_open()) {
		while (!file.eof()) {
			file >> this->prevBest;
		}
	}

	//If program was not run prior previous best fitness is 0
	else
		this->prevBest = 0;

	//Create game
	this->initWindow();
	this->initPopulation();
	this->count = 0;

	//Create window for neural network
	this->initNNWindow();
}

Game::~Game()
{
}

void Game::updatePopulation()
{
	//Adjust positions of snakes if they are alive
	this->population->updateAlive();

	//If the entire population is dead start evolution
	if (this->population->done()) {
		this->population->calculateFitness();

		//If best fitness of the population is better than the best fitness in file write fitness to file
		if (this->population->globalBest > this->prevBest) {
			this->prevBest = this->population->globalBest;
			std::ofstream file;
			file.open("BestSnakeBrain/BestFitnessScore.txt");
			file << this->prevBest;
			file.close();
		}

		//Output generation, fitness, and score to terminal
		std::cout << this->population->gen <<  " Global best fitness is: ";
		std::cout << this->population->globalBestFitness << " ";
		std::cout << " Global best score is " <<this->population->globalBest << '\n';

		//create new population
		this->population->naturalSelection();
	}
}

void Game::update()
{
	//Whether to show all snakes or just the best
	if (Keyboard::isKeyPressed(Keyboard::A)) this->population->drawAll = true;
	if (Keyboard::isKeyPressed(Keyboard::S)) this->population->drawAll = false;

	//Human play
	if (Keyboard::isKeyPressed(Keyboard::Up)) this->population->snakes[0].Direction = 1;
	if (Keyboard::isKeyPressed(Keyboard::Down)) this->population->snakes[0].Direction = 3;
	if (Keyboard::isKeyPressed(Keyboard::Left)) this->population->snakes[0].Direction = 0;
	if (Keyboard::isKeyPressed(Keyboard::Right)) this->population->snakes[0].Direction = 2;

	//If best score from population is better than in file write best DNA to file
	if (this->population->globalBest > this->prevBest) {
		this->population->snakes[0].brain.writeRecordToFile("BestSnakeBrain/DNA.txt");
	}

	//For the first generation have the first 20 snakes be loaded with the best DNA
	if (this->population->gen == 1) {
		for (int i = 0; i < 20; i++) {
			this->population->snakes[i].brain.readFiletoNetwork("BestSnakeBrain/DNA.txt");
			this->population->snakes[i].mutate(5);
		}
	}

	this->updatePopulation();
}

void Game::renderPopulation()
{
	this->population->render(this->window); //Draw all snakes
	this->population->snakes[0].brain.renderNet(this->netWindow); //Draw brain of best snake
}

void Game::render()
{
	//Create both windows and display
	this->window.clear();
	this->netWindow.clear();
	this->renderPopulation();
	this->window.display();
	this->netWindow.display();
}

const RenderWindow& Game::getWindow()
{
	return this->window;
}

const RenderWindow& Game::getNNWindow()
{
	return this->netWindow;
}
