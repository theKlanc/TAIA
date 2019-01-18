#include "TaiaMain.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.hpp"
#include <map>
#include "Aresta.hpp"
#include <iostream>
#include <execution>
#include <fstream>
#include "Solucio.h"

const int GENERATIONS_PER_CYCLE = 1;

TaiaMain::TaiaMain()
{
	srand(time(NULL));
	std::cout << "Entra un nom pel mapa: " << std::endl;
	std::cin >> _nomMapa;

#pragma region Textura mapa
	if (!_texture.loadFromFile(_nomMapa + ".jpg"))
	{
		if (!_texture.loadFromFile(_nomMapa + ".png"))
		{
			throw std::exception("fug");
		}
	}
	_sprite.setTexture(_texture);
#pragma endregion
	unserialize();
	_window = new sf::RenderWindow(sf::VideoMode(_texture.getSize().x, _texture.getSize().y), _nomMapa);
	int distTotal = 0;
}


TaiaMain::~TaiaMain()
{
	serialize();
}

void TaiaMain::draw()
{
	_window->clear(sf::Color::White);
	if (_drawBG)_window->draw(_sprite);
	std::vector<sf::RectangleShape> llistaTemp;
	for (int i =0;i<_arestes.size();i++)
	{
		auto temp=_arestes[i].getShape();
		if(_everOptimized && _solutions[0].hiHaCarril(i))
		{
			int r =0;
			int g =255;
			int b =0;
			temp.setFillColor(sf::Color(r, g, b));
			llistaTemp.push_back(temp);
			continue;
		}
		_window->draw(temp);
	}
	for(sf::RectangleShape& r : llistaTemp)
	{
		_window->draw(r);
	}
	for (auto&[uuid, n] : _nodes)
	{
		_window->draw(n.getShape());
	}
	_window->display();
}
void TaiaMain::loop()
{
	while (_window->isOpen())
	{
		while (_window->pollEvent(_event))
		{
			dealWithEvent();
		}
		if (_optimizing)
		{
			runGeneration();
		}
		draw();
	}
}

void TaiaMain::initSolutions()
{
	int dist = 0;
	for (auto t : _arestes)
	{
		dist += t.getLength();
	}
	Solucio::setup(&_nodes, &_arestes, dist);
	for (int i = 0; i < MAX_SOLUTIONS; ++i)
	{
		Solucio temp = Solucio();
		_solutions.push_back(temp);
	}
	for(Solucio& temp : _solutions)
	{
		temp.getFitness();
	}
}

void TaiaMain::runGeneration()
{
	for (int i = 0; i < GENERATIONS_PER_CYCLE; ++i) {
		mutate();
		fug();
		darwin();
		std::cout << "finished Gen, best fitness: "<< _solutions[0].getFitness() << " dist :"<< _solutions[0].getDist() <<" Max Dist: "<<_solutions[0].getMaxDist()<< " Ntrams: " <<_solutions[0].getNTrams()<<std::endl;
	}
}

