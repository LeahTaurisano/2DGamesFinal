#include "FinalProject.h"
#include "Level1.h"

Scene* FinalProject::createScene()
{
	return FinalProject::create();
}

bool FinalProject::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	map = TMXTiledMap::create("tmx/TitleScreen.tmx");
	auto titleR = map->getLayer("TitleRed");
	auto titleB = map->getLayer("TitleBlue");
	auto background = map->getLayer("Background");
	playR = map->getLayer("PlayRed");
	playB = map->getLayer("PlayBlue");
	auto dePlay = map->getLayer("DePlay");
	helpR = map->getLayer("HelpRed");
	helpB = map->getLayer("HelpBlue");
	auto deHelp = map->getLayer("DeHelp");

	helpScreen = TMXTiledMap::create("tmx/HelpScreen.tmx");

	map->setScale(1.1f);
	helpScreen->setScale(1.1f);

	helpR->setVisible(false);
	helpB->setVisible(false);

	helpScreen->setVisible(false);

	this->addChild(map);
	this->addChild(helpScreen);

	menu = Sprite::create("RedPlayer.png");
	this->addChild(menu);
	menu->setVisible(false);

	controller = KeyboardControllerComponent::create(KeyboardControllerComponent::ARROWS);
	menu->addComponent(controller);
	controller->initInput();

	instructionsText = Label::createWithSystemFont("Step on switches to turn matching colored gates on and off!\n\n\n\nReach the key to open the golden gates.\n\n\n\nBoth players must stand on dual switches to open dual gates.\n\n\n\nBoth players much reach the end to beat the level!", "arial.ttf", 20.f);
	instructionsText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
	this->addChild(instructionsText, 0);
	instructionsText->setVisible(false);

	controlsText = Label::createWithSystemFont("WASD to move player 1.\n\n\nArrow keys to move player 2.\n\n\nPress R to respawn both players.\n\n\nEscape to return to the main menu, and open options during the level!", "arial.ttf", 20.f);
	controlsText->setPosition(Vec2(visibleSize.width / 1.85, visibleSize.height / 4));
	this->addChild(controlsText, 0);
	controlsText->setVisible(false);

	scheduleUpdate();

	return true;
}

void FinalProject::changeMenuVisible(TMXLayer* option)
{
	option->setVisible(!option->isVisible());
}

void FinalProject::menuControls()
{
	if (controller->IsUpPressed())
	{
		if (selected == PLAY) selected = HELP;
		else selected = PLAY;
		
		changeMenuVisible(playR);
		changeMenuVisible(playB);
		changeMenuVisible(helpR);
		changeMenuVisible(helpB);
	}

	if (controller->IsDownPressed())
	{
		if (selected == PLAY) selected = HELP;
		else selected = PLAY;

		changeMenuVisible(playR);
		changeMenuVisible(playB);
		changeMenuVisible(helpR);
		changeMenuVisible(helpB);
	}

	if (controller->isEnterPressed())
	{
		if (selected == PLAY) Director::getInstance()->replaceScene(Level1::createScene());
		if (selected == HELP)
		{
			flipHelpText(map, helpScreen, instructionsText, controlsText);
		}
	}

	if (controller->isEscapePressed())
	{
		if (map->isVisible()) Director::getInstance()->end();
		if (helpScreen->isVisible())
		{
			flipHelpText(map, helpScreen, instructionsText, controlsText);
		}
	}
}

void FinalProject::flipHelpText(TMXTiledMap* map, TMXTiledMap* help, Label* instructions, Label* controls)
{
	map->setVisible(!map->isVisible());
	help->setVisible(!help->isVisible());
	instructions->setVisible(!instructions->isVisible());
	controls->setVisible(!controls->isVisible());
}

void FinalProject::update(float dt)
{
	menuControls();
}