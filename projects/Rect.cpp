#include "Rect.h"
#include "DxLib.h"
#include<cassert>

Rect::Rect():
	m_left(0.0f),
	m_top(0.0f),
	m_right(0.0f),
	m_bottom(0.0f)
{
}

Rect::~Rect()
{
}

void Rect::init(float _left, float _top, float _width, float _height)
{
	m_left = _left;
	m_top = _top;
	m_right = _left + _width;
	m_bottom = _top + _height;
}

void Rect::Draw(unsigned int color,bool isFill)
{
	DrawBox(static_cast<int>(m_left), static_cast<int>(m_top),
		static_cast<int>(m_right), static_cast<int>(m_bottom),
		color,isFill);
}

void Rect::SetCenter(float x, float y, float width, float height)
{
	m_left = x - width / 2.0f;
	m_top = y - height / 2.0f;
	m_right = x + width / 2.0f;
	m_bottom = y + height / 2.0f;
}

float Rect::GetWidth() const
{
	// 左右の座標の入れ替わりチェック
	assert(m_right >= m_left);
	return m_right - m_left;
}

float Rect::GetHeight() const
{
	// 上下の座標の入れ替わりチェック
	assert(m_bottom >= m_top);
	return m_bottom - m_top;
}

Vec2 Rect::GetCenter() const
{
	// 中心座標
	float x = (m_left + m_right) / 2.0f;
	float y = (m_top + m_bottom) / 2.0f;
	return Vec2{ x,y };
}

bool Rect::IsCollision(const Rect& rect)
{
	// 絶対にあたらないパターンをはじいていく
	if (m_left > rect.m_right)return false;
	if (m_top > rect.m_bottom)return false;
	if (m_right < rect.m_left)return false;
	if (m_bottom < rect.m_top)return false;

	// それ以外は当たっている
	return true;
}