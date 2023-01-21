#pragma once

//�A�N�^�[�̊��N���X
class Actor
{
public:
	Actor();
	virtual ~Actor();
	virtual bool Start() = 0;
	virtual void Update() = 0;

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//�������擾
	inline int GetDirection() { return m_direction; }

	/// <summary>
	/// �_���[�W���v�Z���鏈��
	/// </summary>
	/// <param name="damage">��_���[�W</param>
	virtual void ReceiveDamage(int damage) = 0;

	/// <summary>
	/// ���ʂ�ݒ肷�鏈��
	/// </summary>
	/// <param name="vol">����</param>
	void SetSoundVolume(float vol) { m_volume = vol; }

protected:
	//�_�ŏ���
	void Blink();

	ModelRender				m_modelRender;

	EffectEmitter*			m_effect = nullptr;

	Vector3					m_position;
	Vector3					m_moveSpeed;
	Quaternion				m_rotation;

	bool					m_enabled = true;		//�\�����
	int						m_hp = 0;				//HP
	int						m_direction = 1;		//����
	float					m_stopTime = 0.0f;		//�d������
	float					m_ivTime = 0.0f;		//���G����
	float					m_volume = 0.0f;		//����
};