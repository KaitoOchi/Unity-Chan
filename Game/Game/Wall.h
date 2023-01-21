#pragma once


class Player;
class GameCamera;

class Wall: public IGameObject
{
public:
	Wall();
	~Wall();
	bool Start();
	void Update();

	//座標を設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//座標を取得
	inline Vector3 GetPosition() { return m_position; }
	//回転を設定
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	//タイプを設定
	inline void SetNumber(const int num)
	{
		//十の位が1なら
		if (num / 10 == 1) {
			//スクロール遷移しない
			m_canScroll = false;
		}
		m_wallNum = num - ((num / 10) * 10);
	}

private:
	//カメラ固定処理
	void SetCamera();
	//スクロール処理
	void SetScroll();
	//カメラの移動方向処理
	void SetState();

	Player* player = nullptr;
	GameCamera* gameCamera = nullptr;

	CollisionObject* m_collisionObject = nullptr;

	Vector3 m_position;
	Vector3 m_scrollPos;
	Vector3 m_outsidePos;
	Quaternion m_rotation;

	bool m_canScroll = true;
	int m_wallNum = 0;
	int m_scrollNum = 0;
	float m_enabledPos = 0.0f;
};