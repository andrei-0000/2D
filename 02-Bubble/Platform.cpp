#include "Platform.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Keys.h"


enum PlatformAnims
{
	ANIM
};


void Platform::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/plataforma.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ANIM, 3);
	sprite->addKeyframe(ANIM, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ANIM, glm::vec2(0.33f, 0.f));
	sprite->addKeyframe(ANIM, glm::vec2(0.66f, 0.f));




	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));

}

void Platform::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));
}

void Platform::render()
{
	sprite->render();
}

void Platform::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Platform::setPosition(const glm::vec2& pos)
{
	posPlatform = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlatform.x), float(tileMapDispl.y + posPlatform.y)));
}


float Platform::getX()
{
	return posPlatform.x;
}

float Platform::getY()
{
	return posPlatform.y;
}