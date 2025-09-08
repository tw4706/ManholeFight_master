#include "Title.h"
#include"DxLib.h"
#include"Game.h"
#include<cmath>

namespace
{
    // 拡大率
    constexpr float kScaleX = 0.3f;
    constexpr float kScaleY = 0.28f;

    // タイトルロゴ・文字のサイズ
    constexpr int kDrawWidth = 600;
    constexpr int kDrawHeight = 400;
    // 描画の調整用定数
    constexpr int kStrOffsetY = 80;
}

Title::Title() :
    m_titleGraphHandle(-1),
	m_titleLogoHandle(-1),
    m_titleStrHandle(-1),
    m_bgmHandle(-1),
	m_pushHandle(-1)
{
}

Title::~Title()
{
}

void Title::Init()
{
    m_titleGraphHandle = LoadGraph("data/UI/title_1.png");
	m_titleLogoHandle = LoadGraph("data/UI/titleL.png");
    m_titleStrHandle = LoadGraph("data/UI/str.png");
    m_bgmHandle = LoadSoundMem("data/BGM・SE/title.mp3");
    if (!CheckSoundMem(m_bgmHandle)) // 再生されていないなら
    {
        PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
    }
    ChangeVolumeSoundMem(150, m_bgmHandle);
}

void Title::End()
{
    DeleteGraph(m_titleGraphHandle);
    DeleteGraph(m_titleLogoHandle);
    DeleteGraph(m_titleStrHandle);
    StopSoundMem(m_bgmHandle);
    DeleteSoundMem(m_bgmHandle);
}

void Title::Update()
{
}

void Title::Draw()
{
    int drawX = (Game::kScreenWidth - kDrawWidth) / 2;
    int drawY = (Game::kScreenHeight -kDrawHeight) / 2-50;

    int strWidth, strHeight;
    GetGraphSize(m_titleStrHandle, &strWidth, &strHeight);

    int strDrawWidth = static_cast<int>(strWidth * kScaleX);
    int strDrawHeight = static_cast<int>(strHeight * kScaleY);

    // X座標は中央
    int strX = (Game::kScreenWidth - strDrawWidth) / 2;
    // Y座標は中央より下
    int strY = (Game::kScreenHeight - strDrawHeight) / 2 + 150;

    DrawGraph(0,0, m_titleGraphHandle, true);
    // ロゴが画面中央に来るように配置
	DrawExtendGraph(drawX, drawY, drawX + kDrawWidth, 
        drawY + kDrawHeight,m_titleLogoHandle, true);
    // タイトル文字の描画とフェード
    int alpha = 128 + 
        static_cast<int>(127 * sin(GetNowCount() / 100.0));
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);    // アルファの値を周期的に変化させて、点滅風に見せる
    DrawRectExtendGraphF(
        strX, strY,
        strX + strDrawWidth, strY + strDrawHeight+30,
        0, 0, strWidth, strHeight,
        m_titleStrHandle, true
    );
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
