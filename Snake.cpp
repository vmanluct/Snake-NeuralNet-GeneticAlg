#include <SFML/Graphics.hpp>
#include "Snake.h"

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


