#include <SFML/Graphics.hpp>
#include "Snake.h"

void Snake::mutate(float mr)
{
    brain.mutate(mr);
}

void Snake::setVelocity()
{
    decision = brain.output(vision);

    float max = 0;
    int maxIndex = 0;
    int size = sizeof(decision) / sizeof(decision[0]);
    for (int i = 0; i < size;i++) {
        if (max < decision[i]) {
            max = decision[i];
            maxIndex = i;
        }
    }

    move(maxIndex);
}

void Snake::move(int dir)
{
    Direction = dir;
    if (dir == 0) { b[0].y -= 1; }
    else if (dir == 1) { b[0].y += 1; }
    else if (dir == 2) { b[0].x += 1; }
    else if (dir == 3) { b[0].x -= 1; }
    bodySprite[0].setPosition(b[0].x * TEXTURE_SIZE, b[0].y * TEXTURE_SIZE);
}

void Snake::update()
{
    move(Direction);
    ConnectBody();
    eat();
    Collision();
}

void Snake::draw(RenderWindow& window)
{
    for (int i = 0; i < size; i++) {
        window.draw(bodySprite[i]);
    }
    window.draw(foodSprite);
}

void Snake::eat()
{
    if (b[0].x == foodPos.first && b[0].y == foodPos.second) {
        size++;
        foodPos.first = rand() % 30;
        foodPos.second = rand() % 20;
        foodSprite.setPosition(foodPos.first*TEXTURE_SIZE, foodPos.second*TEXTURE_SIZE);
        b[size].x = b[size - 1].x; b[size].y = b[size - 1].y;
    }
}

void Snake::ConnectBody()
{
    for (int i = size;i > 0;--i)
    {
        b[i].x = b[i - 1].x;
        b[i].y = b[i - 1].y;
        bodySprite[i].setPosition(b[i].x * TEXTURE_SIZE, b[i].y * TEXTURE_SIZE);
    }
}

void Snake::Collision()
{
    //Collision with body
    for (int i = 2; i < size; i++) {
        if (b[0].x == b[i].x && b[0].y == b[i].y)
            Dead = true;
    }
    //Collision with wall
    if (b[0].x > 30) { Dead = true; }
    else if (b[0].x < 0) {
        Dead = true; 
    }
    else if (b[0].y > 20) {
        Dead = true; 
    }
    else if (b[0].y < 0) {
        Dead = true; 
    }
}

void Snake::calcFitness()
{
    if (size < 10) {
        fitness = floor(lifeTime * lifeTime * pow(2, (floor(size))));
    }
    else {
        fitness = lifeTime * lifeTime;
        fitness *= pow(2, 10);
        fitness *= (size - 9);
    }
}

Snake Snake::crossover(Snake Partner)
{
    Snake child = Snake();
    child.brain = brain.crossover(Partner.brain);

    return child;
}

Snake Snake::clone()
{
    Snake clone = Snake();
    clone.brain = brain.clone();
    clone.Dead = false;
    return clone;
}

float* Snake::lookInDirection(std::pair<int, int> direction)
{
    float visionInDirection[3];

    std::pair<int, int> position;
    position.first = pos.first;
    position.second = pos.second;
    bool tailIsFound = false;
    bool foodIsFound = false;
    float distance = 0;

    position.first += direction.first;
    position.second += direction.second;

    distance++;

    while (!(position.first < 0 || position.first > 30
        || position.second < 0 || position.second > 20)) {

        if (!foodIsFound && position.first == foodPos.first &&
            position.second == foodPos.second) {
            visionInDirection[0] = 1;
            foodIsFound = true;
        }

        if(!tailIsFound) //keep adding here

    }
}


