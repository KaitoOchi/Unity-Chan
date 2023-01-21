#pragma once


class Player;

//�I�u�W�F�N�g�̊��N���X
class Object 
{
public:
	Object();
	virtual ~Object();
	virtual bool Start() = 0;
	virtual void Update() = 0;

	//���W�̐ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//��]�̐ݒ�
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	//��ނ�ݒ�
	inline void SetType(const int type) { m_type = type; }

protected:
	//�\����Ԃ��擾����
	bool GetEnabled(const Vector3& pos);

	ModelRender m_modelRender;

	CollisionObject* m_collisionObject;

	Player* player = nullptr;

	Vector3 m_position;
	Vector3 m_firstPos;
	Quaternion m_rotation;

	bool m_enabled = true;		//�\�����
	int m_type = 0;				//�^�C�v
};