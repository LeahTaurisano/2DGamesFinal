#include "Level1.h"
#include "FinalProject.h"
#include "CollisionComponent.h"
#include "Level2.h"

Scene* Level1::createScene()
{
	return Level1::create();
}

bool Level1::init()
{
	map = TMXTiledMap::create("tmx/Level1.tmx");
	initialize(map);

	scheduleUpdate();
	gameState = RUNNING;

	return true;
}

void Level1::initialize(TMXTiledMap* level)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	path = level->getLayer("Path");
	auto collision2 = level->getLayer("Collision2");
	auto collision = level->getLayer("Collision");
	player1StartLayer = level->getLayer("Player1 Start");
	player2StartLayer = level->getLayer("Player2 Start");
	playerGoalLayer = level->getLayer("Player End");
	keyLayer = level->getLayer("Key");
	gatesLayer = level->getLayer("Gates");
	blueGatesOn = level->getLayer("BlueGates1");
	blueGatesOff = level->getLayer("BlueGates2");
	blueSwitch = level->getLayer("BlueSwitch");
	redGatesOn = level->getLayer("RedGates1");
	redGatesOff = level->getLayer("RedGates2");
	redSwitch = level->getLayer("RedSwitch");
	dualGate1Red = level->getLayer("DualGate1Red");
	dualGate1Blue = level->getLayer("DualGate1Blue");
	dualSwitch1Red = level->getLayer("DualSwitch1Red");
	dualSwitch1Blue = level->getLayer("DualSwitch1Blue");
	dualGate2Red = level->getLayer("DualGate2Red");
	dualGate2Blue = level->getLayer("DualGate2Blue");
	dualSwitch2Red = level->getLayer("DualSwitch2Red");
	dualSwitch2Blue = level->getLayer("DualSwitch2Blue");

	pause = TMXTiledMap::create("tmx/PauseMenu.tmx");
	returnOption = pause->getLayer("ReturnToGame");
	quitOption = pause->getLayer("Quit");
	quitOption->setVisible(false);

	levelClear = TMXTiledMap::create("tmx/LevelClear.tmx");

	mapSize = &path->getLayerSize();
	tileSize = &path->getMapTileSize();

	level->setScale(1.5f);
	pause->setScale(1.1f);
	levelClear->setScale(1.1f);
	pause->setVisible(false);
	levelClear->setVisible(false);

	this->addChild(level);
	this->addChild(pause);
	this->addChild(levelClear);

	player1StartLayer->setVisible(false);
	player2StartLayer->setVisible(false);
	playerGoalLayer->setVisible(false);
	redGatesOff->setVisible(false);
	blueGatesOff->setVisible(false);

	player1 = Sprite::create("BluePlayer.png");
	player1->addComponent(CollisionComponent::createBox(player1->getContentSize().height, player1->getContentSize().width));
	this->addChild(player1, 0);
	setStartPosition(player1, player1StartLayer, player1Pos);

	player2 = Sprite::create("RedPlayer.png");
	player2->addComponent(CollisionComponent::createBox(player2->getContentSize().height, player2->getContentSize().width));
	this->addChild(player2, 0);
	setStartPosition(player2, player2StartLayer, player2Pos);

	controller = KeyboardControllerComponent::create(KeyboardControllerComponent::ARROWS);
	player1->addComponent(controller);
	controller->initInput();

	controller2 = KeyboardControllerComponent::create(KeyboardControllerComponent::WASD);
	player2->addComponent(controller2);
	controller2->initInput();

	timerLabel = Label::createWithSystemFont(std::to_string((int)timer), "arial.ttf", 20.f);
	timerLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.03f));
	this->addChild(timerLabel, 0);
	timerLabel->setVisible(true);

	scoreText = Label::createWithSystemFont("You cleared the level in " + std::to_string((int)timer) + " seconds!\n\n\n\n\t\t\tPress Enter to go to the next level!", "arial.ttf", 30.f);
	scoreText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
	this->addChild(scoreText, 0);
	scoreText->setVisible(false);

	dualGateOpen1 = ParticleSystemQuad::create("Particle/DualGateOpen.plist");
	dualGateOpen1->setScale(0.5f);
	setParticlePosition(dualGateOpen1, dualGate1Blue, 0);
	dualGateOpen1->setVisible(false);
	this->addChild(dualGateOpen1, 2);

	dualGateOpen2 = ParticleSystemQuad::create("Particle/DualGateOpen.plist");
	dualGateOpen2->setScale(0.5f);
	setParticlePosition(dualGateOpen2, dualGate2Red, 0);
	dualGateOpen2->setVisible(false);
	this->addChild(dualGateOpen2, 2);

	endGate = ParticleSystemQuad::create("Particle/EndGate.plist");
	endGate->setScale(0.2f);
	setParticlePosition(endGate, playerGoalLayer, (tileSize->width * 1.5f / 2.f));
	this->addChild(endGate, 2);

	debug = DrawNode::create(5.f);
	this->addChild(debug, 5);
}

