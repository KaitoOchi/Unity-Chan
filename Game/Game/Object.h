#pragma once


class Player;

//オブジェクトの基底クラス
class Object 
{
public:
	Object();
	virtual ~Object();
	virtual bool Start() = 0;
	virtual void Update() = 0;

	//座標の設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//回転の設定
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	//種類を設定
	inline void SetType(const int type) { m_type = type; }

protected:
	//表示状態を取得する
	bool GetEnabled(const Vector3& pos);

	ModelRender m_modelRender;

	CollisionObject* m_collisionObject;

	Player* player = nullptr;

	Vector3 m_position;
	Vector3 m_firstPos;
	Quaternion m_rotation;

	bool m_enabled = true;		//表示状態
	int m_type = 0;				//タイプ
};