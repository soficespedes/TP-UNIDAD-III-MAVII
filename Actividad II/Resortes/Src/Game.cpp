#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();

	if (arrastrando && cuerpoArrastrado)
	{
		Vector2f mousePos = wnd->mapPixelToCoords(Mouse::getPosition(*wnd));
		b2Vec2 target(mousePos.x, mousePos.y);
		b2Vec2 current = cuerpoArrastrado->GetPosition();
		b2Vec2 velocity = target - current;
		cuerpoArrastrado->SetLinearVelocity(b2Vec2(velocity.x * 10.0f, velocity.y * 10.0f));
	}
	else if (cuerpoArrastrado)
	{
		cuerpoArrastrado->SetLinearVelocity(b2Vec2(0, 0));
		cuerpoArrastrado = nullptr;
	}
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

	sf::CircleShape shape1(5);
	shape1.setFillColor(sf::Color::Blue);
	shape1.setPosition(circulo1->GetPosition().x - 5, circulo1->GetPosition().y - 5);
	wnd->draw(shape1);

	sf::CircleShape shape2(5);
	shape2.setFillColor(sf::Color::Green);
	shape2.setPosition(circulo2->GetPosition().x - 5, circulo2->GetPosition().y - 5);
	wnd->draw(shape2);
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;

		case Event::MouseButtonPressed:
			if (evt.mouseButton.button == Mouse::Left)
			{
				Vector2f mousePos = wnd->mapPixelToCoords(Mouse::getPosition(*wnd));
				b2Vec2 mouseWorld(mousePos.x, mousePos.y);

				if (EstaSobreCuerpo(circulo1, mouseWorld))
				{
					cuerpoArrastrado = circulo1;
					arrastrando = true;
				}
				else if (EstaSobreCuerpo(circulo2, mouseWorld))
				{
					cuerpoArrastrado = circulo2;
					arrastrando = true;
				}
			}
			break;

		case Event::MouseButtonReleased:
			if (evt.mouseButton.button == Mouse::Left)
			{
				arrastrando = false;
			}
			break;
		}
	}
}

bool Game::EstaSobreCuerpo(b2Body* cuerpo, b2Vec2 mousePos)
{
	for (b2Fixture* f = cuerpo->GetFixtureList(); f; f = f->GetNext())
	{
		if (f->TestPoint(mousePos))
			return true;
	}
	return false;
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara);
}

void Game::InitPhysics()
{
	phyWorld = new b2World(b2Vec2(0.0f, 0.0f)); // sin gravedad vertical para permitir el efecto del resorte libre
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Crear el suelo y las paredes estáticas del mundo físico
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
	groundBody->GetFixtureList()->SetFriction(0.1f);  //menos fricción, más rebote

	b2Body* upWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	upWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);


	circulo1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5f, 0.1f);
	circulo1->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	circulo2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5f, 0.1f);
	circulo2->SetTransform(b2Vec2(80.0f, 50.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, circulo1, circulo1->GetWorldCenter(),
		circulo2, circulo2->GetWorldCenter(), 30.0f, 2.0f, 0.5f);
}

Game::~Game(void)
{
}
