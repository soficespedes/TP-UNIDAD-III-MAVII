#include "Game.h"
#include <tchar.h>

using namespace sf;


int _tmain(int argc, _TCHAR* argv[])
{
    // Creación del juego y bucle principal
    Game* Juego;
    Juego = new Game(800, 600, "Resortes - MAVII");
    Juego->Loop(); // Inicia el bucle principal del juego

    return 0;
}
