#include <algorithm>
#include "Population.h"

Population::Population(int size)
{
	srand(time(0));

	//initialize game objects
	this->popSize = size;
	snakes = new Snake[size];

	//Create each snake
	for (int i = 0; i < this->popSize; i++) {
		snakes[i] = Snake();
	}

	//Set the best snake initially
	globalBestSnake = snakes[0].clone();
}

void Population::updateAlive()
{
	//Update snakes that are alive
	for (int i = 0; i < this->popSize; i++) {
		if (!snakes[i].Dead) {
			snakes[i].update();
		}
	}

	//Set the best snake
	setCurrentBest();
}

bool Population::done()
{
	//Check if all snakes are dead, if not return false
	for (int i = 0; i < this->popSize; i++) {
		if (!snakes[i].Dead) return false;
	}
	return true;
}

void Population::calculateFitness()
{
	//Get score of all snakes
	for (int i = 0; i < this->popSize; i++) {
		fitnessSum += snakes[i].calcFitness();
	}

	//Sort snakes in descending order of fitness
	std::sort(snakes, snakes + this->popSize, [](Snake const& a, Snake const& b)->bool 
		{return a.fitness > b.fitness;});
}

void Population::naturalSelection()
{
	Snake* newSnakes = new Snake[this->popSize]; //Create new array of snakes
	setBestSnake(); //Find the best snake
	newSnakes[0] = globalBestSnake.clone(); //Set the best snake as the first snake of the next population

	//For all new snakes do crossover
	for (int i = 0; i < this->popSize; i++) {
		//Elitism, keep top 5 snakes the same
		if (i < 5) {
			newSnakes[i] = snakes[i].clone();
		}

		//Crossover between snakes
		else {
			Snake parent1 = selectSnake();
			Snake parent2 = selectSnake();

			Snake child = parent1.crossover(parent2);

			//5 percent of the population will have mutation as well
			if (rand()%100 < globalMutationRate) {
				child.mutate(5);
			}
			newSnakes[i] = child; //add new snakes to population
		}
	}

	//Reset scores, increase generation, assign snakes to newSnakes
	fitnessSum = 0;
	snakes = newSnakes;
	gen += 1;
	currentBest = 1;
}

Snake Population::selectSnake()
{
	//Choose random snake from top 10 best
	return snakes[rand() % 5];
}

void Population::setBestSnake()
{
	float max = 0;
	int maxIndex = 0;

	//Go through all snakes and find index with highest fitness
	for (int i = 0; i < this->popSize; i++) {
		if (snakes[i].fitness > max) {
			max = snakes[i].fitness;
			maxIndex = i;
		}
	}

	//If max score of best bird is better than all previous fitnesses assign snake as the best
	if (max >= globalBestFitness) {
		globalBestFitness = max;
		globalBestSnake = snakes[maxIndex].clone();
	}

}

void Population::mutate()
{
	//Mutate all snakes in population
	for (int i = 1; i < this->popSize; i++) {
		snakes[i].mutate(globalMutationRate);
	}
}

void Population::setCurrentBest()
{
	//While all snakes are not dead
	if (!done()) {
		float max = 0;
		int maxIndex = 0;

		//Find the index of the best snake
		for (int i = 0; i < this->popSize; i++) {
			if (!snakes[i].Dead && snakes[i].score > max) {
				max = snakes[i].score;
				maxIndex = i;
			}
		}

		//If best snake is better than global best adjust global variables
		if (max > currentBest) {
			currentBest = max;
		}

		if (snakes[currentBestSnake].Dead ||
			max > snakes[currentBestSnake].score + 5) {
			currentBestSnake = maxIndex;
		}

		if (currentBest >= globalBest) {
			globalBest = currentBest;
		}
	}
}


void Population::render(RenderTarget& target)
{
	//Draw all the snakes
	if (drawAll) {
		for (int i = 0; i < this->popSize; i++) {
			if (snakes[i].Dead == false)
				snakes[i].render(target);
		}
	}

	//Only draw the best snake
	else snakes[0].render(target);
}
