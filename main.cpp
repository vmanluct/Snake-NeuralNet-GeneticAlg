#include <SFML\Graphics.hpp>
#include "Snake.h"

int cols = 30;
int rows = 20;
int size = 16;
int w = size * cols;
int h = size * rows;


using namespace sf;

int main() {
	srand(time(0));

	Snake snake = Snake(rand() % 30, rand() % 20, rand() % 30, rand() % 20);
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

		if (timer > delay && !snake.Dead) {
			window.clear();

			if (Keyboard::isKeyPressed(Keyboard::Up)) snake.Direction = 0;
			if (Keyboard::isKeyPressed(Keyboard::Down)) snake.Direction = 1;
			if (Keyboard::isKeyPressed(Keyboard::Right)) snake.Direction = 2;
			if (Keyboard::isKeyPressed(Keyboard::Left)) snake.Direction = 3;

			snake.update();

			snake.draw(window);

			window.display();
			timer = 0;
		}
	}
}