void Level1::changeMenuVisible(TMXLayer* option)
{
	option->setVisible(!option->isVisible());
}

void Level1::setStartPosition(Sprite* sprite, TMXLayer* layer, Vec2& newPosition)
{
	for (int i = 0; i < layer->getLayerSize().width; ++i)
	{
		for (int j = 0; j < layer->getLayerSize().height; ++j)
		{
			if (layer->getTileAt(Vec2(i, j)) != NULL)
			{
				sprite->setPosition(Vec2(tileSize->width * 1.5f * i + (tileSize->width * 1.5f / 2.f), tileSize->height * 1.5f * (mapSize->height - j - 1.f) + (tileSize->height * 1.5f / 2.f)));
				newPosition.x = i;
				newPosition.y = j;
			}
		}
	}
}

void Level1::setParticlePosition(ParticleSystemQuad* particle, TMXLayer* layer, float variance)
{
	for (int i = 0; i < layer->getLayerSize().width; ++i)
	{
		for (int j = 0; j < layer->getLayerSize().height; ++j)
		{
			if (layer->getTileAt(Vec2(i, j)) != NULL)
			{
				particle->setPosition(Vec2(tileSize->width * 1.5f * i + variance, tileSize->height * 1.5f * (mapSize->height - j - 1.f) + (tileSize->height * 1.5f / 2.f)));
			}
		}
	}
}

bool Level1::canMoveTo(Vec2 position, Vec2 currPos, Color4F& drawColor)
{
    if (position.x < 0 || position.x >= path->getLayerSize().width) return false;
	if (position.y < 0 || position.y >= path->getLayerSize().height) return false;
	if (path->getTileAt(position) == NULL) return false;
	if (gatesLayer->getTileAt(position) != NULL && gatesLayer->isVisible()) return false;
	if (blueGatesOn->getTileAt(position) != NULL && blueGatesOn->isVisible()) return false;
	if (blueGatesOn->isVisible() && blueGatesOn->getTileAt(currPos) != NULL) return false;
	if (blueGatesOff->getTileAt(position) != NULL && blueGatesOff->isVisible()) return false;
	if (blueGatesOff->isVisible() && blueGatesOff->getTileAt(currPos) != NULL) return false;
	if (redGatesOn->getTileAt(position) != NULL && redGatesOn->isVisible()) return false;
	if (redGatesOn->isVisible() && redGatesOn->getTileAt(currPos) != NULL) return false;
	if (redGatesOff->getTileAt(position) != NULL && redGatesOff->isVisible()) return false;
	if (redGatesOff->isVisible() && redGatesOff->getTileAt(currPos) != NULL) return false;
	if (dualGate1Red->getTileAt(position) != NULL && dualGate1Red->isVisible() || dualGate1Blue->getTileAt(position) != NULL && dualGate1Blue->isVisible()) return false;
	if (dualGate2Red->getTileAt(position) != NULL && dualGate2Red->isVisible() || dualGate2Blue->getTileAt(position) != NULL && dualGate2Blue->isVisible()) return false;
	if ((position == player1Pos && player1->isVisible()) || (position == player2Pos && player2->isVisible())) return false;

	drawColor = Color4F::GREEN;
	return true;
}

