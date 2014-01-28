#pragma once


class GameEntity
{
public:
	GameEntity();
	~GameEntity();

	virtual void update() = 0;
	virtual void draw() = 0;
};

