#pragma once
#include "cocos2d.h"
#include "KeyboardControllerComponent.h"

USING_NS_CC;

class FinalProject : public Scene
{

public:
	
	static Scene* createScene();

	virtual bool init();

	void changeMenuVisible(TMXLayer* option);

	void menuControls();

	void flipHelpText(TMXTiledMap* map, TMXTiledMap* help, Label* instructions, Label* controls);

	virtual void update(float dt);

	CREATE_FUNC(FinalProject)

private:

	KeyboardControllerComponent* controller;
	Sprite* menu;

	enum selectedMenu
	{
		PLAY,
		HELP
	} selected = PLAY;

	TMXTiledMap* map;
	TMXTiledMap* helpScreen;

	TMXLayer* playR;
	TMXLayer* playB;
	TMXLayer* helpR;
	TMXLayer* helpB;

	Label* instructionsText;
	Label* controlsText;
};