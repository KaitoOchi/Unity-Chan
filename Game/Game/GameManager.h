#pragma once


class GameManager : public IGameObject
{
public:
	GameManager();
	~GameManager();
	bool Start();

	//残機のリセット
	inline void ResetPlayerLeft() { m_playerLeft = 3; }
	//残機の設定
	inline void SetPlayerLeft(const int left) { m_playerLeft = left; }
	//残機の取得
	inline int GetPlayerLeft() { return m_playerLeft; }
	//復活地点の設定
	inline void SetContinuePosition(const Vector3& pos) { m_continuePos = pos; }
	//復活地点の取得
	inline Vector3 GetContinuePosition() { return m_continuePos; }
	//Y缶の取得情報を設定
	inline void SetGetYkan(const bool ykan) { m_getYkan = ykan; }
	//Y缶の取得情報を取得
	inline bool GetGetYkan() { return m_getYkan; }
	//ステージ番号を設定
	inline void SetStageNum(const int num) { m_stageNum = num; }
	//ステージ番号を取得
	inline int GetStageNum() { return m_stageNum; }
	//コース番号を設定
	inline void SetCourceNum(const int num) { m_courceNum = num; }
	//コース番号を取得
	inline int GetCourceNum() { return m_courceNum; }


	float m_bgm = 0.1f;		//BGMの音量
	float m_sfx = 0.1f;		//サウンドの音量

	bool m_isStageClear[4] = { true, true, true, true };		//ステージをクリアしたかどうか
	bool m_getItem[4] = { true, true, true, false };			//アイテムを取得したかどうか
	int m_leftUp = 0;											//Y缶の所持数
	bool m_isOwata = false;										//owata

	bool m_getYkan = false;			//既にY缶を取得したか

private:
	Vector3 m_continuePos = { 0.0f, 0.0f, 0.0f };	//復活する場所
	//Vector3 m_continuePos = { 12483.656f, 3361.0f, 0.0f };

	int m_playerLeft = 3;			//プレイヤーの最大残機
	int m_stageNum = 1;				//ステージ番号
	int m_courceNum = 0;			//コース番号
};
