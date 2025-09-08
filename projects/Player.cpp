#include "Player.h"
#include "SceneMain.h"
#include"Dxlib.h"

namespace
{
	// プレイヤーサイズ
	constexpr float kPlayerSize = 48.0f;
	// プレイヤーグラフィックのサイズ
	constexpr int kGraphWidth = 48;
	constexpr int kGraphHeight = 48;
	// アニメーション情報
	constexpr int kIdleAnimNum = 4;
	constexpr int kAnimWaitFrame = 4;
	constexpr int kRunAnimNum = 6;
	constexpr int kAttackAnimNum = 8;
	constexpr int kWeakAttackAnimNum = 6;
	constexpr int kHurtAnimNum = 2;	
	constexpr int kFallAnimNum = 2;
	constexpr int kWinAnimNum = 6;
	constexpr int kTimeUpAnimNum = 6;
	// 攻撃クールタイム
	constexpr int kAttackCoolTime = 70;
	constexpr int kWeakAttackCoolTime = 35;
	//強攻撃の準備時間
	constexpr int kAttackPrep = 20;
	// 攻撃を受けた後の無敵時間
	constexpr int kHurtDuration = 40;
	constexpr int kWeakHurtDuration = 20;
	// 当たり判定の半径
	constexpr float kDefaultRadius = 16.0f;
	// プレイヤーの移動速度
	constexpr int  kSpeed = 2.0;
	// プレイヤーの拡大率
	constexpr float kScale = 2.0f;
	// ノックバックの距離
	constexpr int knockBackDist = 90;
	// 重力
	constexpr float kGravity = 1.5f;
	// 地面の当たり判定
	constexpr int kGround = 400;
	// 落下速度の最大値
	constexpr float kMaxFallSpeed = 40.0f;
	// 落下開始のY座標
	constexpr int kFallStartY = 400;
	// 一定の座標まで行くと描画をストップ
	constexpr int kDrawStopY = 525;
	// 攻撃判定のフレーム
	constexpr int kAttackActiveStartFrame = 3;
	constexpr int kAttackActiveEndFrame = 5;
	constexpr int kWeakAttackStartFrame = 2;
	constexpr int kWeakAttackEndFrame = 3;
	// 描画の拡大率
	constexpr float kPlayerScale = 2.5f;
}

Player::Player() :
	m_handle(-1),
	m_attackHandle(-1),
	m_wAttackHandle(-1),
	m_runHandle(-1),
	m_hurtHandle(-1),
	m_fallHandle(-1),
	m_winHandle(-1),
	m_timeUpHandle(-1),
	m_pos(0.0f, 0.0f),
	m_centerPos(0.0f, 0.0f),
	m_padType(0),
	m_radius(0.0f),
	m_isAttack(false),
	m_attackCount(0),
	m_wAttackCount(0),
	m_attackPrepCount(0),
	m_hurtCount(0),
	m_isTurn(false),
	m_animFrame(0),
	m_oldInput(0),
	m_isFalling(false),
	m_fallSpeed(0.0f),
	m_gameOver(false),
	m_currentFrame(0),
	m_hasHit(false),
	m_deltaTime(0),
	m_isFallEnd(false),
	m_weakBgHandle(-1),
	m_attackBgHandle(-1),
	m_state(PlayerState::Idle),
	m_attackType(AttackType::None),
	m_otherPlayer(nullptr),
	m_receivedAttackType(AttackType::None)
{
}

Player::~Player()
{

}

void Player::Init(int _padType, Vec2 _firstPos,int _handle,
	int _attackHandle,int _wAttackHandle,int _runHandle,
	int _hurtHandle,int _fallHandle,int _winHandle,int _timeUpHandle,bool _isTurn)
{
	// 初期化
	m_handle = _handle;
	m_attackHandle = _attackHandle;
	m_wAttackHandle = _wAttackHandle;
	m_runHandle = _runHandle;
	m_hurtHandle = _hurtHandle;
	m_fallHandle = _fallHandle;
	m_winHandle = _winHandle;
	m_timeUpHandle = _timeUpHandle;
	m_pos = _firstPos;
	m_pos.y = kGround;
	m_centerPos = Vec2(0.0f, 0.0f);
	m_padType = _padType;
	m_radius = kDefaultRadius;
	m_isAttack = false;
	m_attackCount = 0;
	m_wAttackCount = 0;
	m_attackPrepCount = 0;
	m_hurtCount = 0;
	m_isTurn = _isTurn;
	m_animFrame = 0;
	m_oldInput = 0;
	m_isFalling = false;
	m_fallSpeed = 0.0f;
	m_gameOver = false;
	m_currentFrame = 0;
	m_hasHit = false;
	m_deltaTime = 0;
	m_isFallEnd = false;
	m_weakBgHandle = LoadSoundMem("data/BGM・SE/weak.mp3");
	m_attackBgHandle = LoadSoundMem("data/BGM・SE/attack.mp3");
	m_state = PlayerState::Idle;
	m_attackType = AttackType::None;
	m_receivedAttackType = AttackType::None;
	ChangeVolumeSoundMem(150,m_weakBgHandle);
	ChangeVolumeSoundMem(150,m_attackBgHandle);
}

