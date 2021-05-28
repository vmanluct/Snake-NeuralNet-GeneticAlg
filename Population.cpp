#include "Population.h"

void Population::updateAlive()
{
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (!snakes[i].Dead)
			snakes[i].update();
	}
	setCurrentBest();
}

bool Population::done()
{
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (!snakes[i].Dead) return false;
	}
	return true;
}

void Population::calculateFitness()
{
	for (int i = 0; i < POPULATION_SIZE; i++) {
		snakes[i].calcFitness();
	}
}

void Population::naturalSelection()
{
	Snake* newSnakes = new Snake[POPULATION_SIZE];

	setBestSnake();
	newSnakes[0] = globalBestSnake.clone();


	for (int i = 1; i < POPULATION_SIZE; i++) {
		Snake parent1 = selectSnake();
		Snake parent2 = selectSnake();

		Snake child = parent1.crossover(parent2);
		child.mutate(globalMutationRate);
		newSnakes[i] = child;

		//snakes[i] = newSnakes[i].clone();
	}
	snakes = newSnakes;
	gen += 1;
	currentBest = 1;
}

Snake Population::selectSnake()
{
	long fitnessSum = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		fitnessSum += snakes[i].fitness;
	}
	long r = floor((rand() +rand())%fitnessSum);


	long runningSum = 0;

	for (int i = 0; i < POPULATION_SIZE; i++) {
		runningSum += snakes[i].fitness;
		if (runningSum > r){
			return snakes[i];
		}
	}

	return snakes[0];
}

void Population::setBestSnake()
{
	long max = 0;
	int maxIndex = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (snakes[i].fitness > max) {
			max = snakes[i].fitness;
			maxIndex = i;
		}
	}

	if (max > globalBestFitness) {
		globalBestFitness = max;
		globalBestSnake = snakes[maxIndex].clone();
	}
}

void Population::mutate()
{
	for (int i = 1; i < POPULATION_SIZE; i++) {
		snakes[i].mutate(globalMutationRate);
	}
}

void Population::setCurrentBest()
{
	if (!done());
	float max = 0;
	int maxIndex = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (!snakes[i].Dead && snakes[i].size > max) {
			max = snakes[i].size;
			maxIndex = i;
		}
	}

	if (max > currentBest) {
		currentBest = floor(max);
	}

	if (snakes[currentBestSnake].Dead ||
		max > snakes[currentBestSnake].size + 5) {
		currentBestSnake = maxIndex;
	}

	if (currentBest > globalBest) {
		globalBest = currentBest;
	}
}

void Population::draw(RenderWindow& window)
{
	if (drawAll) {
		for (int i = 0; i < POPULATION_SIZE; i++) {
			snakes[i].draw(window);
		}
	}
	else snakes[0].draw(window);
}
