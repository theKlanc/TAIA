#pragma once
#include <string>
#include "Aresta.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class TaiaMain
{
public:
	TaiaMain();
	~TaiaMain();
	void loop();
private:
	std::string nomMapa;
	std::map<long, Node> nodes;
	std::vector<Aresta> arestes;


	sf::RenderWindow *window;
	sf::Event event;

	sf::Sprite sprite;
	sf::Texture texture;

	Node *origin = nullptr;

	bool buildingEdge = false;
	bool optimizing = false;
	bool drawBG = true;

	void runGeneration();
	void dealWithEvent();
	void draw();
	void unserialize();
	void serialize();
};
