#pragma once


class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//�t�F�[�h�C��
	inline void StartFadeIn() { state = enState_FadeIn; }
	//�t�F�[�h�A�E�g
	inline void StartFadeOut() { state = enState_FadeOut; }
	//�t�F�[�h��
	inline const bool IsFade() const { return state != enState_Idle; }

	float m_alpha = 1.0f;

private:
	enum EnState {
		enState_FadeIn,			//�t�F�[�h�C����
		enState_FadeOut,		//�t�F�[�h�A�E�g��
		enState_Idle,			//�ҋ@��
	};

	SpriteRender	m_spriteRender;
	EnState			state = enState_Idle;		//���

};

