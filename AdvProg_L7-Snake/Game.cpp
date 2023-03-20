// UPDATE THIS FILE

#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Game.h"

using namespace std;

Game::Game(int _width, int _height)
    : width(_width), height(_height), // play screen 
	  squares(_height, vector<CellType>(_width, CELL_EMPTY)), // cell coordinates
      snake(*this, Position(_width/2, _height/2)),  // init snake positin in middle of play screen
      currentDirection(Direction::RIGHT),
      status(GAME_RUNNING),
      score(0)
{
	addCherry();
}

Game::~Game() {}


void Game::snakeMoveTo(Position pos) {
    switch (getCellType(pos)) {
    case CELL_OFF_BOARD:
        setGameStatus(GAME_OVER);
        break;
    case CELL_SNAKE:
        setGameStatus(GAME_OVER);
        break;
    case CELL_CHERRY:
        score++;
        snake.eatCherry();
        addCherry();
        break;
    case CELL_EMPTY:
        setCellType(pos, CELL_SNAKE);
    }
}

void Game::snakeLeave(Position position) {
    setCellType(position, CELL_EMPTY);
}


void Game::processUserInput(Direction direction) {
    inputQueue.push(direction);
}


bool Game::canChange(Direction current, Direction next) const {
	if (current == UP || current == DOWN) 
        return next != UP && next != DOWN;
    return next != LEFT && next != RIGHT;
}


void Game::nextStep() {
	while (!inputQueue.empty()) {
        Direction next = inputQueue.front();
        inputQueue.pop();

        if (canChange(currentDirection, next)) {
            currentDirection = next;
        	break;
		}
    }

    snake.move(currentDirection);
}


void Game::addCherry()
{
    do {
        Position randomPos(rand()%width, rand()%height);
		
        if (getCellType(randomPos) == CELL_EMPTY) {

            setCherryPosition(randomPos);
            setCellType(randomPos, CELL_CHERRY);

       		break;
        }
    } while (true);
}

void Game::setCellType(Position pos, CellType cellType) 
{
    if (pos.isInsideBox(0, 0, width, height)) squares[pos.y][pos.x] = cellType;
}



CellType Game::getCellType(Position pos) const {
	return pos.isInsideBox(0, 0, width, height) ? squares[pos.y][pos.x] : CELL_OFF_BOARD;
}

vector<Position> Game::getSnakePositions() const
{
    return snake.getPositions();
}

GameStatus Game::getGameStatus(){
	return status;
}

int Game::getWidth(){
	return width;
}

int Game::getHeight(){
	return height;
}

Snake Game::getSnake(){
	return snake;
}

