#pragma once


class GameManager : public IGameObject
{
public:
	GameManager();
	~GameManager();
	bool Start();

	//�c�@�̃��Z�b�g
	inline void ResetPlayerLeft() { m_playerLeft = 3; }
	//�c�@�̐ݒ�
	inline void SetPlayerLeft(const int left) { m_playerLeft = left; }
	//�c�@�̎擾
	inline int GetPlayerLeft() { return m_playerLeft; }
	//�����n�_�̐ݒ�
	inline void SetContinuePosition(const Vector3& pos) { m_continuePos = pos; }
	//�����n�_�̎擾
	inline Vector3 GetContinuePosition() { return m_continuePos; }
	//Y�ʂ̎擾����ݒ�
	inline void SetGetYkan(const bool ykan) { m_getYkan = ykan; }
	//Y�ʂ̎擾�����擾
	inline bool GetGetYkan() { return m_getYkan; }
	//�X�e�[�W�ԍ���ݒ�
	inline void SetStageNum(const int num) { m_stageNum = num; }
	//�X�e�[�W�ԍ����擾
	inline int GetStageNum() { return m_stageNum; }
	//�R�[�X�ԍ���ݒ�
	inline void SetCourceNum(const int num) { m_courceNum = num; }
	//�R�[�X�ԍ����擾
	inline int GetCourceNum() { return m_courceNum; }


	float m_bgm = 0.1f;		//BGM�̉���
	float m_sfx = 0.1f;		//�T�E���h�̉���

	bool m_isStageClear[4] = { true, true, true, true };		//�X�e�[�W���N���A�������ǂ���
	bool m_getItem[4] = { true, true, true, false };			//�A�C�e�����擾�������ǂ���
	int m_leftUp = 0;											//Y�ʂ̏�����
	bool m_isOwata = false;										//owata

	bool m_getYkan = false;			//����Y�ʂ��擾������

private:
	Vector3 m_continuePos = { 0.0f, 0.0f, 0.0f };	//��������ꏊ
	//Vector3 m_continuePos = { 12483.656f, 3361.0f, 0.0f };

	int m_playerLeft = 3;			//�v���C���[�̍ő�c�@
	int m_stageNum = 1;				//�X�e�[�W�ԍ�
	int m_courceNum = 0;			//�R�[�X�ԍ�
};
