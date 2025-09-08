#pragma once
#include"Title.h"
#include"SceneMain.h"

enum SceneType
{
	SCENE_TITLE,
	SCENE_MAIN,
};

enum FadeState
{
	FADE_NONE,
	FADE_OUT,
	FADE_IN
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init();
	void End();
	void Update();
	void Draw();

	// 現在のシーンを取得する関数
	void ChangeScene(SceneType nextScene);

private:
	// 現在のシーンのタイプ
	SceneType m_currentType;
	// SceneTitleの変数
	Title m_title;
	// SceneMainの変数
	SceneMain m_main;
	// 次のシーン遷移保存用変数
	SceneType m_nextScene;
	// フェード制御
	FadeState m_fadeState;
	int m_fadeAlpha;
	// フェードインが終了しているかどうか
	bool m_hasFadeIn = false;
	// 決定音のサウンド
	int m_pushHandle;
};

