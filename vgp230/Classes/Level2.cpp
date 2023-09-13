#include "Level2.h"
#include "FinalProject.h"
#include "CollisionComponent.h"

Scene* Level2::createScene()
{
	return Level2::create();
}

bool Level2::init()
{
	map = TMXTiledMap::create("tmx/Level2.tmx");
	initialize(map);

	scheduleUpdate();
	gameState = RUNNING;

	return true;
}

void Level2::menuControls()
{
	menuInput();
	Level2::changeLevel();
}

void Level2::changeLevel()
{
	if (controller->isEnterPressed() && gameState == LEVEL_END) Director::getInstance()->replaceScene(FinalProject::createScene());
}

void Level2::update(float dt)
{
	debug->clear();
	if (gameState == RUNNING)
	{
		timer += dt;
		timerLabel->setString(std::to_string((int)timer));
		if (showDebug)
		{
			debugDraw(player1, drawColor1);
			debugDraw(player2, drawColor2);
		}
		playerMovement(dt, player1, player1Pos, controller, drawColor1, player1End);
		playerMovement(dt, player2, player2Pos, controller2, drawColor2, player2End);
		Level2::menuControls();
	}
	if (gameState == PAUSE)
	{
		Level2::menuControls();
	}
	if (gameState == LEVEL_END)
	{
		Level2::menuControls();
	}
}