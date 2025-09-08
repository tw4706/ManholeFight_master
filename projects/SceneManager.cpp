#include "SceneManager.h"
#include"Dxlib.h"

namespace
{
	constexpr int kFadeSpeed = 5;
}

SceneManager::SceneManager():
	m_currentType(SCENE_TITLE),
	m_nextScene(SCENE_TITLE),
	m_fadeState(FADE_NONE),
	m_fadeAlpha(0),
	m_hasFadeIn(false),
	m_pushHandle(-1)
{
}

SceneManager::~SceneManager()
{
	//m_title.End();
	//m_main.End();
}

void SceneManager::Init()
{
	m_title.Init();
	m_pushHandle = LoadSoundMem("data/BGM・SE/push.mp3");
}

void SceneManager::End()
{
	StopSoundMem(m_pushHandle);
	DeleteSoundMem(m_pushHandle);
}

void SceneManager::Update()
{
	int pad = GetJoypadInputState(DX_INPUT_PAD1);
	int pad2 = GetJoypadInputState(DX_INPUT_PAD2);
	switch (m_fadeState)
	{
	case FADE_NONE:
		switch (m_currentType)
		{
		case SCENE_TITLE:
			m_title.Update();
			// Enterキーを押したらゲーム画面に飛ぶ

			if (CheckHitKey(KEY_INPUT_RETURN) || (pad & PAD_INPUT_1) || (pad2 & PAD_INPUT_1))
			{
				PlaySoundMem(m_pushHandle, DX_PLAYTYPE_BACK);
				ChangeVolumeSoundMem(200, m_pushHandle);
				m_fadeState = FADE_OUT;
				m_fadeAlpha = 0;
				m_nextScene = SCENE_MAIN;
			}
			break;

		case SCENE_MAIN:
			m_main.Update();
			// ゲーム画面からタイトルに戻る処理
			if (CheckHitKey(KEY_INPUT_E)|| (pad & PAD_INPUT_10) || (pad2 & PAD_INPUT_10))
			{
				m_fadeState = FADE_OUT;
				m_fadeAlpha = 0;
				m_nextScene = SCENE_TITLE;
				m_main.StopBGM();
			}
			break;
		}
		break;
	case FADE_OUT:
		m_fadeAlpha += kFadeSpeed;
		if (m_fadeAlpha >= 255)
		{
			m_fadeAlpha = 255;
			ChangeScene(m_nextScene);
			// 最初のフェードインだけ許可
			if (!m_hasFadeIn)
			{
				m_fadeState = FADE_IN;
				m_hasFadeIn = true;
			}
			else
			{
				m_fadeState = FADE_NONE;
			}
		}
		break;
	case FADE_IN:
		m_fadeAlpha -= kFadeSpeed;
		if (m_fadeAlpha <= 0)
		{
			m_fadeAlpha = 0;
			m_fadeState = FADE_NONE;
		}
		break;
	}
}

void SceneManager::Draw()
{
	// シーンごとの描画処理
	switch (m_currentType)
	{
	case SCENE_TITLE:
		m_title.Draw();
		break;
	case SCENE_MAIN:
		m_main.Draw();
		break;
	}
	if (m_fadeState != FADE_NONE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

// シーンの切り替え
void SceneManager::ChangeScene(SceneType nextScene)
{
	switch (m_currentType)
	{
	case SCENE_TITLE:
		m_title.End();
		break;

		case SCENE_MAIN:
			m_main.End();
			break;
	}
	m_currentType = nextScene;
	switch (m_currentType)
	{
	case SCENE_TITLE:
		m_title.Init();
		break;

	case SCENE_MAIN:
		m_main.Init();
		break;
	}
}
