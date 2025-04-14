#include "Avatar.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Avatar::Avatar(b2Body* pCuerpo, sf::Sprite* pSprite) : m_pCuerpo(pCuerpo)
{
	// Guarda la posici�n inicial del cuerpo f�sico
	m_posAux = m_pCuerpo->GetPosition();

	// Guarda un puntero al sprite asociado al cuerpo f�sico
	m_pSprite = pSprite;

	// Calcula el tama�o del sprite para que coincida con el cuerpo f�sico
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
		aabb.Combine(f->GetAABB(0));
	float sizeX = aabb.GetExtents().x * 2.0f / m_pSprite->getTexture()->getSize().x;
	float sizeY = aabb.GetExtents().y * 2.0f / m_pSprite->getTexture()->getSize().y;
	m_pSprite->setScale(sf::Vector2f(sizeX, sizeY));

	// Establece el origen del sprite en su centro
	m_pSprite->setOrigin(sf::Vector2f(m_pSprite->getTexture()->getSize().x / 2.0f, m_pSprite->getTexture()->getSize().y / 2.0f));

	// Establece la posici�n inicial del sprite
	m_pSprite->setPosition(m_posAux.x, m_posAux.y);

	// Asigna un puntero al objeto Avatar al userData del cuerpo f�sico
	m_pCuerpo->GetUserData().pointer = (uintptr_t)this;
}

Avatar::~Avatar()
{
	// Elimina todas las fixtures asociadas al cuerpo f�sico
	for (b2Fixture* f = m_pCuerpo->GetFixtureList(); f; f = f->GetNext())
		m_pCuerpo->DestroyFixture(f);

	// Libera la memoria del sprite si existe
	if (m_pSprite != NULL)
		delete m_pSprite;

	// Destruye el cuerpo f�sico
	m_pCuerpo->GetWorld()->DestroyBody(m_pCuerpo);

	// Asigna NULL al puntero del cuerpo f�sico
	m_pCuerpo = NULL;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

// Operador de conversi�n a b2Body
Avatar::operator b2Body* ()
{
	return m_pCuerpo;
}

// Actualiza la posici�n y el �ngulo del sprite de acuerdo con el cuerpo f�sico
void Avatar::Actualizar()
{
	m_posAux = m_pCuerpo->GetPosition();
	m_angAux = m_pCuerpo->GetAngle(); // Devuelve el �ngulo en radianes

	// Actualiza la posici�n del sprite
	m_pSprite->setPosition(m_posAux.x, m_posAux.y);

	// Actualiza la rotaci�n del sprite
	m_pSprite->setRotation(m_angAux * 180 / 3.14f); // Recibe el �ngulo en grados
	// Notamos que el sentido de rotaci�n de SFML y Box2D son opuestos
}

// Dibuja el sprite en la ventana de renderizado proporcionada
void Avatar::Dibujar(sf::RenderWindow& RW)
{
	RW.draw(*m_pSprite);
}

// Devuelve la posici�n del cuerpo f�sico
b2Vec2 Avatar::GetPosition()
{
	return m_pCuerpo->GetPosition();
}

// Devuelve una referencia al cuerpo f�sico
b2Body& Avatar::GetCuerpo()
{
	return *m_pCuerpo;
}

// Devuelve una referencia al sprite
sf::Sprite& Avatar::GetSprite()
{
	return *m_pSprite;
}

// Establece la posici�n del cuerpo f�sico
void Avatar::SetPosition(b2Vec2 position)
{
	m_pCuerpo->SetTransform(position, m_pCuerpo->GetAngle());
}

// Aplica una fuerza al cuerpo f�sico en un punto espec�fico
void Avatar::AplicarFuerza(b2Vec2& fuerza, b2Vec2& origen)
{
	m_pCuerpo->ApplyForce(fuerza, origen, true);
}

// Aplica un impulso al cuerpo f�sico en un punto espec�fico
void Avatar::AplicarImpulso(b2Vec2& impulso, b2Vec2& origen)
{
	m_pCuerpo->ApplyLinearImpulse(impulso, origen, true);
}
