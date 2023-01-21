#pragma once


class Floor;

class VanishFloorManager : public IGameObject
{
public:
	VanishFloorManager();
	~VanishFloorManager();
	bool Start();
	void Update();

	//座標の設定
	inline void SetPosition(const Vector3& pos, const int num) { m_floorPosition[num] = pos; }

	/// <summary>
	/// 音量を設定する処理
	/// </summary>
	/// <param name="vol">音量</param>
	void SetSoundVolume(float vol) { m_volume = vol; }
	
	//登録した座標を基に足場を生成する
	void SpawnFloor();

private:
	Floor*		floor[10];

	Vector3		m_floorPosition[10];
	int			m_frameCount = 0;		//フレームカウント
	int			m_floorNum = 0;			//床番号
	int			m_activeNum = 0;		//表示状態の床番号
	float		m_volume = 0.0f;		//音量
};

