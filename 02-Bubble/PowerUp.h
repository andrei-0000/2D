#ifndef _PowerUp_INCLUDE
#define _PowerUp_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// PowerUp is basically a Sprite that represents the PowerUp. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class PowerUp
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool haRecollit();

private:

	bool recollit;
	glm::ivec2 tileMapDispl, posPowerUp;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _PowerUp_INCLUDE
