#include <algorithm>
#include "Population.h"

void Population::updateAlive()
{
	for (int i = 0; i < this->popSize; i++) {
		if (!snakes[i].Dead) {
			snakes[i].update();
		}
	}
	setCurrentBest();
}

bool Population::done()
{
	for (int i = 0; i < this->popSize; i++) {
		if (!snakes[i].Dead) return false;
	}
	return true;
}

void Population::calculateFitness()
{
	for (int i = 0; i < this->popSize; i++) {
		fitnessSum += snakes[i].calcFitness();
	}
	std::sort(snakes, snakes + this->popSize, [](Snake const& a, Snake const& b)->bool 
		{return a.fitness > b.fitness;});
}

void Population::naturalSelection()
{
	Snake* newSnakes = new Snake[this->popSize];

	setBestSnake();
	newSnakes[0] = globalBestSnake.clone();


	for (int i = 0; i < this->popSize; i++) {
		if (i < 5) {
			newSnakes[i] = snakes[i].clone();
		}
		else {
			Snake parent1 = selectSnake();
			Snake parent2 = selectSnake();

			Snake child = parent1.crossover(parent2);
			if (rand()%100 < globalMutationRate) {
				child.mutate(5);
			}
			newSnakes[i] = child;
		}
	}
	fitnessSum = 0;
	snakes = newSnakes;
	gen += 1;
	currentBest = 1;
}

Snake Population::selectSnake()
{
	/*float r = (rand()) % (int)fitnessSum;
	float summation = 0;
	for (int i = 0; i < this->popSize; i++) {
		summation += snakes[i].fitness;
		if (summation > r ) {
			return snakes[i];
		}
	}*/
	return snakes[rand() % 10];
}

void Population::setBestSnake()
{
	float max = 0;
	int maxIndex = 0;
	for (int i = 0; i < this->popSize; i++) {
		if (snakes[i].fitness > max) {
			max = snakes[i].fitness;
			maxIndex = i;
		}
	}

	if (max >= globalBestFitness) {
		globalBestFitness = max;
		globalBestSnake = snakes[maxIndex].clone();
	}

}

void Population::mutate()
{
	for (int i = 1; i < this->popSize; i++) {
		snakes[i].mutate(globalMutationRate);
	}
}

void Population::setCurrentBest()
{
	if (!done()) {
		float max = 0;
		int maxIndex = 0;
		for (int i = 0; i < this->popSize; i++) {
			if (!snakes[i].Dead && snakes[i].score > max) {
				max = snakes[i].score;
				maxIndex = i;
			}
		}

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
	if (drawAll) {
		for (int i = 0; i < this->popSize; i++) {
			if (snakes[i].Dead == false)
				snakes[i].render(target);
		}
	}
	else snakes[0].render(target);
}