void TaiaMain::dealWithEvent()
{
	if (_event.type == sf::Event::Closed)
		_window->close();
	if (_event.type == sf::Event::MouseButtonPressed)
	{
		if (!_paintingInt && !_paintingPob) {
			if (_event.mouseButton.button == sf::Mouse::Left)
			{
				Node temp(_event.mouseButton.x, _event.mouseButton.y);
				_nodes.insert(std::make_pair(temp.getUUID(), temp));
			}
			if (_event.mouseButton.button == sf::Mouse::Right)
			{
				if (!_buildingEdge)
				{
					double minima = DBL_MAX;
					Node *nodeMin = nullptr;
					for (auto&[uuid, n] : _nodes)
					{
						double dist = n.getDistance(_event.mouseButton.x, _event.mouseButton.y);
						if (dist <= minima)
						{
							minima = dist;
							nodeMin = &n;
						}
					}
					_origin = nodeMin;
				}
				else
				{
					double minima = DBL_MAX;
					Node *nodeMin = nullptr;
					for (auto&[uuid, n] : _nodes)
					{
						double dist = n.getDistance(_event.mouseButton.x, _event.mouseButton.y);
						if (dist <= minima)
						{
							minima = dist;
							nodeMin = &n;
						}
					}
					Aresta temp(_origin, nodeMin);
					_arestes.push_back(temp);
					Aresta temp2(nodeMin, _origin);
					_arestes.push_back(temp2);
					_origin = nullptr;
				}
				_buildingEdge = !_buildingEdge;
			}
			if (_event.mouseButton.button == sf::Mouse::Middle)
			{
				_buildingEdge = false;
				double minima = DBL_MAX;
				Node nodeMin;
				for (auto&[uuid, n] : _nodes)
				{
					double dist = n.getDistance(_event.mouseButton.x, _event.mouseButton.y);
					if (dist <= minima)
					{
						minima = dist;
						nodeMin = n;
					}
				}
				long uuid = nodeMin.getUUID();

				bool clean = true;
				do {
					clean = true;
					for (auto n = _arestes.begin(); n != _arestes.end(); n++)
					{
						if (n->containsNodeWithUUID(uuid)) {
							clean = false;
							_arestes.erase(n);
							break;
						}
					}
				} while (!clean);
				for (auto&[uuid2, n] : _nodes)
				{
					if (n.getUUID() == uuid)
					{
						_nodes.erase(uuid);
						break;
					}
				}
			}
		}
		else
		{
			if (_paintingInt)
			{
				double minima = DBL_MAX;
				Node nodeMin;
				for (auto&[uuid, n] : _nodes)
				{
					double dist = n.getDistance(_event.mouseButton.x, _event.mouseButton.y);
					if (dist <= minima)
					{
						minima = dist;
						nodeMin = n;
					}
				}
				long uuid = nodeMin.getUUID();
				for (auto&[uuid2, n] : _nodes)
				{
					if (n.getUUID() == uuid)
					{
						n.setInteres(_interes);
						break;
					}
				}
			}
			else if (_paintingPob)
			{
				double minima = DBL_MAX;
				Node nodeMin;
				for (auto&[uuid, n] : _nodes)
				{
					double dist = n.getDistance(_event.mouseButton.x, _event.mouseButton.y);
					if (dist <= minima)
					{
						minima = dist;
						nodeMin = n;
					}
				}
				long uuid = nodeMin.getUUID();
				for (auto&[uuid2, n] : _nodes)
				{
					if (n.getUUID() == uuid)
					{
						n.setPoblacio(_poblacio);
						break;
					}
				}
			}
		}
	}
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			if (!_everOptimized)
				initSolutions();
			_everOptimized = true;
			_optimizing = !_optimizing;
		}
		if (_event.key.code == sf::Keyboard::B)
			_drawBG = !_drawBG;
		if (_event.key.code == sf::Keyboard::P) {
			if (!_paintingPob)
			{
				int pob;
				std::cout << "ENTRA LA POBLACIO: " << std::endl;
				std::cin >> pob;
				_poblacio = pob;
			}
			_paintingPob = !_paintingPob;
		}
		if (_event.key.code == sf::Keyboard::I) {
			if (!_paintingInt)
			{
				int lmao;
				std::cout << "ENTRA L INTERES: " << std::endl;
				std::cin >> lmao;
				_interes = lmao;
			}
			_paintingInt = !_paintingInt;
		}
	}
}

void TaiaMain::unserialize()
{
	std::ifstream nodesStream("nodes_" + _nomMapa + ".dat");
	std::ifstream arestesStream("arestes_" + _nomMapa + ".dat");
	if (nodesStream.is_open()) {
		while (!nodesStream.eof()) {
			std::string temp;
			std::string tempFinal;
			nodesStream >> temp;
			if (temp == "")break;
			tempFinal += temp + " ";
			nodesStream >> temp;
			tempFinal += temp + " ";
			nodesStream >> temp;
			tempFinal += temp + " ";
			nodesStream >> temp;
			tempFinal += temp + " ";
			nodesStream >> temp;
			tempFinal += temp + " ";
			Node nouNode;
			nouNode.unserialize(tempFinal);
			_nodes.insert(std::make_pair(nouNode.getUUID(), nouNode));
		}
	}
	if (arestesStream.is_open()) {
		while (!arestesStream.eof()) {
			std::string temp;
			std::string tempFinal;
			arestesStream >> temp;
			if (temp == "")break;
			tempFinal += temp + " ";
			arestesStream >> temp;
			tempFinal += temp + " ";
			arestesStream >> temp;
			tempFinal += temp + " ";
			Aresta novaAresta;
			try {
				novaAresta.unserialize(tempFinal, &_nodes);
			}
			catch (std::exception e)
			{
				continue;
			}
			_arestes.push_back(novaAresta);
		}
	}
}

void TaiaMain::serialize()
{
	std::ofstream sortidaArestes("arestes_" + _nomMapa + ".dat");
	std::ofstream sortidaNodes("nodes_" + _nomMapa + ".dat");
	for (auto&[uuid, n] : _nodes)
	{
		sortidaNodes << n.serialize() << std::endl;
	}
	sortidaNodes.close();
	for (Aresta a : _arestes)
	{
		sortidaArestes << a.serialize() << std::endl;
	}
	sortidaArestes.close();
}

void TaiaMain::mutate()
{
	std::vector<Solucio> vectorTempSolucions;
	for(Solucio& s : _solutions)
	{
		if(rand()%100<15)
		{
			vectorTempSolucions.push_back(s.mutate());
		}
	}
	_solutions.insert(_solutions.end(),vectorTempSolucions.begin(),vectorTempSolucions.end());
}

void TaiaMain::fug()
{
	std::vector<Solucio> vectorTempSolucions;
	for(Solucio& s : _solutions)
	{
		if(rand()%100<10)
		{
			vectorTempSolucions.push_back(s.fug(_solutions[rand()%_solutions.size()]));
		}
	}
	_solutions.insert(_solutions.end(),vectorTempSolucions.begin(),vectorTempSolucions.end());
}

void TaiaMain::darwin()
{
	std::sort(_solutions.begin(), _solutions.end());
	for (int i = _solutions.size(); i > MAX_SOLUTIONS;--i)
	{
		_solutions.pop_back();
	}
}
