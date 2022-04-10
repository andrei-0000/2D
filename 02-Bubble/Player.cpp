#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Keys.h"
#include <chrono>
#include <thread>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/Player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2, 0.16), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.2f, 0.33f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.33f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(DIE_LEFT, 20);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.6f,0.83f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.4f,0.83f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.2f,0.83f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.f, 0.83f));

	sprite->setAnimationSpeed(DIE_RIGHT, 20);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.f, 0.66f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.2f, 0.66f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.4f, 0.66f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.6f, 0.66f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	//if(dead) sprite->update(deltaTime+10000);
	//else sprite->update(deltaTime);
	sprite->update(deltaTime);


	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if (map->collisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
			if (map->deathCollisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x += 2;
				sprite->changeAnimation(DIE_LEFT);
				//sprite->update(deltaTime + 1000);
				dead = true;
				
			}
			else {
				if (bJumping) {
					stickied = true;
					bJumping = false;
				}
			}
			
		}
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			if (map->deathCollisionMoveLeft(posPlayer, glm::vec2(32, 32))) {
				posPlayer.x += 2;
				sprite->changeAnimation(DIE_LEFT);
				dead = true;
			}
			else {
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
			
		}

	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if (map->collisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
			if (map->deathCollisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x -= 2;
				sprite->changeAnimation(DIE_RIGHT);
				dead = true;
			}
			else {
				if (bJumping) {
					stickied = true;
					bJumping = false;
				}
			}
		}
			
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			if (map->deathCollisionMoveRight(posPlayer, glm::vec2(32, 32))) {
				dead = true;
				posPlayer.x -= 2;
				sprite->changeAnimation(DIE_RIGHT);
			}
			else {
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);

			}
			
		}
	}

	//DASH
	else if (Game::instance().getKey(Keys::Keys::D))
	{
		if (sprite->animation() == STAND_RIGHT && can_dash)
			posPlayer.x += 6;
		else if (sprite->animation() == STAND_LEFT && can_dash)
			posPlayer.x -= 6;
		if (map->collisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
			if (bJumping) {
				stickied = true;
				bJumping = false;
			}
		
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x -= 6;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (map->collisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
			if (bJumping) {
				stickied = true;
				bJumping = false;
			}

			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
			{
				if (bJumping) {
					stickied = true;
					bJumping = false;
				}
				posPlayer.x += 6;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}

	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		if (!stickied) jumpAngle += JUMP_ANGLE_STEP;
		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
			bJumping = false;
			posPlayer.y = startY;
		}
		 if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
			if (!stickied) posPlayer.y += FALL_STEP;
			else posPlayer.y += FALL_STEP/4;
			if (map->deathCollisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(DIE_RIGHT);
				dead = true;
			}
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || stickied) 
			{
					if (Game::instance().getKey(Keys::Keys::Space))
					{
						bJumping = true;
						jumpAngle = 0;
						posPlayer.y -= 2;
						startY = posPlayer.y;
					}
					stickied = false;
			}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

float Player::getX()
{
	return posPlayer.x;
}

float Player::getY()
{
	return posPlayer.y;
}

bool Player::isDead()
{
	return dead;
}

void Player::changeAnim() {
	sprite->changeAnimation(STAND_RIGHT);
}


void Player::changeDeathStatus(bool b)
{
	dead = b;
}

bool canDash() {
	return canDash;
}
