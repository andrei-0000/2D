#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MenuScreen.h"
#include "Game.h"
#include <thread>
#include <chrono>
#include <Windows.h>
#include  "MMSystem.h"
#include <iostream>

#define SCREEN_X 0 // son els píxels que deixa a l'esquerra de la finestra de joc
#define SCREEN_Y 0 // son els píxels que deixa a dalt de la finestra de joc
#define COMPARE_DIFF 3.0


MenuScreen::MenuScreen()
{
	texQuad[0] = NULL; //background
	texQuad[1] = NULL; //middleground
	texQuad[2] = NULL; //foreground
	puntuacio = NULL;

}


MenuScreen::~MenuScreen()
{
	for (int i = 0; i < 3; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}

void MenuScreen::init()
{
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };


	initShaders();

	/*puntuacio = 100;

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(4.f, 4.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //background
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //middleground
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //foreground

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	//Carreguem textures de background/middleground
	texs[0].loadFromFile("images/skyline-b.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura background
	texs[1].loadFromFile("images/buildings.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura middleground
	texs[2].loadFromFile("images/near.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura middleground*/

	

	currentTime = 0.0f;
}


//Funció de comparació

void MenuScreen::update(int deltaTime)
{

	currentTime += deltaTime;
	//Fem update objectes/jugadors

}


//Renderitzar
void MenuScreen::render()
{
	glm::mat4 modelview;

	/*simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);



	//Aqui pot estar el problema
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);


	modelview = glm::mat4(1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(260.f, 190.f, 0.f));
	modelview = glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);

	modelview = glm::mat4(1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(260.f, 490.f, 0.f));
	modelview = glm::scale(glm::mat4(1.0f), glm::vec3(11.f, 14.f, 1.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[1]);

	modelview = glm::mat4(1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 260.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(17.f, 17.f, 1.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[2]);



	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	modelview = glm::translate(modelview, glm::vec3(7000 * (sin(-currentTime / 500)), 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);*/

	//Render mapa
	//Render jugador



}

//Inicialitzar shaders
void MenuScreen::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}




