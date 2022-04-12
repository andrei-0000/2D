#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <thread>
#include <chrono>
#include <Windows.h>
#include  "MMSystem.h"
#include <iostream>

#define SCREEN_X 0 // son els píxels que deixa a l'esquerra de la finestra de joc
#define SCREEN_Y 0 // son els píxels que deixa a dalt de la finestra de joc
#define COMPARE_DIFF 3.0
#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 11 //posicio del player al spawnejar


Scene::Scene()
{
	map = NULL;
	player = NULL;
	texQuad[0] = NULL; //background
	texQuad[1] = NULL; //middleground
	texQuad[2] = NULL; //foreground
	powerUp = NULL;
	objectpunts = NULL;
	platform = NULL;
	final = NULL;

}


Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (objectpunts != NULL)
		delete objectpunts;
	if (powerUp != NULL)
		delete powerUp;
	if (platform != NULL)
		delete platform;
	if (final != NULL)
		delete final;
	for (int i = 0; i < 3; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}

void Scene::init()
{
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };


	initShaders();

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(4.f, 4.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //background
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //middleground
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);  //foreground

	//Posar diferents nivells
	maps.push_back(TileMap::createTileMap("levels/mapa11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	maps.push_back(TileMap::createTileMap("levels/mapa022.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	maps.push_back(TileMap::createTileMap("levels/mapa033.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	maps.push_back(TileMap::createTileMap("levels/mapa044.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	currentMap = 0; //començar per el nivell 0
	map = maps[currentMap];

	//Inicialització de objectes/juadors
	player = new Player();
	powerUp = new PowerUp();
	objectpunts = new ObjectPunts();
	platform = new Platform();
	final = new FinalObject();

	//Inicialització de player
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setTileMap(map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	//Inicialització d'objecte que recarrega dash
	powerUp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	powerUp->setTileMap(map);

	//Inicialització d'objectes de puntuació
	objectpunts->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	objectpunts->setTileMap(map);

	//Inicialització de plataforma
	platform->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	platform->setTileMap(map);
	platform->setPosition(glm::vec2(13 * map->getTileSize(), 11 * map->getTileSize()));

	//Inicialització de objecte final
	final->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	final->setTileMap(map);
	final->setPosition(glm::vec2(14 * map->getTileSize(), 0 * map->getTileSize()));



	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	//Carreguem textures de background/middleground
	texs[0].loadFromFile("images/skyline-b.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura background
	texs[1].loadFromFile("images/buildings.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura middleground
	texs[2].loadFromFile("images/near.png", TEXTURE_PIXEL_FORMAT_RGBA);  //textura middleground

	currentTime = 0.0f;
}
	

//Funció de comparació
bool cmpf(float A, float B, float epsilon = 28.0f)
{
	if (A > B) return (fabs(A - B) < epsilon);
	else return (fabs(B - A) < epsilon);
}

void Scene::update(int deltaTime)
{
	
	currentTime += deltaTime;
	//Fem update objectes/jugadors
	player->update(deltaTime);
	objectpunts->update(deltaTime);
	powerUp->update(deltaTime);
	platform->update(deltaTime);
	final->update(deltaTime);

	//Agafar objecte de puntuació
	if (cmpf(player->getX(), objectpunts->getX()) && cmpf(player->getY(), objectpunts->getY()))
	{
		bool played = PlaySound(TEXT("sounds/coin.wav"), NULL, SND_ASYNC);
		objectpunts->setPosition(glm::vec2(-1 * map->getTileSize(), 0 * map->getTileSize()));
	}


	//Si el jugador mor
	if (player->isDead()) {
		//bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
		//player->changetoDeadAnim();
		nextMap(false);
		player->changeDeathStatus(false);
	}

	//Agafar objecte que recarrega dash
	if (cmpf(player->getX(), powerUp->getX()) && cmpf(player->getY(), powerUp->getY()))
	{
		player->setDash();
		powerUp->setPosition(glm::vec2(-1 * map->getTileSize(), 0 * map->getTileSize()));
	}
	//if (player->isDead()) changeMap(currentMap);

	//Saltar plataforma
	if (cmpf(player->getX(), platform->getX()) && cmpf((player->getY() + 5), (platform->getY()))) {
		player->setPlatform(true);
		std::cout << "hola" << endl;
	}
	else player->setPlatform(false);
	
	if (cmpf(player->getX(), final->getX()) && cmpf(player->getY(), final->getY())) {
		nextMap(true);
	}
}


//Renderitzar
void Scene::render()
{
	glm::mat4 modelview;

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

	/*modelview = glm::translate(modelview, glm::vec3(7000 * (sin(-currentTime / 500)), 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);*/

	//Render mapa
	map->render();
	//Render jugador
	player->render();
	//Render objectes
	powerUp->render();
	objectpunts->render();
	platform->render();
	final->render();

}

//Inicialitzar shaders
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

void Scene::nextMap(bool next)
{
	if (next) ++currentMap;
	if (currentMap >= maps.size()) currentMap = 0;
	map = maps[currentMap];

	//Es pasa el mapa als objectes/jugadors
	player->setTileMap(map);
	powerUp->setTileMap(map);
	objectpunts->setTileMap(map);
	platform->setTileMap(map);
	final->setTileMap(map);
	player->setDash();

	player->setReappearing();
	objectpunts->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	powerUp->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	player->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
	//platform->setPosition(glm::vec2(13 * map->getTileSize(), 11 * map->getTileSize()));
	//final->setPosition(glm::vec2(14 * map->getTileSize(), 11 * map->getTileSize()));

	//Canviar entre diferents mapes
	switch (currentMap) {
	case 0:
		player->setPosition(glm::vec2(1 * map->getTileSize(), 11 * map->getTileSize()));
		break;
	case 1:
		player->setPosition(glm::vec2(8 * map->getTileSize(), 11 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(10 * map->getTileSize(), 11 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(4 * map->getTileSize(), 11 * map->getTileSize()));
		final->changeSprite(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
		final->setPosition(glm::vec2(15 * map->getTileSize(), 11 * map->getTileSize()));
		break;
	case 2:
		player->setPosition(glm::vec2(1 * map->getTileSize(), 6 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(-1 * map->getTileSize(), 11 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(9 * map->getTileSize(), 5 * map->getTileSize()));
		final->setPosition(glm::vec2(19 * map->getTileSize(), 4 * map->getTileSize()));
		break;
	case 3:
		player->setPosition(glm::vec2(0 * map->getTileSize(), 11 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(14 * map->getTileSize(), 4 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(0 * map->getTileSize(), 1 * map->getTileSize()));
		final->setPosition(glm::vec2(19 * map->getTileSize(), 5 * map->getTileSize()));
		platform->setPosition(glm::vec2(6 * map->getTileSize(), 4 * map->getTileSize()));
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


	//objectpunts->setPosition(glm::vec2(1 * map->getTileSize(), 1 * map->getTileSize()));
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
	case 2:
		player->setPosition(glm::vec2(1 * map->getTileSize(), 6 * map->getTileSize()));
		powerUp->setPosition(glm::vec2(-1 * map->getTileSize(), 11 * map->getTileSize()));
		objectpunts->setPosition(glm::vec2(5 * map->getTileSize(), 9 * map->getTileSize()));
		break;
	}
}

int Scene::getCurrentMap() {
	return currentMap;
}



