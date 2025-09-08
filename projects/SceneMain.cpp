#include "SceneMain.h"
#include"Dxlib.h"
#include"Game.h"
#include"Rect.h"
#include "Bg.h"
#include<cmath>
#include "Manhole.h"

SceneMain::SceneMain():
	m_player1GraphHandle(-1),
	m_player2GraphHandle(-1),
	m_player1AttackGraphHandle(-1),
	m_player2AttackGraphHandle(-1),
	m_player1WeakAttackGraphHandle(-1),
	m_player2WeakAttackGraphHandle(-1),
	m_player1RunGraphHandle(-1),
	m_player2RunGraphHandle(-1),
	m_player1HurtGraphHandle(-1),
	m_player2HurtGraphHandle(-1),
	m_player1FallGraphHandle(-1),
	m_player2FallGraphHandle(-1),
	m_player1WinGraphHandle(-1),
	m_player2WinGraphHandle(-1),
	m_timeUp1GraphHandle(-1),
	m_timeUp2GraphHandle(-1),
	m_manhole1GraphHandle(-1),
	m_manhole2GraphHandle(-1),
	m_winPlayer1GraphHandle(-1),
	m_winPlayer2GraphHandle(-1),
	m_timer(0),
	m_startTimer(0),
	m_isStartSeq(false),
	m_gameStartSoundHandle(-1),
	m_gameStartUI1Handle(-1),
	m_gameStartUI2Handle(-1),
	m_gameOver(false),
	m_player1WinFlag(false),
	m_player2WinFlag(false),
	m_bgmHandle(-1),
	m_winBgmHandle(-1),
	m_gameOverBgHandle(-1),
	m_bgmEndFlag(false),
	m_player1(nullptr),
	m_player2(nullptr),
	m_Bg(nullptr),
	m_manhole(nullptr),
	m_roundTimer(nullptr)
{

}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// 現在の時間を取得
	m_timer = GetNowCount();
	m_gameOver = false;
	// プレイヤーのインスタンスを生成	
	m_player1 = new Player();	
	m_player2 = new Player();
	m_Bg = new Bg();
	m_manhole = new Manhole();
	m_roundTimer = new Timer();
	// ゲームスタートのカウントダウン用タイマーを初期化
	m_startTimer = 0;
	m_isStartSeq = true;
	// プレイヤーのゲームオーバーフラグを初期化
	m_gameOver = false;
	m_bgmEndFlag = false;
	// プレイヤーの勝利フラグを初期化
	m_player1WinFlag = false;
	m_player2WinFlag = false;
	//グラフィックの読み込み
	m_player1GraphHandle = LoadGraph("data/Player/Player1.idle.png");
	m_player2GraphHandle = LoadGraph("data/Player/Player2.idle.png");
	m_player1AttackGraphHandle = LoadGraph("data/Player/Player1.weak.png");
	m_player2AttackGraphHandle = LoadGraph("data/Player/Player2.attack.png");
	m_player1WeakAttackGraphHandle = LoadGraph("data/Player/Player1.attack.png");
	m_player2WeakAttackGraphHandle = LoadGraph("data/Player/Player2.weak.png");
	m_player1RunGraphHandle = LoadGraph("data/Player/Player1.run.png");
	m_player2RunGraphHandle = LoadGraph("data/Player/Player2.run.png");
	m_player1HurtGraphHandle = LoadGraph("data/Player/Player1.hurt.png");
	m_player2HurtGraphHandle = LoadGraph("data/Player/Player2.hurt.png");
	m_player1FallGraphHandle = LoadGraph("data/Player/Fall.png");
	m_player2FallGraphHandle = LoadGraph("data/Player/Fall1.png");
	m_player1WinGraphHandle = LoadGraph("data/Player/Win1.png");
	m_player2WinGraphHandle = LoadGraph("data/Player/Win2.png");
	m_timeUp1GraphHandle = LoadGraph("data/Player/death1.png");
	m_timeUp2GraphHandle = LoadGraph("data/Player/death2.png");
	m_manhole1GraphHandle = LoadGraph("data/UI/Manhole1.png");
	m_manhole2GraphHandle = LoadGraph("data/UI/Manhole2.png");
	m_winPlayer1GraphHandle = LoadGraph("data/UI/Win1.png");
	m_winPlayer2GraphHandle = LoadGraph("data/UI/Win2.png");
	m_gameStartUI1Handle = LoadGraph("data/UI/UI1.png");
	m_gameStartUI2Handle = LoadGraph("data/UI/UI2.png");
	//BGMの読み込みと再生
	m_gameStartSoundHandle = LoadSoundMem("data/BGM・SE/ReadyFight.mp3");
	m_bgmHandle = LoadSoundMem("data/BGM・SE/game.mp3");
	ChangeVolumeSoundMem(110, m_bgmHandle);          // 音量の調整
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);     // ループ再生
	m_winBgmHandle = LoadSoundMem("data/BGM・SE/win.mp3");
	m_gameOverBgHandle = LoadSoundMem("data/BGM・SE/gameOver.mp3");
	ChangeVolumeSoundMem(150, m_gameOverBgHandle);

	m_roundTimer->Init(30.0f);
	m_roundTimer->Reset();
	m_roundTimer->Stop(); // タイマーを一時停止
	m_player1->Init(DX_INPUT_PAD1,Vec2(400,480),m_player1GraphHandle,
		m_player1AttackGraphHandle, m_player1WeakAttackGraphHandle,
		m_player1RunGraphHandle,m_player1HurtGraphHandle,m_player1FallGraphHandle, m_player1WinGraphHandle, m_timeUp1GraphHandle,false);
	m_player2->Init(DX_INPUT_PAD2,Vec2(840, 480), m_player2GraphHandle,
		m_player2AttackGraphHandle, m_player2WeakAttackGraphHandle,
		m_player2RunGraphHandle, m_player2HurtGraphHandle, m_player2FallGraphHandle,m_player2WinGraphHandle,m_timeUp2GraphHandle,true);
	m_Bg->Init();
	m_manhole->Init(m_manhole1GraphHandle,m_manhole2GraphHandle);
}

