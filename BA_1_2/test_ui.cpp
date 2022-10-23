// from 

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_GUI.hpp"
#include"BA_Test.hpp"

void ba::test::_ui::initSDL2(void)
{
	int img_f = IMG_INIT_JPG;// | IMG_INIT_PNG;
	if ((SDL_Init(SDL_INIT_EVERYTHING) == -1) || (TTF_Init() == -1) || (IMG_Init(img_f) != (img_f)))
		MyBA_Err("Fail to init SDL2", 1);
	MyBA_PutLog("succeed to init SDL2");
}