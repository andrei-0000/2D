#ifndef _Platform_INCLUDE
#define _Platform_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Platform
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);  
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);


	float getX();
	float getY();

private:

	glm::ivec2 tileMapDispl, posPlatform;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};



#endif // _PLATFORM_INCLUDE