void Player::End()
{
	StopSoundMem(m_weakBgHandle);
	DeleteSoundMem(m_weakBgHandle);
	StopSoundMem(m_attackBgHandle);
	DeleteSoundMem(m_attackBgHandle);
}

void Player::Update(float _deltaTime)
{
	// 重力
	Gravity();

	// 当たり判定
	if (m_isTurn)
	{
		m_colRect.init(m_pos.x - kGraphWidth / 2 + 80.0f,
		m_pos.y - kGraphHeight / 2 + 48.0f,
		kGraphWidth, kGraphHeight);
		
	}
	else
	{
		m_colRect.init(m_pos.x - kGraphWidth / 2 + 40.0f,
		m_pos.y - kGraphHeight / 2 + 48.0f,
		kGraphWidth, kGraphHeight);
	}

	//	コントローラーのボタンの押された状態を取得する
	int padInput = GetJoypadInputState(m_padType);
	int keyInput = 0;

	if (m_padType == DX_INPUT_PAD1)
	{
		if (CheckHitKey(KEY_INPUT_Z))     keyInput |= PAD_INPUT_A;
		if (CheckHitKey(KEY_INPUT_X))	keyInput |= PAD_INPUT_B;
		if (CheckHitKey(KEY_INPUT_A))  keyInput |= PAD_INPUT_LEFT;
		if (CheckHitKey(KEY_INPUT_D)) keyInput |= PAD_INPUT_RIGHT;
		if (CheckHitKey(KEY_INPUT_W))    keyInput |= PAD_INPUT_UP;
		if (CheckHitKey(KEY_INPUT_S))  keyInput |= PAD_INPUT_DOWN;
	}
	else if (m_padType == DX_INPUT_PAD2)
	{
		if (CheckHitKey(KEY_INPUT_K))     keyInput |= PAD_INPUT_A;
		if (CheckHitKey(KEY_INPUT_M))	keyInput |= PAD_INPUT_B;
		if (CheckHitKey(KEY_INPUT_LEFT))  keyInput |= PAD_INPUT_LEFT;
		if (CheckHitKey(KEY_INPUT_RIGHT)) keyInput |= PAD_INPUT_RIGHT;
		if (CheckHitKey(KEY_INPUT_UP))    keyInput |= PAD_INPUT_UP;
		if (CheckHitKey(KEY_INPUT_DOWN))  keyInput |= PAD_INPUT_DOWN;
	}
	int input = padInput | keyInput; // 両方の入力を統合

	// プレイヤーの状態の更新
	UpdateState(input);
	// プレイヤーのアニメーションの更新
	UpdateAnim();
	// 重力の制限
	if (!m_isFalling&& !m_gameOver && m_pos.y >= kGround)
	{
		m_pos.y = kGround;
	}

	// 落下中の処理（Win判定のために毎フレーム動かす）
	if (m_state == PlayerState::Fall)
	{
		m_fallSpeed += kGravity * _deltaTime;
		if (m_fallSpeed > kMaxFallSpeed)
		{
			m_fallSpeed = kMaxFallSpeed;
		}
		m_pos.y += m_fallSpeed * _deltaTime;

		if (m_pos.y > kDrawStopY)
		{
			m_isFallEnd = true;
		}

	}
	// 強攻撃の攻撃判定（Attack状態）
	if (m_state == PlayerState::Attack &&
		m_animFrame >= kAttackActiveStartFrame &&
		m_animFrame <= kAttackActiveEndFrame&&
		!m_hasHit)
	{
		KnockBack();
	}

	// 弱攻撃の攻撃判定（WeakAttack状態）
	if (m_state == PlayerState::WeakAttack &&
		m_animFrame >= kWeakAttackStartFrame &&
		m_animFrame <= kWeakAttackEndFrame&&
		!m_hasHit)
	{
		KnockBack();
	}

	// 勝敗がついていたら操作を無効化（ただしアニメーションは進める）
	if (m_gameOver && m_state != PlayerState::Win && m_state != PlayerState::Fall)
	{
		// アニメーションだけ更新
		UpdateAnim();
		return;
	}

}

