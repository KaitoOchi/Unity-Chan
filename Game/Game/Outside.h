#pragma once


class Player;

class Outside : public IGameObject {

public:
	Outside();
	~Outside();
	bool Start();
	void Update();

	//À•W‚ğİ’è
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//À•W‚Ìæ“¾
	inline Vector3 GetPosition() { return m_position; }
	//”Ô†‚ğİ’è
	inline void SetNumber(const int num) { m_number = num; }
	//”Ô†‚Ìæ“¾
	inline int GetNumber() { return m_number; }

private:
	Player* player = nullptr;

	Vector3 m_position;

	int m_number = 0;
};