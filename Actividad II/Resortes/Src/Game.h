#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;
	SFMLRenderer* debugRender;
	b2Body* circulo1;
	b2Body* circulo2;
	b2Body* cuerpoArrastrado = nullptr;
	bool arrastrando = false;

	// Tiempo de frame
	float frameTime;
	int fps;

public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CheckCollitions();
	bool EstaSobreCuerpo(b2Body* cuerpo, b2Vec2 mousePos);
	~Game(void);
	void InitPhysics();
	

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};