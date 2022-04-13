#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>


void Game::init()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
	if (key == 13)
		scene.nextMap(true);
	if (key == 90)
		timeSlow = !timeSlow;
	if (key == 49) // numero 1
		scene.changeMap(1);
	//std::cout << "1" << endl;
	if (key == 50) // numero 2
		scene.changeMap(2);
	if (key == 51) // numero 3
		scene.changeMap(3);
	if (key == 52) // numero 4
		scene.changeMap(4);
	if (key == 53) // numero 5
		scene.changeMap(5);
	if (key == 54) // numero 6
		scene.changeMap(6);
	if (key == 55) // numero 7
		scene.changeMap(7);
	if (key == 56) // numero 8
		scene.changeMap(8);
	if (key == 57) // numero 9
		scene.changeMap(9);
	if (key == 58) // numero 10
		scene.changeMap(10);


}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





