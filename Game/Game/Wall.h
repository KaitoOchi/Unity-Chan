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

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//���W���擾
	inline Vector3 GetPosition() { return m_position; }
	//��]��ݒ�
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	//�^�C�v��ݒ�
	inline void SetNumber(const int num)
	{
		//�\�̈ʂ�1�Ȃ�
		if (num / 10 == 1) {
			//�X�N���[���J�ڂ��Ȃ�
			m_canScroll = false;
		}
		m_wallNum = num - ((num / 10) * 10);
	}

private:
	//�J�����Œ菈��
	void SetCamera();
	//�X�N���[������
	void SetScroll();
	//�J�����̈ړ���������
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