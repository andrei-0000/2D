#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0 // son els píxels que deixa a l'esquerra de la finestra de joc
#define SCREEN_Y 0 // son els píxels que deixa a dalt de la finestra de joc
#define COMPARE_DIFF 3.0
#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 11 //posicio del player al spawnejar


Scene::Scene()
{
	map = NULL;
	player = NULL;
	texQuad[0] = NULL;
	powerUp = NULL;
	objectpunts = NULL;

}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (objectpunts != NULL)
		delete objectpunts;
	if (powerUp != NULL)
		delete powerUp;
	for (int i = 0; i < 1; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };


	initShaders();

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(4.f, 4.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //suelo

	maps.push_back(TileMap::createTileMap("levels/mapa11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	maps.push_back(TileMap::createTileMap("levels/mapa022.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	currentMap = 0;
	map = maps[currentMap];

	player = new Player();
	powerUp = new PowerUp();
	objectpunts = new ObjectPunts();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setTileMap(map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	powerUp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	powerUp->setTileMap(map);

	objectpunts->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	objectpunts->setTileMap(map);

	texs[0].loadFromFile("images/skyline-b.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura champi
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

bool cmpf(float A, float B, float epsilon = 28.0f)
{
	if (A > B) return (fabs(A - B) < epsilon);
	else return (fabs(B - A) < epsilon);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	objectpunts->update(deltaTime);
	powerUp->update(deltaTime);
	if (cmpf(player->getX(),objectpunts->getX()) && cmpf(player->getY(),objectpunts->getY()))
	{
		objectpunts->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	}
	if (player->isDead()) changeMap(currentMap);
}

void Scene::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(260.f, 560.f, 0.f));
	modelview = glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);
	
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	player->render();
	powerUp->render();
	objectpunts->render();

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
	powerUp->setTileMap(map);
	objectpunts->setTileMap(map);


	objectpunts->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	powerUp->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	player->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));

	switch (currentMap) {
	case 0:
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		break;
	case 1:
		player->setPosition(glm::vec2(8 * map->getTileSize(), 11 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(10 * map->getTileSize(), 11 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(4 * map->getTileSize(), 11 * map->getTileSize()));
		break;
	}
}

void Scene::changeMap(int i)
{
	++currentMap;
	if (i >= maps.size() || i < 0) currentMap = 0;
	map = maps[currentMap];
	player->setTileMap(map);
	powerUp->setTileMap(map);
	objectpunts->setTileMap(map);


	objectpunts->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	powerUp->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	player->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));

	switch (currentMap) {
	case 0:
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		break;
	case 1:
		player->setPosition(glm::vec2(8 * map->getTileSize(), 11 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(10 * map->getTileSize(), 11 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(4 * map->getTileSize(), 11 * map->getTileSize()));
		break;
	}
}

int Scene::getCurrentMap() {
	return currentMap;
}



