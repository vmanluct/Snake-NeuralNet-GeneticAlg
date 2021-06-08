#pragma once
#include "Snake.h"


class Population
{
public:
	//Game objects
	Snake* snakes;

	bool drawAll = true; //Whether to draw all snakes or just one
	int gen = 1; //Generation number
	int globalBest = 0; //Index of best snake in population
	float globalBestFitness = 0; //Best fitness in all generations
	float globalMutationRate = 10; //What percent of the population will be mutated
	int currentBest = 0; //What the current best score is
	int currentBestSnake = 0; //what index the best snake was at
	int popSize; //How many snakes in population
	float fitnessSum = 0; //Total sum of all fitness of the snakes

	Snake globalBestSnake; //Copy of the best snake

	Population(int size); //Constructor

	void updateAlive(); //Update all snakes
	bool done(); //Check if all snakes are dead
	void calculateFitness(); //See how well each snake did
	void naturalSelection(); //Pick snakes for parents and reproduction
	Snake selectSnake(); //Pick snake parent
	void setBestSnake(); //Set the best snake
	void mutate(); //Mutate dna of the snake
	void setCurrentBest(); //Set the best score in the generation
	void render(RenderTarget& target); //Draw all snakes and neural network of best snake
};

