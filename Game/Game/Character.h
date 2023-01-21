#pragma once


class Character : public IGameObject
{
public:
	Character();
	~Character();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//ˆÊ’u‚ğİ’è
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//‰ñ“]‚ğİ’è
	inline void SetRotation(const float rot) { m_rotation.SetRotationDegY(rot); }
	//ˆÚ“®‰Â”\‚©‚Ç‚¤‚©İ’è
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

	bool				m_canMove = false;		//“®‚¯‚é‚©
	int					m_direction = 1;		//•ûŒü
};

