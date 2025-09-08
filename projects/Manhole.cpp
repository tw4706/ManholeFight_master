#include "Manhole.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	constexpr int kManholeGraphWidth = 48;
	constexpr int kManholeGraphHeight = 48;
	// マンホールの当たり判定用の座標
	constexpr int kManholeX1 = 220; // 左マンホールのX座標
	constexpr int kManholeY1 = 460; // 左マンホールのY座標
	constexpr int kManholeX2 = 1060; // 右マンホールのX座標
	constexpr int kManholeY2 = 460; // 右マンホールのY座標
	// 危険を知らせる距離
	constexpr float kWarningDistance = 150.0f;
	// 調整用の距離
	constexpr int kLeft = 96;	// 横幅
	constexpr int kTop = 80;	// 下からの距離
	constexpr int kRight = 64;	// 横幅
	constexpr int kBottom = 240;// 上からの距離
}

Manhole::Manhole():
	m_handle1(-1),
	m_handle2(-1),
	m_leftTriggered(false), 
	m_rightTriggered(false),
	m_warningHandle(-1),
	m_blinkTimer(0.0f),
	m_blinkFlag(false),
	m_dangerSoundHandle(-1),
	m_showLeftWarning(false),
	m_showRightWarning(false),
	m_leftPlayed(false),
	m_rightPlayed(false),
	m_stopSoundFlag(false)
{
}

Manhole::~Manhole()
{
}

void Manhole::Init(int _handle1,int _handle2)
{
	m_handle1 = _handle1;
	m_handle2 = _handle2;
	m_leftRect.init(kManholeX1, kManholeY1, kManholeGraphWidth+20, kManholeGraphHeight);
	m_rightRect.init(kManholeX2, kManholeY2, kManholeGraphWidth+20, kManholeGraphHeight);
	m_warningHandle = LoadGraph("data/UI/Warning.png");
	m_dangerSoundHandle = LoadSoundMem("data/BGM・SE/warning.mp3");
	ChangeVolumeSoundMem(180, m_dangerSoundHandle);
	m_blinkFlag = false;
	m_blinkTimer = 0;
	m_showLeftWarning = false;
	m_showRightWarning = false;
	m_leftPlayed = false;
	m_rightPlayed = false;
}

void Manhole::End()
{


}

void Manhole::Update(const Vec2&_player1Pos, const Vec2& _player2Pos)
{
	Vec2 leftCenter = m_leftRect.GetCenter();
	float LeftDist = sqrt(pow(_player1Pos.x - leftCenter.x, 2) + 
		pow(_player1Pos.y - leftCenter.y, 2));

	Vec2 rightCenter = m_rightRect.GetCenter();
	float RightDist = sqrt(pow(_player2Pos.x - rightCenter.x, 2) + 
		pow(_player2Pos.y - rightCenter.y, 2));

	// マンホールに使づいているかどうかの判定
	bool isNearLeft = LeftDist < kWarningDistance;
	bool isNearRight = RightDist < kWarningDistance;

	// 点滅用タイマーの更新
	m_blinkTimer++;

	// 点滅フラグ
	bool isBlink = (m_blinkTimer % 30 < 15);

	// 左右表示の更新
	m_showLeftWarning = isNearLeft && isBlink;
	m_showRightWarning = isNearRight && isBlink;

	// 左右表示が両方遠い場合はタイマーをリセットする
	if (!isNearLeft && !isNearRight)
	{
		m_blinkTimer = 0;
		m_showLeftWarning = false;
		m_showRightWarning = false;
	}

}

void Manhole::Draw()
{
	DrawExtendGraph(
		static_cast<int>(m_leftRect.m_left)-20,
		static_cast<int>(m_leftRect.m_top),
		static_cast<int>(m_leftRect.m_right)+20,
		static_cast<int>(m_leftRect.m_bottom),
		m_handle1, TRUE);
	//printfDx("pos.x=%f,pos.y=%f\n", pos.x, pos.y);

	DrawExtendGraph(
		static_cast<int>(m_rightRect.m_left)-20,
		static_cast<int>(m_rightRect.m_top),
		static_cast<int>(m_rightRect.m_right)+20,
		static_cast<int>(m_rightRect.m_bottom),
		m_handle2, TRUE);

	//printfDx("pos.x=%f,pos.y=%f\n", pos.x, pos.y);

	if (!m_stopSoundFlag&&m_warningHandle != -1)
	{
		if (m_showLeftWarning)
		{
			Vec2 leftCenter = m_leftRect.GetCenter();
			DrawExtendGraph(
				static_cast<int>(leftCenter.x) - kLeft,
				static_cast<int>(leftCenter.y) + kTop,
				static_cast<int>(leftCenter.x) + kRight,
				static_cast<int>(leftCenter.y) + kBottom,
				m_warningHandle, TRUE);
			//printfDx("左警告表示中\n");

			if (!m_leftPlayed)
			{
				PlaySoundMem(m_dangerSoundHandle, DX_PLAYTYPE_BACK);
				m_leftPlayed = true;
			}
		}
		else
		{
			m_leftPlayed = false; // 表示されていないときはリセット
		}

		if (m_showRightWarning)
		{
			Vec2 rightCenter = m_rightRect.GetCenter();
			DrawExtendGraph(
				static_cast<int>(rightCenter.x) - kLeft,
				static_cast<int>(rightCenter.y) + kTop,
				static_cast<int>(rightCenter.x) + kRight,
				static_cast<int>(rightCenter.y) + kBottom,
				m_warningHandle, TRUE);
			//printfDx("右警告表示中\n");
			if (!m_rightPlayed)
			{
				PlaySoundMem(m_dangerSoundHandle, DX_PLAYTYPE_BACK);
				m_rightPlayed = true;
			}
		}
		else
		{
			m_rightPlayed = false; // 表示されていないときはリセット
		}
	}

#ifdef _DEBUG
	// 左マンホールの当たり判定
	if (!m_leftTriggered)
	{
		m_leftRect.Draw(0xFF0000, false); // 赤色で枠線表示
	}

	// 右マンホールの当たり判定
	if (!m_rightTriggered)
	{
		m_rightRect.Draw(0x0000FF, false); // 青色で枠線表示
	}
#endif
}

bool Manhole::CheckLeftCollision(const Rect& playerRect)
{
	if (!m_leftTriggered && m_leftRect.IsCollision(playerRect))
	{
		m_leftTriggered = true;
		return true;
	}
	return false;
}

bool Manhole::CheckRightCollision(const Rect& playerRect)
{
	if (!m_rightTriggered && m_rightRect.IsCollision(playerRect))
	{
		m_rightTriggered = true;
		return true;
	}

	return false;
}

Vec2 Manhole::GetCenter()const
{
	// 時間切れの際の距離判定に使用
	Vec2 leftCenter = m_leftRect.GetCenter();
	Vec2 rightCenter = m_rightRect.GetCenter();
	return Vec2{ (leftCenter.x + rightCenter.x) / 2.0f, (leftCenter.y + rightCenter.y) / 2.0f };
}

void Manhole::SetGameOver(bool _isOver)
{
	m_stopSoundFlag = true;
	StopSoundMem(m_dangerSoundHandle);
}
