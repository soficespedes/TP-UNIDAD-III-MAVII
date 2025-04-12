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
	b2Body* circuloCentral;
	b2Body* leftWallBody;
	b2Body* rightWallBody;
	b2Body* upWallBody;
	// Joints para cada lado:
	b2DistanceJoint* jointLeft = nullptr;
	b2DistanceJoint* jointRight = nullptr;
	// Tiempo de frame
	float frameTime;
	int fps;
	bool colisionIzquierda = false;
	bool colisionDerecha = false;


public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CheckCollitions();
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};