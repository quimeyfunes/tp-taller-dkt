#include "RelojSprite.h"


RelojSprite::RelojSprite(void)
{
}

//RelojSprite::RelojSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex, string nombre,int maximosCLientes): DibujableTextura(){
//
//}


RelojSprite::~RelojSprite(void)
{
	delete []this->recCuadro;
}
