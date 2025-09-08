#pragma once
#include "Player.h"
#include"Vec2.h"

class Bg
{
public:
	Bg();
	~Bg();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	// グラフィックのハンドル
	int m_mapHandle;
	// バックの背景のグラフィックハンドル
	int m_bgHandle;
};

