#include "FinalObject.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Keys.h"





void FinalObject::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/final2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinal.x), float(tileMapDispl.y + posFinal.y)));

}

void FinalObject::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinal.x), float(tileMapDispl.y + posFinal.y)));
}

void FinalObject::render()
{
	sprite->render();
}

void FinalObject::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void FinalObject::setPosition(const glm::vec2& pos)
{
	posFinal = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFinal.x), float(tileMapDispl.y + posFinal.y)));
}


float FinalObject::getX()
{
	return posFinal.x;
}

float FinalObject::getY()
{
	return posFinal.y;
}
/*
bool FinalObject::getVertical() {
	return vertical;
}

void FinalObject::setVertical(bool a) {
	vertical = a;
}*/

void FinalObject::changeSprite(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool vertical) {
	if (vertical) {
		spritesheet.loadFromFile("images/final1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	}
	else {
		spritesheet.loadFromFile("images/final2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	}
}