void SceneMain::End()
{
	m_player1->End();	// プレイヤーの終了処理
	m_player2->End();
	m_Bg->End();		// 背景の終了処理
	m_manhole->End();	// マンホールの終了処理
	m_roundTimer->End();// タイマーの終了処理
	// グラフィックの解放
	DeleteGraph(m_player1GraphHandle);
	DeleteGraph(m_player2GraphHandle);
	DeleteGraph(m_player1AttackGraphHandle);
	DeleteGraph(m_player2AttackGraphHandle);
	DeleteGraph(m_player1WeakAttackGraphHandle);
	DeleteGraph(m_player2WeakAttackGraphHandle);
	DeleteGraph(m_player1RunGraphHandle);
	DeleteGraph(m_player2RunGraphHandle);
	DeleteGraph(m_player1HurtGraphHandle);
	DeleteGraph(m_player2HurtGraphHandle);
	DeleteGraph(m_player1FallGraphHandle);
	DeleteGraph(m_player2FallGraphHandle);
	DeleteGraph(m_player1WinGraphHandle);
	DeleteGraph(m_player2WinGraphHandle);
	DeleteGraph(m_timeUp1GraphHandle);
	DeleteGraph(m_timeUp2GraphHandle);
	DeleteGraph(m_manhole1GraphHandle);
	DeleteGraph(m_manhole2GraphHandle);
	DeleteGraph(m_winPlayer1GraphHandle);
	DeleteGraph(m_winPlayer2GraphHandle);
	DeleteGraph(m_gameStartUI1Handle);
	DeleteGraph(m_gameStartUI2Handle);
	delete m_roundTimer;
	DeleteSoundMem(m_gameStartSoundHandle);
	DeleteSoundMem(m_bgmHandle);
	DeleteSoundMem(m_winBgmHandle);
	StopSoundMem(m_gameOverBgHandle);
	DeleteSoundMem(m_gameOverBgHandle);
}

