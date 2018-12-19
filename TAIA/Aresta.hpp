#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include "Node.hpp"
#include <map>
class Aresta
{
public:
	Aresta();
	Aresta(Node *n1, Node *n2);
	~Aresta();
	sf::RectangleShape getShape();
	bool containsNodeWithUUID(long uuid);
	std::string serialize();
	void unserialize(std::string temp,std::map<long,Node> *vNodes);
	int getLength();
	long getDestination();
	long getOrigin();
	void setColor(int r,int g,int b);
private:
	Node *_n1, *_n2;
	int _length;
	sf::RectangleShape *_shape;
};

