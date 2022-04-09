#ifndef _ObjectPUNTS_INCLUDE
#define _ObjectPUNTS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// ObjectPunts is basically a Sprite that represents the ObjectPunts. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class ObjectPunts
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool haRecollit();

	float getX();
	float getY();

private:

	bool recollit;
	glm::ivec2 tileMapDispl, posObjectPunts;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _ObjectPunts_INCLUDE


