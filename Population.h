#pragma once
#include "Snake.h"


class Population
{
public:
	Snake* snakes;
	bool drawAll = true;
	int gen = 1;
	int globalBest = 0;
	float globalBestFitness = 0;
	float globalMutationRate = 20;
	int currentBest = 0;
	int currentBestSnake = 0;
	int popSize;
	float fitnessSum = 0;

	Snake globalBestSnake;

	Population() {
		//Default Constructor
	}

	Population(int size) {
		srand(time(0));

		this->popSize = size;
		snakes = new Snake[size];

		for (int i = 0; i < this->popSize; i++) {
			snakes[i] = Snake();
		}
		globalBestSnake = snakes[0].clone();
	}

	void updateAlive();

	bool done();

	void calculateFitness();

	void naturalSelection();

	Snake selectSnake();

	void setBestSnake();

	void mutate();

	void setCurrentBest();

	void render(RenderTarget& target);
};

