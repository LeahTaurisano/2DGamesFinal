#pragma once
#include "cocos2d.h"
#include "KeyboardControllerComponent.h"

USING_NS_CC;

class Level1 : public Scene
{

public:

	static Scene* createScene();

	virtual bool init();

	void initialize(TMXTiledMap* level);

	void changeMenuVisible(TMXLayer* option);

	void playerMovement(float dt, Sprite* active, Vec2& pos, KeyboardControllerComponent* cont, Color4F& drawColor, bool& playerEnd);

	void setStartPosition(Sprite* sprite, TMXLayer* layer, Vec2& newPosition);

	void setParticlePosition(ParticleSystemQuad* particle, TMXLayer* layer, float variance);

	bool canMoveTo(Vec2 position, Vec2 currPos, Color4F& drawColor);

	void switchGates(TMXLayer* button, TMXLayer* openGate, TMXLayer* closedGate, Vec2 pos, Sprite* active, Sprite* player);
	
	void dualSwitchGates(TMXLayer* button, TMXLayer* redGate, TMXLayer* blueGate, bool& switchCheck, bool otherSwitch, Vec2 pos, Sprite* active, Sprite* player, ParticleSystemQuad* particle);

	void switchMap(TMXTiledMap* current, TMXTiledMap* next);

	void menuControls();

	void menuInput();

	void checkLevelEnd(bool p1End, bool p2End);

	void debugDraw(Sprite* player, Color4F drawColor);

	void changeLevel();

	virtual void update(float dt);

	CREATE_FUNC(Level1);

protected: 

	Sprite* player1;
	Vec2 player1Pos;
	Sprite* player2;
	Vec2 player2Pos;

	DrawNode* debug;
	Color4F drawColor1 = Color4F::GREEN;
	Color4F drawColor2 = Color4F::GREEN;
	bool showDebug = false;

	TMXTiledMap* map;
	TMXTiledMap* pause;
	TMXTiledMap* levelClear;

	TMXLayer* path;
	TMXLayer* playerGoalLayer;
	TMXLayer* keyLayer;
	TMXLayer* gatesLayer;
	TMXLayer* redGatesOn;
	TMXLayer* redGatesOff;
	TMXLayer* redSwitch;
	TMXLayer* blueGatesOn;
	TMXLayer* blueGatesOff;
	TMXLayer* blueSwitch;
	TMXLayer* dualSwitch1Red;
	TMXLayer* dualSwitch1Blue;
	TMXLayer* dualGate1Red;
	TMXLayer* dualGate1Blue;
	TMXLayer* dualSwitch2Red;
	TMXLayer* dualSwitch2Blue;
	TMXLayer* dualGate2Red;
	TMXLayer* dualGate2Blue;
	TMXLayer* player1StartLayer;
	TMXLayer* player2StartLayer;

	TMXLayer* returnOption;
	TMXLayer* quitOption;

	Label* scoreText;

	ParticleSystemQuad* dualGateOpen1;
	ParticleSystemQuad* dualGateOpen2;
	ParticleSystemQuad* endGate;

	enum selectedMenu
	{
		RETURN,
		QUIT
	} selected = RETURN;

	bool dualRed1Check = false;
	bool dualBlue1Check = false;
	bool dualRed2Check = false;
	bool dualBlue2Check = false;

	bool player1End = false;
	bool player2End = false;

	KeyboardControllerComponent* controller;
	KeyboardControllerComponent* controller2;

	const Size* mapSize;
	const Size* tileSize;

	float timer = 0.0f;
	Label* timerLabel;

	enum gameState
	{
		START,
		RUNNING,
		PAUSE,
		LEVEL_END
	} gameState = START;
};