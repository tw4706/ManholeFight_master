#pragma once
class Title
{
public:
	Title();
	~Title();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	// タイトル画面のグラフィックハンドル
	int m_titleGraphHandle;
	int m_titleLogoHandle;
	int m_titleStrHandle;
	// BGMのグラフィックハンドル
	int m_bgmHandle;
	// 決定ボタンのサウンド
	int m_pushHandle;

};