void Player::Draw()
{
	// ゲームオーバー時に描画を止める
	if (m_state == PlayerState::Fall && m_pos.y > kDrawStopY)
	{
		return;
	}

	// アニメーションのフレーム数から表示したいコマ番号を計算で求める
	int animNum = 0;
	//// プレイヤーのそれぞれの状態をもとにX座標を計算する
	int srcX = 0;
	int srcY = 0;
	int handle = m_handle;

	// プレイヤーの状態に応じてアニメーションのハンドルを設定
	switch (m_state)
	{
	case PlayerState::Idle:			// 待機中
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kIdleAnimNum;
		handle = m_handle;
		srcY = 0;
		break;
	}

	case PlayerState::Run:			// 移動中
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kRunAnimNum;
		handle = m_runHandle;
		srcY = 0;
		break;
	}

	case PlayerState::Attack:		// 強攻撃
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kAttackAnimNum;
		handle = m_attackHandle;
		srcY = 0;
		break;
	}

	case PlayerState::WeakAttack:	// 弱攻撃
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kWeakAttackAnimNum;
		handle = m_wAttackHandle;
		srcY = 0;
		break;
	}

	case PlayerState::Hurt:			// 攻撃を受けた状態
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kHurtAnimNum;
		handle = m_hurtHandle;
		srcY = 0;
		break;
	}
	case PlayerState::Fall:			// 落下中
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kFallAnimNum;
		handle = m_fallHandle;
		srcY = 0;
		break;
	}
	case PlayerState::Win:			// 勝利
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kWinAnimNum;
		handle = m_winHandle;
		srcY = 0;
		break;
	}
	case PlayerState::TimeUp:		// タイムアップ
	{
		animNum = (m_animFrame / kAnimWaitFrame) % kTimeUpAnimNum;
		handle = m_timeUpHandle;
		srcY = 0;
		break;
	}
	}

	// アニメーションのX座標を求める
	srcX = kGraphWidth * animNum;

	// プレイヤーの描画処理
	if (handle != -1)
	{
		int drawWidth = static_cast<int>(kGraphWidth * kPlayerScale);
		int drawHeight = static_cast<int>(kGraphHeight * kPlayerScale);

if (m_isTurn)
{
	// 左向き（反転描画）
	DrawRectExtendGraph(
		static_cast<int>(m_pos.x) + drawWidth-5,
		static_cast<int>(m_pos.y)-30,
		static_cast<int>(m_pos.x), 
		static_cast<int>(m_pos.y) + drawHeight-30,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		handle, TRUE);
}
else
{
	// 右向き
	DrawRectExtendGraph(
		static_cast<int>(m_pos.x), static_cast<int>(m_pos.y)-30,
		static_cast<int>(m_pos.x) + drawWidth,
		static_cast<int>(m_pos.y) + drawHeight-30,
		srcX, srcY,
		kGraphWidth, kGraphHeight,
		handle, TRUE);
}

	}
#ifdef _DEBUG
	// 当たり判定の表示
	m_colRect.Draw(0xFFFF00, false);
#endif
}

void Player::Gravity()
{
	// 重力を追加する
	m_pos.y += kGravity;
}

