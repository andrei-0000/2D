#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>


void Game::init()
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
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
	if (key == 13) // Tecla Enter per a pasar al seg�ent nivell
		scene.nextMap(true);
	if (key == 115) // Tecla s (min�scula) per a reduir la velocitat del joc
		timeSlow = !timeSlow;
	if (key == 48) // Tecla n�mero 0
		scene.changeMap(0);
	if (key == 49) // Tecla n�mero 1
		scene.changeMap(1);
	if (key == 50) // Tecla n�mero 2
		scene.changeMap(2);
	if (key == 51) // Tecla n�mero 3
		scene.changeMap(3);
	if (key == 52) // Tecla n�mero 4
		scene.changeMap(4);
	if (key == 53) //Tecla n�mero 5
		scene.changeMap(5);
	if (key == 54) // Tecla n�mero 6
		scene.changeMap(6);
	if (key == 55) // Tecla n�mero 7
		scene.changeMap(7);
	if (key == 56) // Tecla n�mero 8
		scene.changeMap(8);
	if (key == 57) // Tecla n�mero 9
		scene.changeMap(9);
	if(key == 102){ //Tecla f (min�scula) per anar a la pantalla final
		scene.changeMap(10);
	}
	if (key == 103) { // Tecla g (min�scula) per a no poder morir amb els tiles de mort
		if (scene.getActiveDeath()) scene.setActiveDeath(false);
		else scene.setActiveDeath(true);
	}
	if(key == 100){ // Tecla d (mim�sucula) per a tenir dash infinit
		scene.infiniteDash();
	}

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





