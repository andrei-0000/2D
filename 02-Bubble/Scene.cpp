#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0 // son els p�xels que deixa a l'esquerra de la finestra de joc
#define SCREEN_Y 0 // son els p�xels que deixa a dalt de la finestra de joc
#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 11 //posicio del player al spawnejar


Scene::Scene()
{
	map = NULL;
	player = NULL;

}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	maps.push_back(TileMap::createTileMap("levels/mapa11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	maps.push_back(TileMap::createTileMap("levels/mapa022.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	currentMap = 0;
	map = maps[currentMap];

	player = new Player();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);



	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);

}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::nextMap()
{
	++currentMap;
	if (currentMap >= maps.size()) currentMap = 0;
	map = maps[currentMap];
	player->setTileMap(map);
	switch (currentMap) {
	case 0:
		player->setPosition(glm::vec2(1 * map->getTileSize(), 11 * map->getTileSize()));
	case 1:
		player->setPosition(glm::vec2(8 * map->getTileSize(), 11 * map->getTileSize()));
	}
}

int Scene::getCurrentMap() {
	return currentMap;
}



