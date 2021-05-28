#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "NeuralNet.h"

#define TEXTURE_SIZE 16
#define MAX_BODY 100

using namespace sf;
using namespace Eigen;
class Snake {
public:
	
	int score;
	int lifeLeft;
	int lifeTime;
	float fitness;

	std::pair<int, int> pos; //position of head
	std::pair<int, int> vel; //what direction snake will move
	std::pair<int, int> temp; //temporary vector

	NeuralNet brain;

	float vision[24];
	MatrixXf decision;

	int growCount;
	bool test;

	int Direction, size;
	bool Dead;
	std::string foodPath = "Assets/red.png";
	std::string bodyPath = "Assets/green.png";
	struct body { int x; int y; }b[MAX_BODY];
	std::pair<int, int> foodPos;

	Texture foodTexture;
	Texture bodyTexture;

	Sprite bodySprite[100];
	Sprite foodSprite;

	Snake(){
		//Default Constructor
	}

	Snake(int xStart, int yStart, int foodStartX, int foodStartY) {
		Direction = rand()%4; 
		size = 1; score = 1; Dead = false;
		pos.first = xStart; pos.second = yStart;
		vel.first = 0; vel.second = -1;
		b[0].x = xStart; b[0].y = yStart; 
		foodPos.first = xStart; foodPos.second = yStart;
		
		if (!foodTexture.loadFromFile(foodPath) ||
			!bodyTexture.loadFromFile(bodyPath)) std::cout << "Error, file not found";
		foodSprite.setTexture(foodTexture);
		bodySprite[0].setTexture(bodyTexture);
		
		foodSprite.setPosition(foodPos.first*TEXTURE_SIZE, foodPos.second*TEXTURE_SIZE);
		bodySprite[0].setPosition(b[0].x * TEXTURE_SIZE, b[0].y * TEXTURE_SIZE);

		for (int i = 1; i < MAX_BODY; i++) {
			b[i].x = 0; b[i].y = 0;
			bodySprite[i].setTexture(bodyTexture);
		}

		brain = NeuralNet(24, 18, 4);
		lifeLeft = 50;
		lifeTime = 0;
		fitness = 0;

		/*vision = new float[24]();
		decision = new float[4]();*/
		//vision[24];

	}

	void mutate(float mr);

	void setVelocity();

	void move(int dir);

	void update();

	void draw(RenderWindow& window);

	void eat();

	void ConnectBody();

	void Collision();

	void calcFitness();

	Snake crossover(Snake Partner);

	Snake clone();

	bool isOnTail(int x, int y);

	float* lookInDirection(std::pair<int, int> direction); //need to finish

	void look();
   
};

