#pragma once

#include "Level3DRender/LevelRender.h"

/*Credit
* Sound / BGM
* >OrangSoundSeed
* >OtoLogic
* >KAML
* 
* CG
* >CGtrader
*/

/*Effect
* EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/barrett.efk");
* EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/weapon01.efk");
* EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/weapon02.efk");
* EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/weapon03.efk");
* 
* EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/death.efk");
* EffectEngine::GetInstance()->ResistEffect(6, u"Assets/effect/healHP_small.efk");
* EffectEngine::GetInstance()->ResistEffect(7, u"Assets/effect/lefuUp.efk");
* EffectEngine::GetInstance()->ResistEffect(8, u"Assets/effect/bigin.efk");
* EffectEngine::GetInstance()->ResistEffect(9, u"Assets/effect/hit.efk");
* EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/healMP_small.efk");
*/

/*Sound
* g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");
* g_soundEngine->ResistWaveFileBank(1, "Assets/sound/weapon01.wav");
* g_soundEngine->ResistWaveFileBank(1, "Assets/sound/weapon02.wav");
* g_soundEngine->ResistWaveFileBank(1, "Assets/sound/weapon03.wav");
* 
* g_soundEngine->ResistWaveFileBank(5, "Assets/sound/damage.wav");
* g_soundEngine->ResistWaveFileBank(6, "Assets/sound/dead.wav");
* 
* 
* g_soundEngine->ResistWaveFileBank(9, "Assets/sound/heal.wav");
* g_soundEngine->ResistWaveFileBank(10, "Assets/sound/vanish_floor.wav");
* g_soundEngine->ResistWaveFileBank(11, "Assets/sound/bgm_title.wav");
* g_soundEngine->ResistWaveFileBank(12, "Assets/sound/enter.wav");
* g_soundEngine->ResistWaveFileBank(13, "Assets/sound/select.wav");
* 
* 
* 
* 
* g_soundEngine->ResistWaveFileBank(18, "Assets/sound/stage1_bgm.wav");
* 
* 
* 
* 
* g_soundEngine->ResistWaveFileBank(23, "Assets/sound/getItem.wav");
*/

//予定
//880
//1000

//outsideは-500の地点に置く

class Fade;
class GameManager;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//ゲームオーバー時の処理
	void IsGameOver(const int playerLeft);
	//ゲームクリア時の処理
	void IsGameClear();

private:
	SpriteRender	m_playerLeftSpriteRender;
	SpriteRender	m_playerLeftUpSpriteRender;
	SpriteRender	m_playerHPBarSpriteRender;
	SpriteRender	m_playerWeaponFrameSpriteRender;
	Fade*			fade = nullptr;
	GameManager*	gameManager = nullptr;

	SoundSource* m_bgm = nullptr;

	bool			m_isWaitFadeOut = false;		//フェードアウトの待機時間かどうか
	bool			m_isClear = false;				//クリアしたかどうか
	bool			m_isGameOver = false;			//ゲームオーバーかどうか

	int m_saveData[12];
};

