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
private:
	Texture foodTexture;
	Texture bodyTexture;

	void initVariables();
	void initTexture();
	void initSprite();

	float distanceToApple;
	float movesScore;
	float penalty;
	int sameMoveCount;

	float calcDistanceToApple();

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


	RectangleShape bodySprite[100];
	RectangleShape foodSprite;

	Snake();
	~Snake();

	void mutate(float mr);

	void setVelocity();

	void move(int dir);

	void updateMovement();

	void update();


	void eat();

	void ConnectBody();

	void Collision();

	int calcFitness();

	Snake crossover(Snake Partner);

	Snake clone();

	bool isOnTail(int x, int y);

	float* lookInDirection(std::pair<int, int> direction); //need to finish

	void look();

	void render(RenderTarget& target);
   
};

