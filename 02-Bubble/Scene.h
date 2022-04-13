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
#include "Platform.h"
#include "FinalObject.h"
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

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
	void nextMap(bool next);
	void changeMap(int i);
	int getCurrentMap();
	void setActiveDeath(bool a);
	bool getActiveDeath();
	void infiniteDash();

private:
	void initShaders();

private:
	Quad* quad;
	Texture texs[3];
	TexturedQuad* texQuad[3];
	TileMap *map;
	vector<TileMap *> maps;
	int currentMap;
	Player *player;
	ObjectPunts* objectpunts;
	PowerUp* powerUp;
	Platform* platform;
	FinalObject* final;
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	int puntuacio;
	bool recollit_puntuacio;
	bool finalPos = false;

};


#endif // _SCENE_INCLUDE

