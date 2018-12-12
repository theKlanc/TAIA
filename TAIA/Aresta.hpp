#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include "Node.hpp"

class Aresta
{
public:
	Aresta();
	Aresta(Node n1, Node n2);
	~Aresta();
	sf::RectangleShape getShape();
	long getUUID();
	bool containsNodeWithUUID(long uuid);
	std::string serialize();
	void unserialize(std::string temp,std::vector<Node> vNodes);
private:
	long _UUID;
	Node _n1, _n2;
	int _length;
	sf::RectangleShape *_shape;
};

