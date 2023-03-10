#pragma once


class Player;

class Outside : public IGameObject {

public:
	Outside();
	~Outside();
	bool Start();
	void Update();

	//座標を設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//座標の取得
	inline Vector3 GetPosition() { return m_position; }
	//番号を設定
	inline void SetNumber(const int num) { m_number = num; }
	//番号の取得
	inline int GetNumber() { return m_number; }

private:
	Player* player = nullptr;

	Vector3 m_position;

	int m_number = 0;
};