#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}
Ragdoll::Ragdoll(b2World* world, b2Vec2 position) {
	CreateBodyParts(world, position);
	CreateJoints(world);
}

Ragdoll::~Ragdoll() {
}

void Ragdoll::CreateBodyParts(b2World* world, b2Vec2 pos) {

	// Cabeza
	head = Box2DHelper::CreateRectangularDynamicBody(world, 3.5, 3.5, 1.0f, 1.0f, 0.1f);
	head->SetTransform(pos + b2Vec2(0, -5), 0.0f);

	// Torso
	torso = Box2DHelper::CreateRectangularDynamicBody(world, 5, 8, 1.0f, 0.3f, 0.1f);
	torso->SetTransform(pos, 0.0f);

	// Brazos
	upperArmLeft = Box2DHelper::CreateRectangularDynamicBody(world, 2, 6, 1.0f, 0.3f, 0.1f);
	upperArmLeft->SetTransform(pos + b2Vec2(-4.5f, -2.0f), 0.0f);

	upperArmRight = Box2DHelper::CreateRectangularDynamicBody(world, 2, 6, 1.0f, 0.3f, 0.1f);
	upperArmRight->SetTransform(pos + b2Vec2(4.5f, -2.0f), 0.0f);

	// Piernas
	upperLegLeft = Box2DHelper::CreateRectangularDynamicBody(world, 2.5f, 10, 1.0f, 0.3f, 0.1f);
	upperLegLeft->SetTransform(pos + b2Vec2(-1.7f, 6.5f), 0.0f);

	upperLegRight = Box2DHelper::CreateRectangularDynamicBody(world, 2.5f, 10.0f, 1.0f, 0.3f, 0.1f);
	upperLegRight->SetTransform(pos + b2Vec2(1.7f, 6.5f), 0.0f);
}

void Ragdoll::CreateJoints(b2World* world) {
	b2RevoluteJointDef jointDef;
	jointDef.collideConnected = false;

	// Cabeza al torso, no supe como corregir para que la cabeza no se fuese a un lado al caer
	jointDef.bodyA = head;
	jointDef.bodyB = torso;
	jointDef.localAnchorA.Set(0, 1.5f);    
	jointDef.localAnchorB.Set(0, -4.0f);   
	joints.push_back(static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef)));

	// Brazo izquierdo al torso
	jointDef.bodyA = upperArmLeft;
	jointDef.bodyB = torso;
	jointDef.localAnchorA.Set(0, 3.0f);    
	jointDef.localAnchorB.Set(-2.5f, -2.5f); 
	joints.push_back(static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef)));

	// Brazo derecho al torso
	jointDef.bodyA = upperArmRight;
	jointDef.bodyB = torso;
	jointDef.localAnchorA.Set(0, 3.0f);
	jointDef.localAnchorB.Set(2.5f, -2.5f);
	joints.push_back(static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef)));

	// Pierna izquierda al torso
	jointDef.bodyA = upperLegLeft;
	jointDef.bodyB = torso;
	jointDef.localAnchorA.Set(0, -3.0f);   
	jointDef.localAnchorB.Set(-1.5f, 4.0f); 
	joints.push_back(static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef)));

	// Pierna derecha al torso
	jointDef.bodyA = upperLegRight;
	jointDef.bodyB = torso;
	jointDef.localAnchorA.Set(0, -3.0f);
	jointDef.localAnchorB.Set(1.5f, 4.0f);
	joints.push_back(static_cast<b2RevoluteJoint*>(world->CreateJoint(&jointDef)));
}


void Game::DrawGame()
{
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt)) // Procesa todos los eventos acumulados
	{
		switch (evt.type)
		{
		case Event::Closed: // Si se solicita cerrar la ventana
			wnd->close(); // Cierra la ventana
			break;
		}
	}
}

void Game::CheckCollitions()
{
}
void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo, las paredes y el techo
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// creamos nuestro pequeño monstruo (ragdollsito)
	ragdoll = new Ragdoll(phyWorld, b2Vec2(50.0f, 70.0f));
}

Game::~Game(void)
{}