void Level1::switchGates(TMXLayer* button, TMXLayer* openGate, TMXLayer* closedGate, Vec2 pos, Sprite* active, Sprite* player)
{
	if (button->getTileAt(pos) != NULL && active == player)
	{
		openGate->setVisible(false);
		closedGate->setVisible(true);
	}
	if (button->getTileAt(pos) == NULL && active == player)
	{
		openGate->setVisible(true);
		closedGate->setVisible(false);
	}
}

void Level1::dualSwitchGates(TMXLayer* button, TMXLayer* redGate, TMXLayer* blueGate, bool& switchCheck, bool otherSwitch, Vec2 pos, Sprite* active, Sprite* player, ParticleSystemQuad* particle)
{
	if (button->getTileAt(pos) != NULL && active == player)
	{
		switchCheck = true;
		if (switchCheck && otherSwitch)
		{
			if (redGate->isVisible() && blueGate->isVisible())
			{
				particle->resetSystem();
				particle->setVisible(true);
			}
			redGate->setVisible(false);
			blueGate->setVisible(false);
		}
	}
	if (button->getTileAt(pos) == NULL && active == player) switchCheck = false;
}

void Level1::switchMap(TMXTiledMap* current, TMXTiledMap* next)
{
	current->setVisible(!current->isVisible());
	next->setVisible(!next->isVisible());
}

void Level1::playerMovement(float dt, Sprite* active, Vec2& pos, KeyboardControllerComponent* contr, Color4F& drawColor, bool& playerEnd)
{
	if (active->isVisible())
	{
		if (contr->IsLeftPressed())
		{
			auto position = active->getPosition() - Vec2(tileSize->width * 1.5, 0.0f);
			auto newPos = Vec2(pos.x - 1, pos.y);
			if (canMoveTo(newPos, pos, drawColor))
			{
				active->setPosition(position);
				pos.x -= 1;
			}
			else (drawColor = Color4F::RED);
		}
		if (contr->IsRightPressed())
		{
			auto position = active->getPosition() + Vec2(tileSize->width * 1.5, 0.0f);
			auto newPos = Vec2(pos.x + 1, pos.y);
			if (canMoveTo(newPos, pos, drawColor))
			{
				active->setPosition(position);
				pos.x += 1;
			}
			else (drawColor = Color4F::RED);
		}
		if (contr->IsUpPressed())
		{
			auto position = active->getPosition() + Vec2(0.0f, tileSize->height * 1.5);
			auto newPos = Vec2(pos.x, pos.y - 1);
			if (canMoveTo(newPos, pos, drawColor))
			{
				active->setPosition(position);
				pos.y -= 1;
			}
			else (drawColor = Color4F::RED);
		}
		if (contr->IsDownPressed())
		{
			auto position = active->getPosition() - Vec2(0.0f, tileSize->height * 1.5);
			auto newPos = Vec2(pos.x, pos.y + 1);
			if (canMoveTo(newPos, pos, drawColor))
			{
				active->setPosition(position);
				pos.y += 1;
			}
			else (drawColor = Color4F::RED);
		}
		if (contr->isRPressed())
		{
			setStartPosition(player1, player1StartLayer, player1Pos);
			setStartPosition(player2, player2StartLayer, player2Pos);
			player1->setVisible(true);
			player2->setVisible(true);
			player1End = false;
			player2End = false;
			keyLayer->setVisible(true);
			gatesLayer->setVisible(true);
		}
		if (keyLayer->getTileAt(pos) != NULL)
		{
			keyLayer->setVisible(false);
			gatesLayer->setVisible(false);
		}

		if (playerGoalLayer->getTileAt(pos) != NULL)
		{
			playerEnd = true;
			active->setVisible(false);
		}

		switchGates(blueSwitch, blueGatesOn, blueGatesOff, pos, active, player1);
		switchGates(redSwitch, redGatesOn, redGatesOff, pos, active, player2);

		dualSwitchGates(dualSwitch1Red, dualGate1Red, dualGate1Blue, dualRed1Check, dualBlue1Check, pos, active, player2, dualGateOpen1);
		dualSwitchGates(dualSwitch1Blue, dualGate1Red, dualGate1Blue, dualBlue1Check, dualRed1Check, pos, active, player1, dualGateOpen1);
		dualSwitchGates(dualSwitch2Red, dualGate2Red, dualGate2Blue, dualRed2Check, dualBlue2Check, pos, active, player2, dualGateOpen2);
		dualSwitchGates(dualSwitch2Blue, dualGate2Red, dualGate2Blue, dualBlue2Check, dualRed2Check, pos, active, player1, dualGateOpen2);

		checkLevelEnd(player1End, player2End);
	}
}

