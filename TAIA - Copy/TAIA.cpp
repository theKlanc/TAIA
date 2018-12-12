#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.hpp"
#include <map>
#include "Aresta.hpp"
#include <iostream>
#include <execution>
#include <fstream>


int main()
{
	srand(time(NULL));
	std::string nomMapa;
	std::cout << "Entra un nom pel mapa: "<<std::endl;
	std::cin >> nomMapa;
	std::map<long,Node> nodes;
	std::vector<Aresta> arestes;

#pragma region Textura flaçà
	sf::Texture texture;
	if (!texture.loadFromFile("bg.png"))
	{
		throw "lmao";
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);
#pragma endregion
#pragma region unserialize
	std::ifstream nodesStream("nodes_" + nomMapa + ".dat");
	std::ifstream arestesStream("arestes_" + nomMapa + ".dat");
	if (nodesStream.is_open()) {
		while (!nodesStream.eof()) {
			std::string temp;
			std::string tempFinal;
			nodesStream >> temp;
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
			nodes.insert(std::make_pair(nouNode.getUUID(),nouNode));
		}
	}
	if (arestesStream.is_open()) {
		while (!arestesStream.eof()) {
			std::string temp;
			std::string tempFinal;
			arestesStream >> temp;
			tempFinal += temp + " ";
			arestesStream >> temp;
			tempFinal += temp + " ";
			arestesStream >> temp;
			tempFinal += temp + " ";
			arestesStream >> temp;
			tempFinal += temp + " ";
			Aresta novaAresta;
			novaAresta.unserialize(tempFinal,nodes);
			arestes.push_back(novaAresta);
		}
	}

#pragma endregion
	sf::RenderWindow window(sf::VideoMode(texture.getSize().x, texture.getSize().y), nomMapa);

	bool halfDone = false;
	Node *origin = nullptr;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "the left button was pressed" << std::endl;
					long posX = event.mouseButton.x;
					long posY = event.mouseButton.y;
					Node temp(posX, posY);
					nodes.insert(std::make_pair(temp.getUUID(),temp));
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					if(!halfDone)
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
					halfDone = !halfDone;
				}
				if (event.mouseButton.button == sf::Mouse::Middle)
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
					long uuid = nodeMin.getUUID();
					for (auto n = nodes.begin(); n != nodes.end();n++)
					{
						if(n->second.getUUID() == uuid)
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
				if (event.key.code == sf::Keyboard::P) {
					double minima = DBL_MAX;
					Node nodeMin;
					for (auto&[uuid,n] : nodes)
					{
						double dist = n.getDistance(event.mouseButton.x, event.mouseButton.y);
						if (dist <= minima)
						{
							minima = dist;
							nodeMin = n;
						}

					}
					long uuid = nodeMin.getUUID();
					{
						int pob;
						std::cout << "ENTRA LA POBLACIO: " << std::endl;
						std::cin >> pob;
						nodes[uuid].setPoblacio(pob);
						break;
					}
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
					{
						int interes;
						std::cout << "ENTRA L INTERES: " << std::endl;
						std::cin >> interes;
						nodes[uuid].setInteres(interes);
						break;
					}
				}
			}
		}

		window.clear();
		window.draw(sprite);
		for(Aresta a : arestes)
		{
			window.draw(a.getShape());
		}
		for(auto&[uuid, n] : nodes)
		{
			window.draw(n.getShape());
		}
		window.display();
	}

#pragma region serialize
	std::ofstream sortidaArestes("arestes_"+nomMapa+".dat");
	std::ofstream sortidaNodes("nodes_"+nomMapa+".dat");
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
#pragma endregion
	return 0;
}
