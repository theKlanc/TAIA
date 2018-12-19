#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <memory>

class Node
{
public:
	Node();
	Node(long posX, long posY);
	~Node();
	sf::CircleShape getShape();
	long getUUID();
	double getDistance(int posX,int posY);
	std::pair<int,int> getPos();
	std::string serialize();
	void unserialize(std::string);
	void setPoblacio(int p);
	void setInteres(int i);
	int getInteres();
private:
	long _UUID;
	int _posX, _posY;
	int _poblacio, _interes;
	sf::CircleShape *_shape;
};

