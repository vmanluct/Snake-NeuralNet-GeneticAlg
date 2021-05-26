#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define TEXTURE_SIZE 16
#define MAX_BODY 100

using namespace sf;
class Snake {
public:

	int Direction, size;
	float score;
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
		Direction = 0; size = 1; score = 0; Dead = false;

		b[0].x = xStart; b[0].y = yStart; foodPos.first = foodStartX; foodPos.second = foodStartY;
		
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
	}

	void move(int dir);

	void update();

	void draw(RenderWindow& window);

	void eat();

	void ConnectBody();

	void Collision();
   
};

