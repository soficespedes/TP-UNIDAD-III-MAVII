#pragma once
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;
//tuve inconvenientes para que el programa me leyera la clase independiente Ragdoll.h que habia creado, por eso la coloqué dentro de Game.h
class Ragdoll {
public:
	Ragdoll(b2World* world, b2Vec2 position);
	~Ragdoll();
private:
	// Piezas del cuerpo
	b2Body* torso;
	b2Body* upperArmLeft;
	b2Body* upperArmRight;
	b2Body* lowerArmLeft;
	b2Body* lowerArmRight;
	b2Body* upperLegLeft;
	b2Body* upperLegRight;
	b2Body* lowerLegLeft;
	b2Body* lowerLegRight;
	b2Body* head;

	// Joints (articulaciones)
	std::vector<b2RevoluteJoint*> joints;

	void CreateBodyParts(b2World* world, b2Vec2 position);
	void CreateJoints(b2World* world);
};

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
	Ragdoll* ragdoll;
	// Tiempo de frame
	float frameTime;
	int fps;
	
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
