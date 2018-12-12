#include "TaiaMain.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.hpp"
#include <map>
#include "Aresta.hpp"
#include <iostream>
#include <execution>
#include <fstream>



TaiaMain::TaiaMain()
{
	srand(time(NULL));
	std::cout << "Entra un nom pel mapa: " << std::endl;
	std::cin >> nomMapa;

#pragma region Textura mapa
	if (!texture.loadFromFile(nomMapa + ".jpg"))
	{
		if(!texture.loadFromFile(nomMapa + ".png"))
		{
			throw std::exception("fug");
		}
	}
	sprite.setTexture(texture);
#pragma endregion
	unserialize();
	window = new sf::RenderWindow(sf::VideoMode(texture.getSize().x, texture.getSize().y), nomMapa);
}


TaiaMain::~TaiaMain()
{
	serialize();
}

void TaiaMain::draw()
{
	window->clear(sf::Color::White);
	if(drawBG)window->draw(sprite);
	for (Aresta a : arestes)
	{
		window->draw(a.getShape());
	}
	for (auto&[uuid, n] : nodes)
	{
		window->draw(n.getShape());
	}
	window->display();
}
void TaiaMain::loop()
{
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			dealWithEvent();
		}
		if(optimizing)
		{
			runGeneration();
		}
		draw();
	}
}

void TaiaMain::runGeneration()
{
}

void TaiaMain::dealWithEvent()
{
	if (event.type == sf::Event::Closed)
		window->close();
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			Node temp(event.mouseButton.x, event.mouseButton.y);
			nodes.insert(std::make_pair(temp.getUUID(), temp));
		}
		if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (!buildingEdge)
			{
				double minima = DBL_MAX;
				Node nodeMin;
				for (auto&[uuid, n] : nodes)
				{
					double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
					if (dist <= minima)
					{
						minima = dist;
						nodeMin = n;
					}
				}
				origin = new Node(nodeMin);
			}
			else
			{
				double minima = DBL_MAX;
				Node nodeMin;
				for (auto&[uuid, n] : nodes)
				{
					double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
					if (dist <= minima)
					{
						minima = dist;
						nodeMin = n;
					}
				}
				Aresta temp(*origin, *new Node(nodeMin));
				arestes.push_back(temp);
				origin = nullptr;
			}
			buildingEdge = !buildingEdge;
		}
		if (event.mouseButton.button == sf::Mouse::Middle)
		{
			buildingEdge = false;
			double minima = DBL_MAX;
			Node nodeMin;
			for (auto&[uuid, n] : nodes)
			{
				double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
				if (dist <= minima)
				{
					minima = dist;
					nodeMin = n;
				}
			}
			long uuid = nodeMin.getUUID();
			for (auto n = nodes.begin(); n != nodes.end(); n++)
			{
				if (n->second.getUUID() == uuid)
				{
					nodes.erase(n->first);
					break;
				}
			}
			bool clean = true;
			do {
				clean = true;
				for (auto n = arestes.begin(); n != arestes.end(); n++)
				{
					if (n->containsNodeWithUUID(uuid)) {
						clean = false;
						arestes.erase(n);
						break;
					}
				}
			} while (!clean);
		}
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::B)
			drawBG = !drawBG;
		if (event.key.code == sf::Keyboard::P) {
			double minima = DBL_MAX;
			Node nodeMin;
			for (auto&[uuid, n] : nodes)
			{
				double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
				if (dist <= minima)
				{
					minima = dist;
					nodeMin = n;
				}

			}
			long uuid = nodeMin.getUUID();
			int pob;
			std::cout << "ENTRA LA POBLACIO: " << std::endl;
			std::cin >> pob;
			nodes[uuid].setPoblacio(pob);
		}
		if (event.key.code == sf::Keyboard::I) {
			double minima = DBL_MAX;
			Node nodeMin;
			for (auto&[uuid, n] : nodes)
			{
				double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
				if (dist <= minima)
				{
					minima = dist;
					nodeMin = n;
				}

			}
			long uuid = nodeMin.getUUID();
			int interes;
			std::cout << "ENTRA L INTERES: " << std::endl;
			std::cin >> interes;
			nodes[uuid].setInteres(interes);
		}
	}
}

void TaiaMain::unserialize()
{
	std::ifstream nodesStream("nodes_" + nomMapa + ".dat");
	std::ifstream arestesStream("arestes_" + nomMapa + ".dat");
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
			nodes.insert(std::make_pair(nouNode.getUUID(), nouNode));
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
				novaAresta.unserialize(tempFinal, nodes);
			}catch(std::exception e)
			{
				continue;
			}
			arestes.push_back(novaAresta);
		}
	}
}

void TaiaMain::serialize()
{
	std::ofstream sortidaArestes("arestes_" + nomMapa + ".dat");
	std::ofstream sortidaNodes("nodes_" + nomMapa + ".dat");
	for (auto&[uuid, n] : nodes)
	{
		sortidaNodes << n.serialize() << std::endl;
	}
	sortidaNodes.close();
	for (Aresta a : arestes)
	{
		sortidaArestes << a.serialize() << std::endl;
	}
	sortidaArestes.close();
}
