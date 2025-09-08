#pragma once
#include"Vec2.h"
class Rect
{
public:
	Rect();
	~Rect();

	void init(float _left,float _top,float _width,float _height);
	void End();
	void Update();
	void Draw(unsigned int Color,bool isFill);

	// 中心座標と幅高さを指定
	void SetCenter(float x, float y, float width, float height);
	// 矩形の幅を取得
	float GetWidth()const;
	// 矩形の高さを取得
	float GetHeight()const;
	// 矩形の中心座標を取得
	Vec2 GetCenter() const;
	// 矩形の左上のX座標を取得
	float GetLeft()const { return m_left; }
	// 矩形の左上のY座標を取得
	float GetTop()const { return m_top; }
	// 矩形の右下のX座標を取得
	float GetRight()const { return m_right; }
	// 矩形の右下のY座標を取得
	float GetBottom()const { return m_bottom; }
	// 矩形の当たり判定を取得
	bool IsCollision(const Rect& rect);

public:
	float m_left;   // 矩形の左上のX座標	
	float m_top;    // 矩形の左上のY座標
	float m_right;  // 矩形の右下のX座標
	float m_bottom; // 矩形の右下のY座標


private:


};

