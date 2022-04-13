#ifndef _MENUSCREEN_INCLUDE
#define _MENUSCREEN_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class MenuScreen
{

public:
	MenuScreen();
	~MenuScreen();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Quad* quad;
	Texture texs[3];
	TexturedQuad* texQuad[3];
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
	glm::mat4 projection;
	int puntuacio;

};


#endif // _MENUSCREEN_INCLUDE