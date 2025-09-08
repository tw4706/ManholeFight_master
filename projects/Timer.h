#pragma once
class Timer
{
public:
	Timer();
	~Timer();

	void Init(float _maxTime);
	void End();
	void Update(float _deltaTime);
	void Draw(int _x, int _y)const;

	// タイマーの開始と停止
	void Start();
	void Stop();
	// タイマーのリセット
	void Reset();

	// タイマーの設定
	int GetTime() const { return static_cast<int>(m_time); }
	bool IsTimeUp()const { return m_time <= 0.0f; }
	bool IsRunning() const { return m_isRunning; }

private:
	// 残り時間
	float m_time;
	// 最大時間
	float m_maxTime;
	// タイマーが動いているか
	bool m_isRunning;
	// タイマーのハンドル
	int m_timerHandle;
};

