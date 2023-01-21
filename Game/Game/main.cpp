#include "stdafx.h"
#include "system/system.h"

#include<InitGUID.h>
#include<dxgidebug.h>

#include "GameManager.h"
#include "Game.h"
#include "Fade.h"

#include "Title.h"
#include "GameOver.h"



void ReportLiveObjects()
{
	IDXGIDebug* pDxgiDebug = nullptr;

	typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);

	// 出力。
	pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
}

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Unity_Chan"));
	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//Gameクラスのオブジェクトを作成。
	NewGO<GameManager>(0, "gameManager");
	NewGO<Fade>(2, "fade");

	//NewGO<Game>(0, "game");
	NewGO<Title>(0, "title");
	//NewGO<GameOver>(0, "gameOver");

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//if (g_pad[0]->IsTrigger(enButtonA) ){
		//	g_pad[0]->SetVibration(/*durationSec=*/0.5f, /*normalizedPower=*/1.0f);
		//}
		K2Engine::GetInstance()->Execute();
	}

	K2Engine::DeleteInstance();

#ifdef _DEBUG
	ReportLiveObjects();
#endif // _DEBUG
	return 0;
}

