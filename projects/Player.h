#pragma once
#include"Manhole.h"
#include"Rect.h"
#include"DxLib.h"
#include"Vec2.h"
#include<vector>

enum class PlayerState
{
	Idle,		// 待機
	Run,		// 走る
	AttackPrep,	// 強攻撃準備
	Attack,		// 強攻撃
	WeakAttack,	// 弱攻撃
	Hurt,		// 攻撃を受けた
	Fall,		// 落下中
	Win,		// 勝利
	TimeUp		// タイムアップ
};

enum class AttackType
{
	None,		// 攻撃していない
	Normal,		// 強攻撃
	Weak,		// 弱攻撃
};

class Player; // 前方宣言

// 攻撃を一時保存するための構造体
struct Attack
{
	Player* attacker;
	Player* target;
	int priority;   // 強攻撃ほど高く
	int hitFrame;   // 攻撃が当たったフレーム
};

class Player
{
public:
	Player();
	~Player();

	void Init(int _padType,Vec2 _firstPos,int _handle,int _attackHandle,
		int _wAttackHandle,int _runHandle,int _hurtHandle,
		int _fallHandle,int _winHandle,int _timeUpHandle,bool _isTurn);
	void End();
	void Update(float _deltaTime);
	void Draw();
	void Gravity();	// 重力

	void UpdateState(int _input);									// プレイヤーの状態管理
	bool IsMoving(int _input);										// プレイヤーの移動
	void UpdateAnim();												// プレイヤーのアニメーション管理
	void SetOtherPlayer(Player* other) { m_otherPlayer = other; }	// 攻撃対象の取得
	void KnockBack();
	// ノックバック処理
	// 当たり判定を取得するためのgetter関数
	const Rect& GetCollisionRect()const { return m_colRect; }
	// 座標のgetter関数
	const Vec2& GetPos() const { return m_pos; }
	bool IsHurt()const;
	void SetGameOver(bool isOver) { m_gameOver = isOver; }			// ゲームオーバーの判定
	void CheckManholeCollision(Manhole* pManhole);					// マンホールとの当たり判定
	bool IsFallEnd()const { return m_isFallEnd; }					// 落下終了の判定
	PlayerState GetState()const { return m_state; }				// プレイヤーの状態を取得
	PlayerState SetState(PlayerState state) { return m_state = state; } // プレイヤーの状態を設定
	// 先輩からのアドバイス:関数は動詞から始める
	//　UpdateAnim
	//　UpdateState


private:

	int m_handle;				// グラフィックのハンドル
	int m_attackHandle;			// 強攻撃のグラフィックのハンドル
	int m_wAttackHandle;		// 弱攻撃のグラフィックのハンドル
	int m_runHandle;			// 走るグラフィックのハンドル
	int m_hurtHandle;			// 攻撃を受けた時のグラフィックのハンドル
	int m_fallHandle;			// 落下中のグラフィックのハンドル
	int m_winHandle;			// 勝利のグラフィックのハンドル
	int m_timeUpHandle;			// タイムアップのグラフィックのハンドル
	int m_weakBgHandle;			// 弱攻撃のサウンド
	int m_attackBgHandle;		// 強攻撃のサウンド
	Vec2 m_pos;					// 座標
	Vec2 m_centerPos;			// 当たり判定のサイズ座標
	int m_padType;				// パッドの種類
	float m_radius;				// 当たり判定の半径
	bool m_isAttack;			// 攻撃中かどうか
	int m_attackCount;			// 強攻撃のカウント
	int m_wAttackCount;			// 弱攻撃のカウント
	int m_attackPrepCount;		// 強攻撃の準備のカウント
	int m_hurtCount;			// 攻撃を受けたカウント
	bool m_isTurn;				// 左右反転
	int m_animFrame;			// アニメーションのフレーム
	int m_oldInput;				// 前回の入力
	bool m_isFalling;			// 落下中かどうか
	float m_fallSpeed;			// 落下速度
	bool m_gameOver;			// ゲームオーバーかどうか
	int m_currentFrame;			// 現在のフレーム数
	bool m_hasHit;				// 攻撃判定済みフラグ
	float m_deltaTime;			// 落下の経過時間
	bool m_isFallEnd;			// 落下終了かどうか
	PlayerState m_state;		// プレイヤーの状態
	AttackType m_attackType;	// 攻撃の種類
	Player* m_otherPlayer;		// 対戦相手のプレイヤー(攻撃の対象となる)
	AttackType m_receivedAttackType;// 攻撃状態保存用
	std::vector<Attack> m_pendingAttacks;
protected:
	// 当たり判定の矩形
	Rect m_colRect;	
};

