#pragma once

class Fade;
class GameManager;

class Menu : public IGameObject
{
public:
	Menu();
	~Menu();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	//入力処理
	void Input();
	//説明文の処理
	void Text();
	//カーソル移動の処理
	void Select(const int select);
	//決定処理
	void StateTransition(const bool num);
	//ステージセレクトシーンの遷移処理
	void NotifyStageSelect(bool isStageClear[], bool getItem[], const int leftUp, const bool isAll);

	ModelRender			m_backGroundModelRender;
	ModelRender			m_cursorModelRender;
	SpriteRender		m_menuSpriteRender;
	SpriteRender		m_settingSpriteRender;
	SpriteRender		m_explainSpriteRender;
	SpriteRender		m_bgmSpriteRender[10];
	SpriteRender		m_sfxSpriteRender[10];
	SpriteRender		m_difficultSpriteRender[2];
	FontRender			m_debugFontRender;
	FontRender			m_explainFontRender;

	Fade*				fade = nullptr;
	GameManager*		gameManager = nullptr;

	Quaternion			m_rotation;

	const wchar_t* m_message = nullptr;

	const wchar_t m_message_menu[6][255] = {L"さいしょから はじめる",
											 L"操作方法を 確認する",
											 L"設定を 変更する",
											 L"BGMを 変更します",
										 	 L"効果音を 変更します",
											 L"ゲームモードを 変更します",
											};

	char				font[5][5];
	bool				password[5][5] = { { 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 } };

	bool				m_isWaitFadeOut = false;

	int					m_state = 0;		//ステート
	int					m_hCursor = 0;		//水平方向のカーソル
	int					m_vCursor = 0;		//垂直方向のカーソル
	int					m_maxCursor = 2;	//カーソルの最大値
	int					m_maxHCursor[3] = { 10, 10, 1 };
	int					m_settingNum[3] = { 5, 5, 0 };

	//セーブデータ
	//0～3　クリアしたステージ
	//4～7　アイテム取得したステージ
	//8		Y缶の量
	//9		ゲームモード
	//10	bgm
	//11	sfx
	int m_saveData[12] = { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 2 };
};