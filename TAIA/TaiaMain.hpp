#pragma once
#include <string>
#include "Aresta.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "Solucio.h"

class TaiaMain
{
public:
	TaiaMain();
	~TaiaMain();
	void loop();
private:
	std::string _nomMapa;
	std::map<long, Node> _nodes;
	std::vector<Aresta> _arestes;

	std::vector<Solucio> _solutions;


	sf::RenderWindow *_window;
	sf::Event _event;

	sf::Sprite _sprite;
	sf::Texture _texture;

	Node *_origin = nullptr;

	bool _everOptimized=false;
	bool _buildingEdge = false;
	bool _optimizing = false;
	bool _drawBG = true;
	bool _paintingPob = false;
	bool _paintingInt = false; 

	int _interes = 0;
	int _poblacio = 0;

	const int MAX_SOLUTIONS=3000;

	void initSolutions();

	void runGeneration();
	void dealWithEvent();
	void draw();
	void unserialize();
	void serialize();

	void mutate();
	void fug();
	void darwin();
};
