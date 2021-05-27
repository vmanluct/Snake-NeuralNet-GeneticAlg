#include <SFML\Graphics.hpp>
#include "Population.h"

int cols = 30;
int rows = 20;
int size = 16;
int w = size * cols;
int h = size * rows;
//Snake snake = Snake(rand() % 30, rand() % 20, rand() % 30, rand() % 20);


using namespace sf;

int main() {
	srand(time(0));
	Population pop = Population(1);
	//Snake snake = Snake(rand() % 30, rand() % 20, rand() % 30, rand() % 20);

	//Snake snake = Snake(rand() % 30, rand() % 20, rand() % 30, rand() % 20);
	/*for (int i = 0; i < 5; i++) {
		snakes[i] = Snake(rand() % 30, rand() % 20, rand() % 30, rand() % 30);
	}*/

	RenderWindow window(VideoMode(w, h), "Snake");

	Clock clk;
	float timer = 0, delay = 0.1;

	while (window.isOpen()) {
		float time = clk.getElapsedTime().asSeconds();
		clk.restart();
		timer += time;
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
		}

		if (timer > delay ) {
			window.clear();

			if (Keyboard::isKeyPressed(Keyboard::Up)) pop.snakes[0].Direction = 0;
			if (Keyboard::isKeyPressed(Keyboard::Down)) pop.snakes[0].Direction = 1;
			if (Keyboard::isKeyPressed(Keyboard::Right)) pop.snakes[0].Direction = 2;
			if (Keyboard::isKeyPressed(Keyboard::Left)) pop.snakes[0].Direction = 3;

			if (Keyboard::isKeyPressed(Keyboard::A)) pop.drawAll = false;
			if (Keyboard::isKeyPressed(Keyboard::S)) pop.drawAll = true;


			pop.updateAlive();//snake.update();

			pop.draw(window);//snake.draw(window);

			if (pop.done()) {
				pop.calculateFitness();
				std::cout << pop.snakes[0].fitness << '\n';
				pop.naturalSelection();
			}


			window.display();
			timer = 0;
		}
	}
}