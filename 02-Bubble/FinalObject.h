#ifndef _FinalObject_INCLUDE
#define _FinalObject_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class FinalObject
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);


	float getX();
	float getY();
	bool getVertical();
	void setVertical(bool a);
	void changeSprite(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool vertical);
private:

	glm::ivec2 tileMapDispl, posFinal;
	
	//bool vertical;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};



#endif // _FINAL_OBJECT_INCLUDE
