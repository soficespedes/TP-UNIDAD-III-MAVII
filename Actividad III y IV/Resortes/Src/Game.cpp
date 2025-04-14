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


void Game::DrawGame()
{
	sf::RectangleShape squareShape(sf::Vector2f(20, 20));
	squareShape.setFillColor(sf::Color::Red);
	squareShape.setPosition(40, 40);
	wnd->draw(squareShape);

	sf::CircleShape circleShape(5);
	circleShape.setFillColor(sf::Color::Yellow);
	circleShape.setPosition(circuloCentral->GetPosition().x - 5, circuloCentral->GetPosition().y - 5);
	wnd->draw(circleShape);
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
{}

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


	// Creamos el cuadrado
	b2Body* squareBody = Box2DHelper::CreateRectangularStaticBody(phyWorld,  20, 20);
	squareBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	// Creamos un circulo que ataremos al resorte
	circuloCentral = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloCentral->SetTransform(b2Vec2(20.0f, 50.0f), 0.0f);

	// Atamos la esfera mediante un resorte al cuadrado
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloCentral, circuloCentral->GetWorldCenter(),
		squareBody, squareBody->GetWorldCenter(), 20.0f, 0.1f, 0.0f);

	
}

Game::~Game(void)
{}