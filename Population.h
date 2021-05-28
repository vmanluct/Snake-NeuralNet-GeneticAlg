#pragma once
#include "Snake.h"

#define POPULATION_SIZE 100

class Population
{
public:
	Snake* snakes;
	bool drawAll = true;
	int gen = 1;
	int globalBest = 1;
	long globalBestFitness = 0;
	long globalMutationRate = 0.01;
	int currentBest = 1;
	int currentBestSnake = 0;

	Snake globalBestSnake;

	Population() {
		//Default Constructor
	}

	Population(int size) {
		size = POPULATION_SIZE;
		snakes = new Snake[size];

		for (int i = 0; i < size; i++) {
			snakes[i] = Snake(rand()%30, rand()%20, rand()%30, rand()%20);
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

	void draw(RenderWindow& window);
};

