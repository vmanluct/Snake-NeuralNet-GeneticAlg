#include <SFML/Graphics.hpp>
#include "Snake.h"

void Snake::initVariables()
{
    //Starting position for snake and food
    int xStart = 29;//rand() % 25; 
    int yStart = 19;//rand() % 15;
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

    this->penalty = 1; //Help with calculating fitness
    this->distanceToApple = calcDistanceToApple();
    this->lifeLeft = 50; //How many moves snake starts with
    this->lifeTime = 0; //How long snake has been alive in frames
    this->Direction = 1;//rand() % 4; //Direction snake is moving
    this->fitness = 0; //How well snake did
    this->size = 4; //How many body parts snake has
    this->score = 0; //How many apples snake ate
    this->Dead = false; //If snake is alive
    this->brain = NeuralNet(11, 10, 3); //Neural network for snake
    this->ConnectBody(); //Connect all the body pieces when created
    
}


void Snake::initSprite()
{
    //Set color of body and food to the same color
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
    //Euclidean distance to food
    float x = powf((pos.first - foodPos.first), 2);
    float y = powf((pos.second - foodPos.second), 2);
    float dist = sqrtf(x + y);
    return dist;
}

Snake::Snake()
{
    //Initialize everything for snake
    this->initVariables();
    this->initSprite();
}

Snake::~Snake()
{

}


void Snake::mutate(float mr)
{
    //Mutate snake dna
    brain.mutate(mr);
}

void Snake::move(int dir)
{

    //Left
    if (dir == 0) {
        b[0].x -= 1;
        vel.first = -1;
        vel.second = -0;
    }
    //Up
    else if (dir == 1) {
        b[0].y -= 1;
        vel.first = 0;
        vel.second = -1;
    }
    //Right
    else if (dir == 2) {
        b[0].x += 1;
        vel.first = 1;
        vel.second = 0;
    }
    //Down
    else if (dir == 3) {
        b[0].y += 1;
        vel.first = 0;
        vel.second = 1;
    }

    //Change what direction snake is moving
    Direction = dir;
    
    //Adjust position of snakes head
    pos.first = b[0].x; pos.second = b[0].y;
    bodySprite[0].setPosition(b[0].x * TEXTURE_SIZE, b[0].y * TEXTURE_SIZE);

    //How far head is from apple
    float newDistToApple = calcDistanceToApple();
    distanceToApple = newDistToApple;
}

void Snake::updateMovement()
{
    this->lifeLeft -= 1; //Adjust how many steps snake has left
    this->lifeTime += 1; //Adjust how long snake has been alive

    //Check if snake has used all steps
    if (this->lifeLeft <= 0) {
        this->Dead = true;
    }

    //Update everything about the snake
    else {
        this->ConnectBody(); //Connect all body
        this->think(); //Check everything snake can see
        this->makeMove(); //Decide what to do based on neural network
        this->eat(); //Eat food if overlapping
        this->Collision(); //Check if head is colliding
    }
}

void Snake::update()
{
    //Update everything about the snake
    this->updateMovement();
}


void Snake::eat()
{
    //If snakes head overlaps food eat
    if (b[0].x == foodPos.first && b[0].y == foodPos.second) {
        size++; //increase size of snake

        //Position food at new position in window
        foodPos.first = rand() % 30; 
        foodPos.second = rand() % 20;
        foodSprite.setPosition(foodPos.first*TEXTURE_SIZE, foodPos.second*TEXTURE_SIZE);
        
        //Add new body piece
        b[size].x = b[size - 1].x; b[size].y = b[size - 1].y;
        
        //Increase score, and add more moves
        score++;
        lifeLeft += 100;
    }
}

void Snake::ConnectBody()
{
    //Connect all body pieces together
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
    for (int i = 1; i < size; i++) {
        if (b[0].x == b[i].x && b[0].y == b[i].y) {
           Dead = true;
           penalty = 0.8; //If collision with body lower score by 0.8
        }
    }
    //Collision with wall
    if (b[0].x > 29) { 
        Dead = true; 
    }
    else if (b[0].x < 0) {
        Dead = true;
    }
    else if (b[0].y > 19) {
        Dead = true; 
    }
    else if (b[0].y < 0) {
        Dead = true;
    }
}

float Snake::calcFitness()
{
    //See how well snake is doing
    fitness += (lifeTime + powf(2,powf(2,score)))*penalty;
    return fitness;
}

Snake Snake::crossover(Snake Partner)
{
    //Create new snake with brain that is a mix of both
    Snake child = Snake();
    child.brain = brain.crossover(Partner.brain);

    return child;
}

Snake Snake::clone()
{
    //Create a deep copy of the snakes brain
    Snake clone = Snake();
    clone.brain = brain.clone();
    clone.Dead = false;
    return clone;
}

bool Snake::isOnTail(int x, int y)
{
    //Check if head is overlapping any body part
    for (int i = 2; i < size; i++) {
        if (x == b[i].x && y == b[i].y) return true;
    }
    return false;
}

void Snake::render(RenderTarget& target)
{
    //Draw all body parts to the screen
    for (int i = 0; i < size; i++) {
        target.draw(bodySprite[i]);
    }
    //Draw the food to the screen
    target.draw(foodSprite);
}

