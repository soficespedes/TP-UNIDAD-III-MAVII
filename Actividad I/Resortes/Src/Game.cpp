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
		CheckCollitions(); // Verifica colisiones 
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


void Game::DrawGame()
{
	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	//Dibujar el techo
	sf::RectangleShape upWallShape(sf::Vector2f(500, 5));
	upWallShape.setFillColor(sf::Color::Red);
	upWallShape.setPosition(0, 0);
	wnd->draw(upWallShape);

	// Dibujar las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	// Dibujar el cuerpo de control (círculo)
	sf::CircleShape circle(5);
	circle.setFillColor(sf::Color::Blue);
	circle.setPosition(circuloCentral->GetPosition().x - 5, circuloCentral->GetPosition().y - 5);
	wnd->draw(circle);
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

void Game::CheckCollitions() //acá aplicaremos la ley de elasticidad y se activan resortes(joints) :
{
	if (!circuloCentral)
		return;

	b2Vec2 vel = circuloCentral->GetLinearVelocity();
	b2Vec2 pos = circuloCentral->GetPosition();

	const float elasticidad = 0.9f;

	// Colisión con la pared izquierda
	if (pos.x - 5.0f <= 0.0f && vel.x < 0)
	{
		if (!jointLeft) {
			float restLength = 45.0f;
			float freq = 2.0f;
			float damp = 0.5f;
			jointLeft = Box2DHelper::CreateDistanceJoint(phyWorld, circuloCentral, circuloCentral->GetWorldCenter(), leftWallBody, b2Vec2(5.0f, 50.0f), restLength, freq, damp);
		}
		vel.x = -vel.x * elasticidad;
		circuloCentral->SetLinearVelocity(vel);
	}
	else if (jointLeft)
	{
		phyWorld->DestroyJoint(jointLeft);
		jointLeft = nullptr;
	}

	// Colisión con la pared derecha
	if (pos.x + 5.0f >= 100.0f && vel.x > 0)
	{
		if (!jointRight) {
			float restLength = 45.0f;
			float freq = 2.0f;
			float damp = 0.5f;
			jointRight = Box2DHelper::CreateDistanceJoint(phyWorld, circuloCentral, circuloCentral->GetWorldCenter(), rightWallBody, b2Vec2(95.0f, 50.0f), restLength, freq, damp);
		}
		vel.x = -vel.x * elasticidad;
		circuloCentral->SetLinearVelocity(vel);
	}
	else if (jointRight)
	{
		phyWorld->DestroyJoint(jointRight);
		jointRight = nullptr;
	}
	// Colisión con el techo
	if (pos.y - 5.0f <= 0.0f && vel.y < 0)
	{
		vel.y = -vel.y * elasticidad;
		circuloCentral->SetLinearVelocity(vel);
	}
	// Colisión con el suelo
	if (pos.y + 5.0f >= 100.0f && vel.y > 0)
	{
		vel.y = -vel.y * elasticidad;
		circuloCentral->SetLinearVelocity(vel);
	}
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

	// Crea cuerpos estáticos para simular el suelo y las paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Creamos el cuerpo de la pelota (círculo) en una posición intermedia
	circuloCentral = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.8f);
	circuloCentral->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
	circuloCentral->SetLinearVelocity(b2Vec2(10.0f, 0.0f));
}

Game::~Game(void)
{}