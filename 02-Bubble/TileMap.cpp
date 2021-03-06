#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}

//Render del mapa
void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}


//Llegir nivell del .txt
bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	int var1, var2;
	char var5;
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < (((mapSize.x) * 2) - 1); i++) {
			fin.get(tile);
			if (tile != ' ') {
				if (tile == '\n') {
					fin.get(tile);
				}
				if (tile == '-') {
					char tile2;
					fin.get(tile2);
					if (i != 0) {
						map[j * mapSize.x + (i - (i / 2))] = 0;
					}
					else {
						map[j * mapSize.x + i] = 0;
					}
				}
				else {
					char tile2;
					fin.get(tile2);
					if (tile2 == ' ') {
						fin.unget();
						if (i != 0) {
							map[j * mapSize.x + (i - (i / 2))] = (tile - int('0')) + 1;

						}
						else {
							map[j * mapSize.x + i] = (tile - int('0')) + 1;
						}
					}
					else {
						string s1, s2, s3;
						s1.append(1, tile);
						s2.append(1, tile2);
						s3.append(s1);
						s3.append(s2);
						int number = stoi(s3);
						if (i != 0) {
							map[j * mapSize.x + (i - (i / 2))] = number + 1;
						}
						else {
							map[j * mapSize.x + i] = number + 1;
						}
						tile2 = ' ';
					}
				}
			}
			else {
				while (tile == ' ' || tile == '\n') {
					fin.get(tile);
				}
				fin.unget();
			}
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

//Colisi? amb mapa esquerra
bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize; //dividim entre els pixels del tile per a que la x estigui en tiles --> x es posicio de esquerra de tot 
	y0 = pos.y / tileSize; //dividim entre els pixels del tile per a que la 7 estigui en tiles --> y es la posicio de dalt de tot
	y1 = (pos.y + size.y - 1) / tileSize; //y1 --> posicio final de la caixa en y --> abaix de la caixa
	if ((x < 0)) return true;
	for(int y=y0; y<=y1; y++) //bucle per a veure si hi ha algun tros de la caixa que colisiona
	{
		   if(map[y*mapSize.x+x] != 0&& map[y*mapSize.x+x] != 12 && map[y * mapSize.x + x] != 13) //si es != 0 significa que hi ha un tile i per tant retorna true
			return true;
	}
	
	return false;
}
//Colisi? amb mapa dreta
bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	if ((x > mapSize.x)) return true;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0 && map[y*mapSize.x+x] != 12 && map[y * mapSize.x + x] != 13)
			return true;
	}
	
	return false;
}


//Colisi? salt dreta
bool TileMap::collisionMoveRightJump(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize + 1;
	if ((x > mapSize.x)) return true;
	if (y < 0) return true;
	//y1 = (pos.y + size.y - 1) / tileSize;
	//for (int y = y0; y <= y1; y++)
	//{
		if (map[y*mapSize.x + x] != 0 && map[y * mapSize.x + x] != 12 && map[y * mapSize.x + x] != 13)
			return true;
	//}

	return false;
}


//Colisi? salt esquerra
bool TileMap::collisionMoveLeftJump(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y, y1;

	x = pos.x / tileSize;
	y = pos.y / tileSize + 1;
	if ((x < 0)) return true;
	if (y < 0) return true;
	//y1 = (pos.y + size.y - 1) / tileSize;
	//for (int y = y0; y <= y1; y++)
	//{
	if (map[y*mapSize.x + x] != 0 && map[y * mapSize.x + x] != 12 && map[y * mapSize.x + x] != 13)
		return true;
	//}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, x2, y;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y) / tileSize;
	if (y > mapSize.y) return true;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0 && map[y * mapSize.x + x] != 12 && map[y * mapSize.x + x] != 13)
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y) / tileSize;
	if (y < 0) return true;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 12 && map[y * mapSize.x + x] != 13)
		{
				return true;
		}
	}

	return false;
}
bool TileMap::deathCollisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{

	if (activeDeath) {
		int x, y0, y1;

		x = pos.x / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++)
		{
			if (map[y * mapSize.x + x] == 18)
				return true;
		}

		return false;
	}
	else return false;

}

bool TileMap::deathCollisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	if (activeDeath) {
		int x, y0, y1;

		x = (pos.x + size.x - 1) / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++)
		{
			if (map[y * mapSize.x + x] == 18)
				return true;
		}

		return false;
	}
	else return false;

}

bool TileMap::deathCollisionMoveRightJump(const glm::ivec2& pos, const glm::ivec2& size) const
{
	if (activeDeath) {
		int x, y, y1;

		x = (pos.x + size.x - 1) / tileSize;
		y = pos.y / tileSize + 1;
		//y1 = (pos.y + size.y - 1) / tileSize;
		//for (int y = y0; y <= y1; y++)
		//{
		if (map[y * mapSize.x + x] == 18)
			return true;
		//}

		return false;
	}
	else return false;

}

bool TileMap::deathCollisionMoveLeftJump(const glm::ivec2& pos, const glm::ivec2& size) const
{

	if (activeDeath) {
		int x, y, y1;

		x = pos.x / tileSize;
		y = pos.y / tileSize + 1;
		//y1 = (pos.y + size.y - 1) / tileSize;
		//for (int y = y0; y <= y1; y++)
		//{
		if (map[y * mapSize.x + x] == 18)
			return true;
		//}

		return false;
	}
	else return false;

}

bool TileMap::deathCollisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	if (activeDeath) {
		int x0, x1, y;

		x0 = pos.x / tileSize;
		x1 = (pos.x + size.x - 1) / tileSize;
		y = (pos.y + size.y - 1) / tileSize;
		for (int x = x0; x <= x1; x++)
		{
			if (map[y * mapSize.x + x] == 18)
			{
				if (*posY - tileSize * y + size.y <= 4)
				{
					*posY = tileSize * y - size.y;
					return true;
				}
			}
		}

		return false;

	}
	else return false;

}

bool TileMap::deathCollisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	if (activeDeath) {
		int x0, x1, y;

		x0 = pos.x / tileSize;
		x1 = (pos.x + size.x - 1) / tileSize;
		y = (pos.y) / tileSize;
		for (int x = x0; x <= x1; x++)
		{
			if (map[y * mapSize.x + x] == 18)
			{
				return true;
			}
		}

		return false;

	}
	else return false;

}



void TileMap::setActiveDeath(bool a) {
	activeDeath = a;
}

bool TileMap::getActiveDeath() {
	return activeDeath;
}



