void Level1::menuControls()
{
	menuInput();
	changeLevel();
}

void Level1::menuInput()
{
	if (controller->isEscapePressed() && gameState == RUNNING)
	{
		gameState = PAUSE;
		switchMap(map, pause);
		player1->setVisible(false);
		player2->setVisible(false);
		endGate->setVisible(false);
	}
	if (controller->isTildePressed())
	{
		showDebug = !showDebug;
	}
	if (gameState == PAUSE)
	{
		if (controller->IsUpPressed())
		{
			if (selected == RETURN) selected = QUIT;
			else selected = RETURN;
			changeMenuVisible(returnOption);
			changeMenuVisible(quitOption);
		}

		if (controller->IsDownPressed())
		{
			if (selected == RETURN) selected = QUIT;
			else selected = RETURN;
			changeMenuVisible(returnOption);
			changeMenuVisible(quitOption);
		}

		if (controller->isEnterPressed())
		{
			if (selected == RETURN)
			{
				map->setVisible(true);
				pause->setVisible(false);
				player1->setVisible(true);
				player2->setVisible(true);
				endGate->setVisible(true);
				gameState = RUNNING;
			}
			if (selected == QUIT) Director::getInstance()->replaceScene(FinalProject::createScene());
		}
	}
}

void Level1::checkLevelEnd(bool p1End, bool p2End)
{
	if (p1End && p2End)
	{
		gameState = LEVEL_END;
		switchMap(map, levelClear);
		endGate->setVisible(false);
		scoreText->setString("You cleared the level in " + std::to_string((int)timer) + " seconds!\n\n\n\n\t\t\tPress Enter to go to the next level!");
		scoreText->setVisible(true);
		timerLabel->setVisible(false);
	}
}

void Level1::debugDraw(Sprite* player, Color4F drawColor)
{
	auto collision = dynamic_cast<CollisionComponent*>(player->getComponent("CollisionComponent"));
	auto pos = player->getPosition();
	auto colW = collision->GetWidth() / 2.0f;
	auto colH = collision->GetHeight() / 2.0f;
	debug->drawRect(Vec2(pos.x - colW, pos.y + colH), Vec2(pos.x + colW, pos.y + colH), Vec2(pos.x + colW, pos.y - colH), Vec2(pos.x - colW, pos.y - colH), drawColor);
}

void Level1::changeLevel()
{
	if (controller->isEnterPressed() && gameState == LEVEL_END) Director::getInstance()->replaceScene(Level2::createScene());
}

void Level1::update(float dt)
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
		menuControls();
	}
	if (gameState == PAUSE)
	{
		menuControls();
	}
	if (gameState == LEVEL_END)
	{
		menuControls();
	}
}