#include <SFML/Graphics.hpp>
#include "Snake.h"

void Snake::mutate(float mr)
{
    brain.mutate(mr);
}

void Snake::setVelocity()
{
    decision = brain.output(vision);
    float t1 = decision(0, 0);
    float t2 = decision(1, 0);
    float t3 = decision(2, 0);
    float t4 = decision(3, 0);

    float max = 0;
    int maxIndex = 0;
    int size = 4;//sizeof(decision) / sizeof(decision[0]);
    for (int i = 0; i < size;i++) {
        if (max < decision(i,0)) {
            max = decision(i,0);
            maxIndex = i;
        }
    }

    move(maxIndex);
}

void Snake::move(int dir)
{
    Direction = dir;
    if (dir == 0) { 
        b[0].x -= 1; 
        vel.first = -1;
        vel.second = -0;
    }
    else if (dir == 1) {
        b[0].y -= 1; 
        vel.first = 0;
        vel.second = -1;
    }
    else if (dir == 2) {
        b[0].x += 1;
        vel.first = 1;
        vel.second = 0;
    }
    else if (dir == 3) {
        b[0].y += 1;
        vel.first = 0;
        vel.second = 1;
    }
    pos.first = b[0].x; pos.second = b[0].y;
    bodySprite[0].setPosition(b[0].x * TEXTURE_SIZE, b[0].y * TEXTURE_SIZE);
}

void Snake::update()
{
    lifeTime++;
    lifeLeft--;
    if (lifeLeft <= 0) {
        fitness -= 1000;
        Dead = true;
    }
    else {
        eat();
        look();
        setVelocity();
        ConnectBody();
       // eat();
        Collision();
    }
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
        score++;
        lifeLeft += 100;
        fitness += 1000;
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
        lifeTime /= 2;
    }
    //Collision with wall
    if (b[0].x > 30) { 
        Dead = true; 
        lifeTime /= 2;
    }
    else if (b[0].x < 0) {
        Dead = true;
        lifeTime /= 2;
    }
    else if (b[0].y > 20) {
        Dead = true; 
        lifeTime /= 2;
    }
    else if (b[0].y < 0) {
        Dead = true; 
        lifeTime /= 2;
    }
}

void Snake::calcFitness()
{
    int x = pos.first - foodPos.first;
    int y = pos.second - foodPos.second;
    double xSq = pow(x, 2);
    double ySq = pow(y, 2);
    double dist = std::sqrt(xSq + ySq);

    if (dist < 5) fitness += 100;

    if (size < 10) {
        fitness += floor(lifeTime * lifeTime * pow(2, (floor(size))));
    }
    else {
        fitness += lifeTime * lifeTime;
        fitness *= pow(2, 10);
        fitness *= (size - 9);
    }
}

Snake Snake::crossover(Snake Partner)
{
    Snake child = Snake(rand()%30, rand()%20, rand()%30,rand()%20);
    child.brain = brain.crossover(Partner.brain);

    return child;
}

Snake Snake::clone()
{
    static Snake clone = Snake(rand()%30, rand()%20, rand()%30, rand()%20);
    clone.brain = brain.clone();
    clone.Dead = false;
    return clone;
}

bool Snake::isOnTail(int x, int y)
{
    for (int i = 2; i < size; i++) {
        if (x == b[i].x && y == b[i].y) return true;
    }
    return false;
}

float* Snake::lookInDirection(std::pair<int, int> direction)
{
    static float visionInDirection[3];

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

        if (!tailIsFound && isOnTail(position.first, position.second)) {
            visionInDirection[1] = 1 / distance;
            tailIsFound = true;
        }
        position.first += direction.first;
        position.second += direction.second;
        distance++;

    }
    visionInDirection[2] = 1 / distance;
    return visionInDirection;
}

void Snake::look()
{
    //look left
    float* tempVision = lookInDirection(std::pair<int, int>(1, 0));
    vision[0] = tempVision[0];
    vision[1] = tempVision[1];
    vision[2] = tempVision[2];
    //look left/up
    tempVision = lookInDirection(std::pair<int, int>(-1, -1));
    vision[3] = tempVision[0];
    vision[4] = tempVision[1];
    vision[5] = tempVision[2];

    //look up
    tempVision = lookInDirection(std::pair<int, int>(0, -1));
    vision[6] = tempVision[0];
    vision[7] = tempVision[1];
    vision[8] = tempVision[2];

    //look up/right
    tempVision = lookInDirection(std::pair<int, int>(1, -1));
    vision[9] = tempVision[0];
    vision[10] = tempVision[1];
    vision[11] = tempVision[2];

    //look right
    tempVision = lookInDirection(std::pair<int, int>(1, 0));
    vision[12] = tempVision[0];
    vision[13] = tempVision[1];
    vision[14] = tempVision[2];

    //look right/down
    tempVision = lookInDirection(std::pair<int, int>(1, 1));
    vision[15] = tempVision[0];
    vision[16] = tempVision[1];
    vision[17] = tempVision[2];

    //look down
    tempVision = lookInDirection(std::pair<int, int>(0, 1));
    vision[18] = tempVision[0];
    vision[19] = tempVision[1];
    vision[20] = tempVision[2];

    //look down/left
    tempVision = lookInDirection(std::pair<int, int>(-1, 1));
    vision[21] = tempVision[0];
    vision[22] = tempVision[1];
    vision[23] = tempVision[2];
}


