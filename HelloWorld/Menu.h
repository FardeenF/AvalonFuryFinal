#pragma once
#include "Scene.h"



class Menu : public Scene
{
public:
	Menu(std::string name);

	virtual void InitScene(float windowWidth, float windowHeight);
	int Menu1();

private:
	int m_mainMenu;
};

struct MenuButton
{
	int buttonID;

};

