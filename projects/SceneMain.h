#pragma once
#include"Player.h"
#include"Bg.h"
#include"Manhole.h"
#include"Timer.h"

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();

	// ゲーム内BGMをストップ
	void StopBGM();

private:
	// プレイヤーのグラフィックハンドル
	int m_player1GraphHandle; 
	int m_player2GraphHandle;
	// プレイヤーの攻撃グラフィックハンドル
	int m_player1AttackGraphHandle; 
	int m_player2AttackGraphHandle;
	// プレイヤーの弱攻撃グラフィックハンドル
	int m_player1WeakAttackGraphHandle;
	int m_player2WeakAttackGraphHandle;
	// プレイヤーの走るグラフィックハンドル
	int m_player1RunGraphHandle;	
	int m_player2RunGraphHandle;
	// プレイヤーの攻撃を受けた時のハンドル
	int m_player1HurtGraphHandle;
	int m_player2HurtGraphHandle;
	// プレイヤーの落下中のグラフィックハンドル
	int m_player1FallGraphHandle;
	int m_player2FallGraphHandle;
	// プレイヤーの勝利ポーズのグラフィックハンドル
	int m_player1WinGraphHandle;
	int m_player2WinGraphHandle;
	// 時間切れのグラフィックハンドル
	int m_timeUp1GraphHandle;
	int m_timeUp2GraphHandle;
	// マンホールのグラフィックハンドル
	int m_manhole1GraphHandle;
	int m_manhole2GraphHandle;
	// 勝利したプレイヤーのハンドル
	int m_winPlayer1GraphHandle;
	int m_winPlayer2GraphHandle;
	// ゲーム画面の時間をカウントする変数
	int m_timer;
	// ゲームスタートの時間をカウントする変数
	int m_startTimer;
	// ゲームスタートのシーケンス
	bool m_isStartSeq;
	// ゲーム開始合図のサウンド
	int m_gameStartSoundHandle;
	// ゲーム開始のUIハンドル
	int m_gameStartUI1Handle;
	int m_gameStartUI2Handle;
	// ゲームオーバーのフラグ
	bool m_gameOver;
	// 勝利したプレイヤーのフラグ
	bool m_player1WinFlag;
	bool m_player2WinFlag;
	// BGMのハンドル
	int m_bgmHandle;
	// 勝利時のBGMハンドル
	int m_winBgmHandle;
	// ゲームオーバーのサウンド
	int m_gameOverBgHandle;
	// BGMが終わったかどうか
	bool m_bgmEndFlag;

	// プレイヤー
	Player*m_player1;
	Player*m_player2;
	// 背景
	Bg* m_Bg;
	// マンホール
	Manhole* m_manhole;
	// タイマー
	Timer*m_roundTimer;
};