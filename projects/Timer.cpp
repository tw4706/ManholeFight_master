#include "Timer.h"
#include "DxLib.h"

namespace
{
	constexpr float kScale = 1.3f; // タイマー画像の拡大率
}

Timer::Timer():
	m_time(0.0f),
	m_maxTime(0.0f),
	m_isRunning(false),
	m_timerHandle(-1)
{
}

Timer::~Timer()
{
}

void Timer::Init(float _maxTime)
{
	m_timerHandle = LoadGraph("data/UI/Timer.png");
	m_maxTime = _maxTime;
	Reset();
}

void Timer::End()
{

}

void Timer::Update(float _deltaTime)
{
	if (m_isRunning && m_time > 0.0f)
	{
		m_time-= _deltaTime;
		if (m_time < 0.0f)
		{
			m_time = 0.0f;
			// タイマーが0になったら停止
			m_isRunning = false;
		}
	}
}
void Timer::Draw(int _x, int _y) const
{
	SetFontSize(36);
	ChangeFont("Msゴシック");
	int color = GetColor(255, 255, 0);
	// タイマーの残り時間を整数に変換
	char buffer[32];
	sprintf_s(buffer, "%d", static_cast<int>(m_time));
	DrawFormatString(_x+10, _y+60, color, buffer);

	DrawExtendGraph(_x-140, _y-40,_x*kScale,_y*kScale, m_timerHandle, true);
}

void Timer::Start()
{
	m_isRunning = true;
}

void Timer::Stop()
{
	m_isRunning = false;
}

void Timer::Reset()
{
	m_time = m_maxTime;
	m_isRunning = false;
}
