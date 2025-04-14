#include "Game.h"

using namespace sf;

class Pelota
{
private:
    // Sprite y su imagen para representaci�n gr�fica
    Sprite* _sprite;
    Texture* _image;
    RenderWindow* wnd;
    // Body para representaci�n f�sica
    b2Body* _body;

public:
    // Constructor de la clase Pelota
    Pelota(b2World* _world, RenderWindow* _wnd)
    {
        // Guardamos un puntero a la ventana para dibujar luego
        wnd = _wnd;
        // Cargamos la textura de la pelota desde un archivo
        _image = new Texture();
        _image->loadFromFile("Pelota.png");
        // Creamos el sprite usando la textura cargada
        _sprite = new Sprite(*_image);

        // Definimos las propiedades del cuerpo f�sico de la pelota
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(100.0f, 0.0f); // Posici�n inicial del cuerpo f�sico
        _body = _world->CreateBody(&bodyDef);

        // Creamos la forma de colisi�n para la pelota (en este caso, un cuadrado)
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(20.0f, 20.0f); // Tama�o de la pelota

        // Definimos las propiedades del fixture de la pelota
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 10.0f; // Densidad del cuerpo f�sico
        fixtureDef.friction = 0.3f; // Fricci�n del cuerpo f�sico
        fixtureDef.restitution = 1.0f; // Restituci�n (elasticidad) del cuerpo f�sico
        _body->CreateFixture(&fixtureDef); // Creamos el fixture y lo asociamos al cuerpo f�sico
    }

    // M�todo que actualiza la posici�n del sprite seg�n la posici�n del cuerpo f�sico
    void ActualizarPosiciones()
    {
        b2Vec2 pos = _body->GetPosition();
        _sprite->setPosition(pos.x, pos.y);
    }

    // M�todo que dibuja el sprite en la ventana
    void Dibujar()
    {
        wnd->draw(*_sprite);
    }
    // Otros m�todos relacionados con el personaje
};

// Funci�n principal del programa
int main(int argc, char* argv[])
{
    // Creamos una instancia del juego
    Game* Juego;
    // Inicializamos el juego con una ventana de 800x600 p�xeles con el t�tulo "Avatar"
    Juego = new Game(800, 600, "Avatar - MAVII");
    // Iniciamos el bucle principal del juego
    Juego->Loop();

    return 0;
}