int Snake::dangerStraight()
{
    //Initialize hypothetical position of snakes head
    std::pair<int, int> position = { 0, 0 };
    position.first = pos.first;
    position.second = pos.second;

    //Check in moving all directions if there is an object in front of the snakes head
    if (this->Direction == 0) {
        if (position.first - 1 < 0 || this->isOnTail(position.first - 1, position.second)) {
            return 1;
        }
    }
    else if (this->Direction == 1) {
        if (position.second - 1 < 0 || this->isOnTail(position.first, position.second - 1)) {
            return 1;
        }
    }
    else if (this->Direction == 2) {
        if (position.first + 1 > 29 || this->isOnTail(position.first + 1, position.second)) {
            return 1;
        }
    }
    else if (this->Direction == 3) {
        if (position.second + 1 > 19 || this->isOnTail(position.first, position.second + 1)) {
            return 1;
        }
    }

    return 0;
}

int Snake::dangerLeft()
{
    //Initialize hypothetical position of snakes head
    std::pair<int, int> position = { 0, 0 };
    position.first = pos.first;
    position.second = pos.second;

    //Check in moving all directions if there is an object to the left of the snakes head
    if (this->Direction == 0) {
        if (position.second + 1 > 19 || this->isOnTail(position.first, position.second + 1)) {
            return 1;
        }
    }
    else if (this->Direction == 1) {
        if (position.first - 1 < 0 || this->isOnTail(position.first - 1, position.second)) {
            return 1;
        }
    }
    else if (this->Direction == 2) {
        if (position.second - 1 < 0 || this->isOnTail(position.first, position.second - 1)) {
            return 1;
        }
    }
    else if (this->Direction == 3) {
        if (position.first + 1 > 29 || this->isOnTail(position.first + 1, position.second)) {
            return 1;
        }
    }

    return 0;
}

int Snake::dangerRight()
{
    //Initialize hypothetical position of snakes head
    std::pair<int, int> position = { 0, 0 };
    position.first = pos.first;
    position.second = pos.second;

    //Check in all moving directions if there is an object to the right of the snakes head
    if (this->Direction == 0) {
        if (position.second - 1 < 0 || this->isOnTail(position.first, position.second - 1)) {
            return 1;
        }
    }
    else if (this->Direction == 1) {
        if (position.first + 1 > 29 || this->isOnTail(position.first + 1, position.second)) {
            return 1;
        }
    }
    else if (this->Direction == 2) {
        if (position.second + 1 > 19 || this->isOnTail(position.first, position.second + 1)) {
            return 1;
        }
    }
    else if (this->Direction == 3) {
        if (position.first - 1 < 0 || this->isOnTail(position.first - 1, position.second)) {
            return 1;
        }
    }

    return 0;
}

int Snake::foodLeft()
{
    //Check if the food is to the left of the snakes head
    if (pos.first > foodPos.first) {
        return 1;
    }
    else {
        return 0;
    }
}

int Snake::foodUp()
{
    //Check if food is above snakes head
    if (pos.second > foodPos.second) {
        return 1;
    }
    else {
        return 0;
    }
}

int Snake::foodRight()
{
    //Check if food is to the right of the snakes head
    if (pos.first < foodPos.first) {
        return 1;
    }
    else {
        return 0;
    }
}

int Snake::foodDown()
{
    //Check if food is below snakes head
    if (pos.second < foodPos.second) {
        return 1;
    }
    else {
        return 0;
    }
}

void Snake::moveStraight()
{
    //Continue moving forward
    this->move(this->Direction);
}

void Snake::moveRight()
{
    //Move right in respect to the direction snake is moving
    if (this->Direction == 0) {
        this->move(1);
    }
    else if (this->Direction == 1) {
        this->move(2);
    }
    else if (this->Direction == 2) {
        this->move(3);
    }
    else if (this->Direction == 3) {
        this->move(0);
    }
}

void Snake::moveLeft()
{
    //Move left in respect to the direction the snake is moving
    if (this->Direction == 0) {
        this->move(3);
    }
    else if (this->Direction == 1) {
        this->move(0);
    }
    else if (this->Direction == 2) {
        this->move(1);
    }
    else if (this->Direction == 3) {
        this->move(2);
    }
}

int Snake::movingLeft()
{
    //Check if snake is moving left
    if (this->Direction == 0) {
        return 1;
    }
    else
        return 0;
}

int Snake::movingUp()
{
    //Check if snake is moving up
    if (this->Direction == 1) {
        return 1;
    }
    else
        return 0;
}

int Snake::movingRight()
{
    //Check if snake is moving right
    if (this->Direction == 2) {
        return 1;
    }
    else
        return 0;
}

int Snake::movingDown()
{
    //Check if snake is moving down
    if (this->Direction == 3) {
        return 1;
    }
    else
        return 0;
}

void Snake::think()
{
    //Check for danger around the head of the snake
    this->vision[0] = dangerStraight();
    this->vision[1] = dangerLeft();
    this->vision[2] = dangerRight();

    //Check what direction snake is moving, only one of these will be 1
    this->vision[3] = movingLeft();
    this->vision[4] = movingUp();
    this->vision[5] = movingRight();
    this->vision[6] = movingDown();

    //Check what directions the apple is in
    this->vision[7] = foodLeft();
    this->vision[8] = foodUp();
    this->vision[9] = foodRight();
    this->vision[10] = foodDown();
}

void Snake::makeMove()
{
    this->decision = brain.output(vision); //Calculate value of all moves

    float max = 0;
    int maxIndex = 0;

    //Go through all moves and see which is the best
    for (int i = 0; i < 3; i++) {
        if (max < decision(i, 0)) {
            max = decision(i,0);
            maxIndex = i;
        }
    }

    //Decide whether to move forward, left, or right
    if (maxIndex == 0) {
        this->moveStraight();
    }
    else if (maxIndex == 1) {
        this->moveRight();
    }
    else {
        this->moveLeft();
    }
}