void SceneMain::Update()
{
	Vec2 p1Center = m_player1->GetCollisionRect().GetCenter();
	Vec2 p2Center = m_player2->GetCollisionRect().GetCenter();

	if (m_roundTimer->IsTimeUp())
	{
		// printfDx("時間切れ!");
		if (m_roundTimer->IsTimeUp() && !m_gameOver)
		{
			Vec2 manholeCenter = m_manhole->GetCenter();
			Vec2 p1Center = m_player1->GetCollisionRect().GetCenter();
			Vec2 p2Center = m_player2->GetCollisionRect().GetCenter();

			float dist1 = fabs(p1Center.x - manholeCenter.x);
			float dist2 = fabs(p2Center.x - manholeCenter.x);

			if (dist1 > dist2)
			{
				m_player1->SetState(PlayerState::TimeUp); // 敗者
				m_player2->SetState(PlayerState::Win);    // 勝者

				m_player2WinFlag = true;
			}
			else if (dist1 < dist2)
			{
				m_player1->SetState(PlayerState::Win);    // 勝者
				m_player2->SetState(PlayerState::TimeUp); // 敗者

				m_player1WinFlag = true;
			}
			else
			{
				m_player1->SetState(PlayerState::TimeUp);
				m_player2->SetState(PlayerState::TimeUp);
			}

			m_gameOver = true;
			m_roundTimer->Stop();
			PlaySoundMem(m_gameOverBgHandle, DX_PLAYTYPE_BACK);
			return;
		}
	}

	if (m_isStartSeq)
	{
		m_startTimer++;
		if (m_startTimer == 1)
		{
			PlaySoundMem(m_gameStartSoundHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(200, m_gameStartSoundHandle);
		}

		// 3秒後に開始
		if (m_startTimer > 180)
		{
			m_isStartSeq = false;
			m_roundTimer->Start(); // タイマーを再開
			m_timer = GetNowCount(); // タイマーの基準時間をリセット
		}
		return; // カウントダウン中はプレイヤーは動けない
	}

	// ワンボタン勝利
	if (CheckHitKey(KEY_INPUT_V))
	{
		m_gameOver = true;
		m_player1->SetGameOver(true);
		m_player2->SetGameOver(false);
		m_manhole->SetGameOver(true);
		m_player2WinFlag = true;
	}
	else if(CheckHitKey(KEY_INPUT_B))
	{ 
		m_gameOver = true;
		m_player1->SetGameOver(false);
		m_player2->SetGameOver(true);
		m_manhole->SetGameOver(true);
		m_player1WinFlag = true;
	}
	

	int currentTime = GetNowCount();
	float deltaTime = (currentTime - m_timer) / 1000.0f;

	m_timer = currentTime;
	int pad1 = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int pad2 = GetJoypadInputState(DX_INPUT_PAD2);
	// printfDx("%d", pad);
	if (CheckHitKey(KEY_INPUT_R)||(pad1&PAD_INPUT_X)|| (pad2 & PAD_INPUT_X))
	{
		End();   // 現在のリソースを解放
		Init();  // 再初期化
	}

	if (m_gameOver)
	{
		m_player1->Update(deltaTime);
		m_player2->Update(deltaTime);
		if (!m_bgmEndFlag)
		{
			// プレイヤー1が落ちた → プレイヤー2が勝利
			if (m_player2WinFlag)
			{
				if (m_player1->GetState() == PlayerState::Fall && m_player1->IsFallEnd())
				{
					//printfDx("プレイヤー2の勝利!");
					// 落下演出が終わったら勝利BGMを流す
					if (!m_bgmEndFlag && CheckSoundMem(m_gameOverBgHandle) == 0)
					{
						PlaySoundMem(m_winBgmHandle, DX_PLAYTYPE_LOOP);
						ChangeVolumeSoundMem(150, m_winBgmHandle);
						m_player2->SetState(PlayerState::Win);
						m_bgmEndFlag = true;
					}
				}
			}
			// プレイヤー2が落ちた → プレイヤー1が勝利
			else if (m_player1WinFlag)
			{
				if (m_player2->GetState() == PlayerState::Fall && m_player2->IsFallEnd())
				{
					//printfDx("プレイヤー1の勝利！");
					// 落下演出が終わったら勝利BGMを流す
					if (!m_bgmEndFlag && CheckSoundMem(m_gameOverBgHandle) == 0)
					{
						PlaySoundMem(m_winBgmHandle, DX_PLAYTYPE_BACK);
						ChangeVolumeSoundMem(150, m_winBgmHandle);
						m_player1->SetState(PlayerState::Win);
						m_bgmEndFlag = true;
					}
				}
			}

			m_Bg->Update();
			m_manhole->Update(p1Center,p2Center);
		}

		StopSoundMem(m_bgmHandle);
		return;
	}
	m_roundTimer->Update(deltaTime);
	
	m_player1->SetOtherPlayer(m_player2);
	m_player2->SetOtherPlayer(m_player1);
	m_player2->Update(deltaTime);
	m_player1->Update(deltaTime);
	m_Bg->Update();
	m_manhole->Update(p1Center,p2Center);
	if (!m_gameOver && !m_roundTimer->IsTimeUp())
	{
		// プレイヤー1が左マンホールに触れたら2の勝利
		if (m_manhole->CheckLeftCollision(m_player1->GetCollisionRect()))
		{
			m_player1->CheckManholeCollision(m_manhole);
			//printfDx("プレイヤー2の勝利!");
			m_gameOver = true;
			m_player2WinFlag = true;
			m_player1->SetGameOver(true);
			m_player1->SetState(PlayerState::Fall);
			m_player2->SetGameOver(false);
			m_manhole->SetGameOver(true);
			m_roundTimer->Stop();
			PlaySoundMem(m_gameOverBgHandle, DX_PLAYTYPE_BACK);
		}
		// プレイヤー2が右マンホールに触れたら1の勝利
		else if (m_manhole->CheckRightCollision(m_player2->GetCollisionRect()))
		{
			m_player2->CheckManholeCollision(m_manhole);
			//printfDx("プレイヤー1の勝利！");
			m_gameOver = true;
			m_player1WinFlag = true;
			m_player2->SetGameOver(true);
			m_player2->SetState(PlayerState::Fall);
			m_player1->SetGameOver(false);
			m_manhole->SetGameOver(true);
			m_roundTimer->Stop();
			PlaySoundMem(m_gameOverBgHandle, DX_PLAYTYPE_BACK);
		}
	}
}

void SceneMain::Draw()
{
	// 描画(後に描画したものが前に出る)
	m_Bg->Draw();
	m_player1->Draw();
	m_player2->Draw();
	m_roundTimer->Draw(640, 600);
	m_manhole->Draw();
	// ゲームタートのカウントダウン
	if (m_isStartSeq)
	{
		if (m_startTimer < 120)
		{
			DrawExtendGraph(500, 200, 800, 400, m_gameStartUI1Handle, true);
		}
		else
		{
			DrawExtendGraph(500, 200, 800, 400, m_gameStartUI2Handle, true);
		}
		return;
	}
	// 点滅用の処理
	int now = GetNowCount();
	// 500ミリ秒ごとに点滅する
	bool blink = ((now / 500) % 2) == 0;
	if (m_player1WinFlag&&blink)
	{
		DrawExtendGraph(500, 200, 800, 400, m_winPlayer1GraphHandle, true);
	}
	else if (m_player2WinFlag && blink)
	{
		DrawExtendGraph(500, 200, 800, 400, m_winPlayer2GraphHandle, true);
	}
#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
#endif
}

void SceneMain::StopBGM()
{
	StopSoundMem(m_bgmHandle);
}
