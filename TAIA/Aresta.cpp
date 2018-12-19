#include "Aresta.hpp"
#include <string>
#include <sstream>


Aresta::Aresta()
{
}

Aresta::Aresta(Node n1, Node n2)
{
	if (n1.getUUID() == n2.getUUID()) {
		throw "LMAO";
	}
	if (n1.getUUID() < n2.getUUID()) {
		_n1 = n1;
		_n2 = n2;
	}
	else {
		_n1 = n2;
		_n2 = n1;
	}
	_length = _n1.getDistance(_n2.getPos().first, _n2.getPos().second);
	_shape = new sf::RectangleShape(sf::Vector2f(_length, 3.f));
	_shape->setPosition(_n1.getPos().first+2, _n1.getPos().second+2);

	double delta_x = _n2.getPos().first - _n1.getPos().first;
	double delta_y = _n2.getPos().second - _n1.getPos().second;
	double theta_radians = atan2(delta_y, delta_x);
	_shape->setRotation(theta_radians * 180 / 3.1415926535f);



	int r =0;
	int g =0;
	int b =0;
	_shape->setFillColor(sf::Color(r, g, b));
}


Aresta::~Aresta()
{
}

sf::RectangleShape Aresta::getShape()
{
	return *_shape;
}


std::string Aresta::serialize()
{
	return std::to_string(_n1.getUUID()) + " " + std::to_string(_n2.getUUID()) + " " + std::to_string(_length);
}

void Aresta::unserialize(std::string entrada, std::map<long,Node> vNodes)
{
	std::stringstream ss(entrada);
	int tempI;
	ss >> tempI;
	for (auto&[uuid, n] : vNodes) {
		if (n.getUUID() == tempI) {
			_n1 = n;
			break;
		}
	}
	ss >> tempI;
	bool swop = false;
	if (tempI == _n1.getUUID()) {
		throw std::exception("RIPEO");
	}
	if (tempI < _n1.getUUID()) {
		_n2 = _n1;
		swop = true;
	}
	for (auto&[uuid, n] : vNodes) {
		if (n.getUUID() == tempI) {
			if (swop) {
				_n1 = n;
			}
			else {
				_n2 = n;
			}
			break;
		}
	}
	ss >> _length;
	_shape = new sf::RectangleShape(sf::Vector2f(_length, 3.f));
	_shape->setPosition(_n1.getPos().first + 2, _n1.getPos().second + 2);

	double delta_x = _n2.getPos().first - _n1.getPos().first;
	double delta_y = _n2.getPos().second - _n1.getPos().second;
	double theta_radians = atan2(delta_y, delta_x);
	_shape->setRotation(theta_radians * 180 / 3.1415926535f);



	int r =0;
	int g =0;
	int b =0;
	_shape->setFillColor(sf::Color(r, g, b));
}

int Aresta::getLength()
{
	return _length;
}

bool Aresta::containsNodeWithUUID(long uuid) {
	return uuid == _n1.getUUID() || uuid == _n2.getUUID();
}