// プレイヤーの状態を更新する関数
void Player::UpdateState(int _input)
{
	// プレイヤーの状態を保存
	PlayerState saveState = m_state;
	// 攻撃のトリガーをチェック
	bool attackTrigger = (_input & PAD_INPUT_A) && !(m_oldInput & PAD_INPUT_A);
	// 弱攻撃のトリガーをチェック
	bool weakAttackTrigger = (_input & PAD_INPUT_B) && !(m_oldInput & PAD_INPUT_B);

	// 落下中は操作を受け付けない
	if (m_state == PlayerState::Fall)
	{
		m_oldInput = _input; // 入力履歴だけ更新しておく
		return;
	}


	// プレイヤーの状態を更新する
	switch (m_state)
	{
	case PlayerState::Idle:
		if (!m_isAttack && IsMoving(_input))
		{
			m_state = PlayerState::Run;
		}
		else if ((_input & PAD_INPUT_A))
		{
			m_state = PlayerState::AttackPrep;
			m_attackCount = 0;
			m_isAttack = true;
			//printfDx("攻撃した！\n");
		}
		else if ((_input & PAD_INPUT_B))
		{
			//printfDx("Bボタンを押した！\n");
			m_attackType = AttackType::Weak;
			m_wAttackCount = 0;
			m_isAttack = true;
			m_state = PlayerState::WeakAttack;
			//printfDx("弱攻撃した！\n");
			PlaySoundMem(m_weakBgHandle, DX_PLAYTYPE_BACK);
		}
		break;

		// 強攻撃の準備状態
	case PlayerState::AttackPrep:
	{
		m_attackPrepCount++;
		if (m_attackPrepCount >= kAttackPrep)
		{
			m_state = PlayerState::Attack;
			PlaySoundMem(m_attackBgHandle, DX_PLAYTYPE_BACK);
			m_attackPrepCount = 0;
			m_attackType = AttackType::Normal;
			//printfDx("強攻撃の準備完了!\n");
		}
		break;
	}

	case PlayerState::Run:
	{
		// 移動処理
		if (_input & PAD_INPUT_LEFT && !(_input & PAD_INPUT_RIGHT))
		{
			m_pos.x -= kSpeed;
			m_isTurn = true;
		}
		if (_input & PAD_INPUT_RIGHT && !(_input & PAD_INPUT_LEFT))
		{
			m_pos.x += kSpeed;
			m_isTurn = false;
		}
		if (!IsMoving(_input))
		{
			m_state = PlayerState::Idle;
		}
		break;
	}

	case PlayerState::Attack:
		m_attackCount++;
		// 攻撃
		// 押した瞬間判定する
		if (!m_isAttack && attackTrigger)
		{
			m_state = PlayerState::Attack;
			m_attackCount = 0;
			m_isAttack = true;
			//printfDx("攻撃！\n");
		}

		// 攻撃のカウントがクールタイムを超えたら
		if (m_attackCount > kAttackCoolTime)
		{
			m_isAttack = false;
			m_attackType = AttackType::Normal;
			m_state = IsMoving(_input) ? PlayerState::Run : PlayerState::Idle;
			// 攻撃カウントをリセット
			m_attackCount = 0;
			m_hasHit = false;
		}
		break;

	case PlayerState::WeakAttack:	// 弱攻撃
		m_wAttackCount++;
		if (!m_isAttack && weakAttackTrigger)
		{
			m_state = PlayerState::WeakAttack;
			m_attackCount = 0;
			m_isAttack = true;
			//printfDx("弱パンチ！\n");
		}

		if (m_wAttackCount > kWeakAttackCoolTime)
		{
			// クールタイムを超えたら通常状態に戻す
			m_isAttack = false;
			m_attackType = AttackType::Normal;// 攻撃タイプを通常に戻す
			m_state = IsMoving(_input) ? PlayerState::Run : PlayerState::Idle;
			m_wAttackCount = 0;
			m_hasHit = false;
		}
		break;
	case PlayerState::Hurt:
		m_hurtCount++;
		//printfDx("HurtCount:%d\n", m_hurtCount);
		// 攻撃を受けた後の無敵時間を経過したら通常状態に戻す
		if ((m_receivedAttackType == AttackType::Weak && m_hurtCount > kWeakHurtDuration) ||
			(m_receivedAttackType == AttackType::Normal && m_hurtCount > kHurtDuration))
		{
			m_hurtCount = 0;
			m_receivedAttackType = AttackType::None;

			m_isAttack = false;
			m_hasHit = false;
			m_state = IsMoving(_input) ? PlayerState::Run : PlayerState::Idle;
		}
		break;
	case PlayerState::Fall:
		break;
	case PlayerState::Win:
		break;
	case PlayerState::TimeUp:
		break;
	}
	m_oldInput = _input; // 前回の入力状態を更新
}

