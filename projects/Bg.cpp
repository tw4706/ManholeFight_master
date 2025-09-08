#include "Bg.h"
#include "DxLib.h"
#include"Game.h"
#include "Player.h"

namespace 
{
	constexpr int kMapHeight = 600;
}

Bg::Bg() :
	m_mapHandle(-1),
	m_bgHandle(-1)
{
	
}

Bg::~Bg()
{
}

void Bg::Init()
{
	m_mapHandle = LoadGraph("data/BG/mapChip.png");
	m_bgHandle = LoadGraph("data/UI/title.png");
}

void Bg::End()
{
	DeleteGraph(m_mapHandle);
	DeleteGraph(m_bgHandle);
}

void Bg::Update()
{

}

void Bg::Draw()
{
	DrawExtendGraph(0, 0, Game::kScreenWidth, kMapHeight, m_bgHandle, true);
	DrawExtendGraph(0, 0, Game::kScreenWidth, kMapHeight, m_mapHandle, true);
}


