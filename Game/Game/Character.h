#pragma once


class Character : public IGameObject
{
public:
	Character();
	~Character();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//�ʒu��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//��]��ݒ�
	inline void SetRotation(const float rot) { m_rotation.SetRotationDegY(rot); }
	//�ړ��\���ǂ����ݒ�
	inline void SetCanMove(const bool move) { m_canMove = move; }

private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip		animationClips[enAnimationClip_Num];

	void Move();
	void PlayAnimation();

	ModelRender			m_modelRender;
	Vector3				m_position;
	Quaternion			m_rotation;

	EnAnimationClip		m_animationState = enAnimationClip_Idle;

	bool				m_canMove = false;		//�����邩
	int					m_direction = 1;		//����
};

