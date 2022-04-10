#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "ObjectPunts.h"
#include "PowerUp.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void nextMap();
	void changeMap(int i);
	int getCurrentMap();

private:
	void initShaders();

private:
	Quad* quad;
	Texture texs[1];
	TexturedQuad* texQuad[1];
	TileMap *map;
	vector<TileMap *> maps;
	int currentMap;
	Player *player;
	ObjectPunts* objectpunts;
	PowerUp* powerUp;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

