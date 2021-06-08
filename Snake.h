#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "NeuralNet.h"

#define TEXTURE_SIZE 16
#define MAX_BODY 20

using namespace sf;
using namespace Eigen;
class Snake {
private:
	//Initializers
	void initVariables();
	void initSprite();

	//Neural network stuff
	float distanceToApple;
	float penalty;

	//Calculate Euclidean distance to the apple
	float calcDistanceToApple();

public:
	
	//Variables for how well snake is doing
	int score;
	int lifeLeft;
	int lifeTime;
	float fitness;

	std::pair<int, int> pos; //position of head
	std::pair<int, int> vel; //what direction snake will move
	std::pair<int, int> temp; //temporary vector

	//Neural network stuff
	NeuralNet brain;
	float vision[11];
	MatrixXf decision;

	//Stuff for snake
	int Direction, size;
	bool Dead;
	
	//Hold locations for food, and all the body
	struct body { int x; int y; }b[MAX_BODY];
	std::pair<int, int> foodPos;

	//Shapes to display game objects
	RectangleShape bodySprite[MAX_BODY];
	RectangleShape foodSprite;

	Snake();
	~Snake();

	void mutate(float mr); //Mutate brain of the snake

	void move(int dir); //Move the snake

	void updateMovement(); //Adjust positions of all snakes body

	void update(); //Update everything

	void eat(); //Check if head collided with food

	void ConnectBody(); //Connect all the body pieces together

	void Collision(); //Check if snake hit body or walls

	float calcFitness(); //See how well snake is doing

	Snake crossover(Snake Partner); //Breed two snakes to create a child

	Snake clone(); //Create a deep copy of the snake

	bool isOnTail(int x, int y); //Check if head is intersecting and body part

	void render(RenderTarget& target); //Draw snake and food to the screen
   

	//Vision functions for the snake new
	int dangerStraight();
	int dangerLeft();
	int dangerRight();

	//Directions of food
	int foodLeft();
	int foodUp();
	int foodRight();
	int foodDown();

	//Ai movement
	void moveStraight();
	void moveRight();
	void moveLeft();

	//Direction of Snake
	int movingLeft();
	int movingUp();
	int movingRight();
	int movingDown();

	//Ai Decision functions
	void think();
	void makeMove();

};

