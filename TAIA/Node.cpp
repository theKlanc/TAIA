#include "Node.hpp"
#include <iostream>
#include <string>
#include <sstream>


Node::Node()
{
	_UUID = rand();
}

Node::Node(long posX, long posY)
{
	_UUID = rand();
	_shape = new sf::CircleShape(4.f);
	_shape->setPosition(posX-2, posY-2);
	_posX = posX;
	_posY = posY;
	int r =0;
	int g =0;
	int b =255;
	_shape->setFillColor(sf::Color(r, g, b));
	_poblacio = 2;
	_interes = 0;
}


Node::~Node()
{
}

sf::CircleShape Node::getShape()
{
	return *_shape;
}

long Node::getUUID()
{
	return _UUID;
}

double Node::getDistance(int posX, int posY)
{
	return sqrt(pow((posX - _posX), 2) + pow((posY - _posY), 2));
}

std::pair<int,int> Node::getPos()
{
	return std::make_pair(_posX, _posY);
}
std::string Node::serialize()
{
	return std::to_string(_UUID) + " " + std::to_string(_poblacio) + " " + std::to_string(_interes) + " " + std::to_string(_posX) + " " + std::to_string(_posY);
}

void Node::unserialize(std::string entrada)
{
	std::stringstream ss(entrada);
	int tempI;
	ss >> tempI;
	_UUID = tempI;
	ss >> tempI;
	_poblacio = tempI;
	ss >> tempI;
	_interes = tempI;
	ss >> tempI;
	_posX = tempI;
	ss >> tempI;
	_posY = tempI;

	_shape = new sf::CircleShape(4.f);
	_shape->setPosition(_posX - 2, _posY - 2);
	int r =0;
	int g =0;
	int b =255;
	_shape->setFillColor(sf::Color(r, g, b));
}

void Node::setPoblacio(int p)
{
	_poblacio = p;
	_shape->setFillColor(sf::Color(0, (double)p*2.55f, 0));
}

void Node::setInteres(int i)
{
	_interes = i;
	_shape->setFillColor(sf::Color((double)i*2.55f, 0, 0));
}

int Node::getInteres()
{return _interes;
}

