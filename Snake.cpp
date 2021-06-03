#include <SFML/Graphics.hpp>
#include "Snake.h"

void Snake::initVariables()
{
    int xStart = rand() % 25;
    int yStart = rand() % 15;
    int foodStartX = rand() % 30;
    int foodStartY = rand() % 20;
    pos.first = xStart;
    pos.second = yStart;
    vel.first = 0;
    vel.second = 0;
    b[0].x = xStart;
    b[0].y = yStart;
    foodPos.first = foodStartX;
    foodPos.second = foodStartY;
    this->sameMoveCount = 0;
    this->movesScore = 0;
    this->penalty = 0;
    this->distanceToApple = calcDistanceToApple();
    this->lifeLeft = 200;
    this->lifeTime = 0;
    this->Direction = 1;//rand() % 4;
    this->fitness = 0;
    this->size = 4;
    this->score = 0;
    this->Dead = false;
    this->brain = NeuralNet(24, 20, 4);
    
}

void Snake::initTexture()
{
    if (!this->bodyTexture.loadFromFile(bodyPath))
        std::cout << "Could not load body texture";
    if (!this->foodTexture.loadFromFile(foodPath))
        std::cout << "Could not load food texture";
}

void Snake::initSprite()
{
    /*this->foodSprite.setTexture(this->foodTexture);
    this->foodSprite.setPosition(foodPos.first*TEXTURE_SIZE,foodPos.second*TEXTURE_SIZE);
    for (int i = 0; i < MAX_BODY; i++) {
        bodySprite[i].setTexture(bodyTexture);
    }*/
    Color color = Color(rand() % 255, rand() % 255, rand() % 255);
    foodSprite.setSize({ 16, 16 });
    foodSprite.setFillColor(color);
    foodSprite.setPosition(foodPos.first * TEXTURE_SIZE, foodPos.second * TEXTURE_SIZE);
    for (int i = 0; i < MAX_BODY; i++) {
        bodySprite[i].setSize({ 16,16 });
        bodySprite[i].setFillColor(color);
    }
}

float Snake::calcDistanceToApple()
{
    float x = powf((pos.first - foodPos.first), 2);
    float y = powf((pos.second - foodPos.second), 2);
    float dist = sqrtf(x + y);
    return dist;
}

Snake::Snake()
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
   // this->ConnectBody();
}

Snake::~Snake()
{
   // delete brain;
}


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
    int size = 4;
    for (int i = 0; i < size;i++) {
        if (max < decision(i,0)) {
            max = decision(i,0);
            maxIndex = i;
        }
    }
    if (maxIndex == Direction) {
        sameMoveCount += 1;
    }
    else {
        sameMoveCount = 0;
    }

    move(maxIndex);
}

void Snake::move(int dir)
{
   // Direction = dir;
    if (dir == 0 && Direction != 2) { 
        b[0].x -= 1; 
        vel.first = -1;
        vel.second = -0;
    }
    else if (dir == 1 && Direction != 3) {
        b[0].y -= 1; 
        vel.first = 0;
        vel.second = -1;
    }
    else if (dir == 2 && Direction != 0) {
        b[0].x += 1;
        vel.first = 1;
        vel.second = 0;
    }
    else if (dir == 3 && Direction != 1) {
        b[0].y += 1;
        vel.first = 0;
        vel.second = 1;
    }
    Direction = dir;
    pos.first = b[0].x; pos.second = b[0].y;
    bodySprite[0].setPosition(b[0].x * TEXTURE_SIZE, b[0].y * TEXTURE_SIZE);
    
    float newDistToApple = calcDistanceToApple();
    if (distanceToApple > newDistToApple) {
        movesScore += 400;
    }   
    if (sameMoveCount > 5) {
        movesScore /= 1.5;
    }

    distanceToApple = newDistToApple;
}

void Snake::updateMovement()
{
    this->lifeLeft -= 1;
    this->lifeTime += 1;
    if (this->lifeLeft <= 0) {
        penalty = 1;
        this->Dead = true;
    }
    else {
        this->ConnectBody();
        this->look();
        this->setVelocity();
        this->eat();       
        this->Collision();
    }
}

void Snake::update()
{
    this->updateMovement();
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
        movesScore += 500;
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
        if (b[0].x == b[i].x && b[0].y == b[i].y) {
           Dead = true;
            penalty = 0.9;
        }
    }
    //Collision with wall
    if (b[0].x > 30) { 
        Dead = true; 
        //b[0].x = 0;
        penalty = 0.2;


    }
    else if (b[0].x < 0) {
        Dead = true;
        //b[0].x = 30;
        penalty = 0.2;


    }
    else if (b[0].y > 20) {
        Dead = true; 
        //b[0].y = 0;
        penalty = 0.2;


    }
    else if (b[0].y < 0) {
        Dead = true; 
        //b[0].y = 20;
        penalty = 0.2;


    }
}

int Snake::calcFitness()
{
    //fitness = score*5000 + lifeTime*lifeTime + movesScore*penalty;
    /*if (fitness < 0)
        std::cout << "Negative \n";*/
    fitness = lifeTime*lifeTime*powf(2,size-3);
    return fitness;
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
            visionInDirection[0] = 1 / distance;
            //fitness += ((1 - (1 / distance))*1000);
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

void Snake::render(RenderTarget& target)
{
    for (int i = 0; i < size; i++) {
        target.draw(bodySprite[i]);
    }
    target.draw(foodSprite);
}


