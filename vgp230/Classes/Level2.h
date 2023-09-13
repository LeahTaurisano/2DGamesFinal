#include "Level1.h"

class Level2 : public Level1
{

public:

	static Scene* createScene();

	virtual bool init();

	virtual void menuControls();

	virtual void changeLevel();

	virtual void update(float dt);

	CREATE_FUNC(Level2);
};