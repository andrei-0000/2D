#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Keys.h"
#include <chrono>
#include <thread>
#include <Windows.h>
#include  "MMSystem.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define PLATFORM_HEIGHT 130
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT,DASH_LEFT,DASH_RIGHT, JUMP_LEFT, JUMP_RIGHT
};


//Inicialització del jugador
void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;

	//Carregar textura
	spritesheet.loadFromFile("images/Player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.2, 0.111), &spritesheet, &shaderProgram);
	//Nombre d'animacions
	sprite->setNumberAnimations(10);

	//Animació de estar dret mirant cap a l'esquerra
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.2f, 0.222f));

	//Animació de estar dret mirant cap a la dreta
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.222f));

	//Animació de moure's cap a l'esquerra
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.333f));

	//Animació de moure's cap a la dreta
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.f));


	//Animació de morir mirant cap a l'esquerra
	sprite->setAnimationSpeed(DIE_LEFT, 20);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.6f,0.555f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.4f,0.555f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.2f,0.555f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.f, 0.555f));


	//Animació de morir mirant cap a la dreta
	sprite->setAnimationSpeed(DIE_RIGHT, 20);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.f, 0.444f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.2f, 0.444f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.4f, 0.444F));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.6f, 0.444f));


	//Animacíó de dash mirant cap a la dreta
	sprite->setAnimationSpeed(DASH_RIGHT, 20);
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.8f, 0.875f));
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.6f, 0.875f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.4f, 0.875f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.2f, 0.875f));


	//Animació de dash mirant cap a la esquerra
	sprite->setAnimationSpeed(DASH_LEFT, 20);
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.8f, 0.777f));
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.6f, 0.777f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.4f, 0.777f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.2f, 0.777f));


	//Animació de saltar mirant cap a l'esquerra
	sprite->setAnimationSpeed(JUMP_LEFT, 20);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.6f, 0.666f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.4f, 0.666f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.2f, 0.666f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f, 0.666f));


	//Animació de saltar mirant cap a la dreta
	sprite->setAnimationSpeed(JUMP_RIGHT, 20);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f, 0.111f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.2f, 0.111f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.4f, 0.111f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.111f));

	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	//if(dead) sprite->update(deltaTime+10000);
	//else sprite->update(deltaTime);
	sprite->update(deltaTime);

	if (dying) {
			sprite->changeAnimation(DIE_RIGHT);
		die();
	}

	else if (reappearing) {
		reappear();
	}

	else if (dashing) {
		dash();
	}


	else if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		setDash();
		if (map->collisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
			if (map->deathCollisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x += 2;
				bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
				sprite->changeAnimation(DIE_LEFT);
				dying = true;
				
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
				bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
				sprite->changeAnimation(DIE_LEFT);
				dying = true;
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
		setDash();
		if (map->collisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
			if (map->deathCollisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
				posPlayer.x -= 2;
				bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
				sprite->changeAnimation(DIE_RIGHT);
				dying = true;
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
				posPlayer.x -= 2;
				bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
				sprite->changeAnimation(DIE_RIGHT);
				dying = true;
			}
			else {
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);

			}
			
		}
	}

	//DASH
	else  if (Game::instance().getKey(Keys::Keys::D))
	{
		if (sprite->animation() == STAND_RIGHT && can_dash && !dashing) {
			dashing = true;
			dash_direction = "right";
			dash();
		}
		else if (sprite->animation() == STAND_LEFT && can_dash && !dashing) {
			dashing = true;
			dash_direction = "left";
			dash();
		}
	}

	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	if (platformJumping) {
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - PLATFORM_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	
	if(bJumping)
	{
		if (!stickied) jumpAngle += JUMP_ANGLE_STEP;
		else  if (Game::instance().getKey(Keys::Keys::D)) //dash while jumping
		{
			if (sprite->animation() == STAND_RIGHT && can_dash && !dashing) {
				dashing = true;
				dash_direction = "right";
				dash();
			}
			else if (sprite->animation() == STAND_LEFT && can_dash && !dashing) {
				dashing = true;
				dash_direction = "left";
				dash();
			}
		}
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
				bool played1 = PlaySound(TEXT("sounds/death.wav"), NULL, SND_ASYNC);
				sprite->changeAnimation(DIE_RIGHT);
				dying = true;
			}
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || stickied) 
			{
					if (Game::instance().getKey(Keys::Keys::Space))
					{
						bJumping = true;
						jumpAngle = 0;
						posPlayer.y -= 2;
						startY = posPlayer.y;
						//if(sprite->animation() == (STAND_LEFT || MOVE_LEFT)) sprite->changeAnimation(JUMP_LEFT);
						//else if (sprite->animation() == (STAND_RIGHT || MOVE_RIGHT)) sprite->changeAnimation(JUMP_RIGHT);
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

void Player::changetoDeadAnim() {
	if(sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(DIE_RIGHT);
	else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(DIE_LEFT);
}

void Player::setDash()
{
	dashing = false;
	can_dash = true;
	dash_steps = 8;
}

void Player::clearDash()
{
	can_dash = false;
	dash_steps = 0;
}

void Player::setDeath()
{
	die_steps = 60;
}

void Player::setReappearing()
{
	reappearing = true;
	reappearing_steps = 15;
}

void Player::setPlatform(bool a) {
	platformJumping = a;
}

bool Player::getPlatform() {
	return platformJumping;
}

void Player::dash() {
	if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
	{
		posPlayer.x -=  12;
		sprite->changeAnimation(STAND_RIGHT);
		dashing = false;
		dash_steps = 0;
		if (map->collisionMoveRightJump(posPlayer, glm::ivec2(32, 32))) {
			if (bJumping) {
				stickied = true;
				bJumping = false;
			}
		}
	}

	else if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
	{
		posPlayer.x += 12;
		sprite->changeAnimation(STAND_LEFT);
		dashing = false;
		dash_steps = 0;
		if (map->collisionMoveLeftJump(posPlayer, glm::ivec2(32, 32))) {
			if (bJumping) {
				stickied = true;
				bJumping = false;
			}
		}
	}

	if (dash_direction == "left") {
		posPlayer.x -= 12;
	}
	else if (dash_direction == "right") {
		posPlayer.x += 12;
	}
	--dash_steps;
	if (dash_steps <= 0) {
		dash_steps = 0;
		dashing = false;
		can_dash = false;
	}
}

void Player::die() {
	if (die_steps > 30) posPlayer.y += 16;
	if (die_steps < 30) posPlayer.y -= 16;
	--die_steps;

	if (die_steps <= 0) {
		dead = true;
		die_steps = 60;
		dying = false;
	}
}

void Player::reappear() {
	if (reappearing_steps > 7) posPlayer.y += 15;
	else if (reappearing_steps < 8) posPlayer.y -= 25;
	--reappearing_steps;
	if (reappearing_steps <= 0) {
		reappearing_steps = 0;
		reappearing = false;
	}
}

void Player::changeDeathStatus(bool b)
{
	dead = b;
}

bool canDash() {
	return canDash;
}