// プレイヤーの移動反転処理
bool Player::IsMoving(int _input)
{
	return (_input & PAD_INPUT_LEFT) || (_input & PAD_INPUT_RIGHT);
}

// プレイヤーのアニメーション
void Player::UpdateAnim()
{
	int animFrames = 0;
	switch (m_state)
	{
	case PlayerState::Idle:
		animFrames = kIdleAnimNum;
		break;
	case PlayerState::AttackPrep:
		animFrames = 2; 
		break;
	case PlayerState::Run:
		animFrames = kRunAnimNum;
		break;
	case PlayerState::Attack:
		animFrames = kAttackAnimNum;
		break;
	case PlayerState::WeakAttack:
		animFrames = kWeakAttackAnimNum;
		break;
	case PlayerState::Hurt:
		animFrames = kHurtAnimNum;
		break;
	case PlayerState::Fall:
		animFrames = kFallAnimNum;
		break;
	case PlayerState::Win:
		animFrames = kWinAnimNum;
		break;
	case PlayerState::TimeUp:
		animFrames = kTimeUpAnimNum;
		break;
	}

	// アニメーションフレームを更新
	if (animFrames > 0)
	{
		m_animFrame++;
		if (m_animFrame >= animFrames * kAnimWaitFrame)
		{
			m_animFrame = 0;
		}
	}
}

// Hurt状態を返す関数
bool Player::IsHurt() const
{
	return m_state == PlayerState::Hurt;
}

void Player::CheckManholeCollision(Manhole* manhole)
{
	if (m_state == PlayerState::Fall || m_state == PlayerState::TimeUp || m_gameOver)return;

	bool hitLeft = manhole->CheckLeftCollision(m_colRect);
	bool hitRight = manhole->CheckRightCollision(m_colRect);

	if (hitLeft || hitRight)
	{
		m_state = PlayerState::Fall;
		m_animFrame = 0;
		m_isFalling = true;
		m_fallSpeed = 0.0f;
		//printfDx(hitLeft ? "左のマンホールに落ちた\n" : "右のマンホールに落ちた\n");
	}
}

//プレイヤーの攻撃(ノックバック)処理 
void Player::KnockBack()
{
	if (m_hasHit || m_otherPlayer->m_hasHit) return;

	// 両者が攻撃中なら相打ち
	bool bothAttacking = (m_state == PlayerState::Attack || m_state == PlayerState::WeakAttack) &&
		(m_otherPlayer->m_state == PlayerState::Attack || m_otherPlayer->m_state == PlayerState::WeakAttack);
	if (m_colRect.IsCollision(m_otherPlayer->GetCollisionRect()))
	{
		if (bothAttacking)
		{
			// 相打ち処理（両者を同時にHurt）
			float knockBackValueA = (m_attackType == AttackType::Weak) ? knockBackDist * 0.5f : knockBackDist;
			float knockBackValueB = (m_otherPlayer->m_attackType == AttackType::Weak) ? knockBackDist * 0.5f : knockBackDist;

			m_pos.x += m_otherPlayer->m_isTurn ? -knockBackValueB : knockBackValueB;
			m_otherPlayer->m_pos.x += m_isTurn ? -knockBackValueA : knockBackValueA;

			m_state = PlayerState::Hurt;
			m_otherPlayer->m_state = PlayerState::Hurt;

			m_hurtCount = 0;
			m_otherPlayer->m_hurtCount = 0;

			m_receivedAttackType = m_otherPlayer->m_attackType;
			m_otherPlayer->m_receivedAttackType = m_attackType;

			m_animFrame = 0;
			m_otherPlayer->m_animFrame = 0;
			m_hasHit = true;
			m_otherPlayer->m_hasHit = true;

			return; // 相打ちで終了
		}

		// 攻撃処理（相手が攻撃中でない場合）
		if (!m_otherPlayer->IsHurt())
		{
			float knockBackValue = (m_attackType == AttackType::Weak) ? knockBackDist * 0.5f : knockBackDist;
			m_otherPlayer->m_pos.x += m_isTurn ? -knockBackValue : knockBackValue;
			m_otherPlayer->m_state = PlayerState::Hurt;
			m_otherPlayer->m_hurtCount = 0;
			m_otherPlayer->m_receivedAttackType = m_attackType;
			m_otherPlayer->m_animFrame = 0;
			m_hasHit = true;
		}
	}
}