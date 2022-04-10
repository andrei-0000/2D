#include "ObjectPunts.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Keys.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum ObjectPuntsAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void ObjectPunts::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/gem.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObjectPunts.x), float(tileMapDispl.y + posObjectPunts.y)));

}

void ObjectPunts::update(int deltaTime)
{
	sprite->update(deltaTime);
	float time = glutGet(GLUT_ELAPSED_TIME);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObjectPunts.x), float((tileMapDispl.y + posObjectPunts.y))));
}

void ObjectPunts::render()
{
	sprite->render();
}

void ObjectPunts::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void ObjectPunts::setPosition(const glm::vec2& pos)
{
	posObjectPunts = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObjectPunts.x), float(tileMapDispl.y + posObjectPunts.y)));
}

bool ObjectPunts::haRecollit() {
	return recollit;
}

float ObjectPunts::getX()
{
	return posObjectPunts.x;
}

float ObjectPunts::getY()
{
	return posObjectPunts.y;
}
