
// othelloAI.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CothelloAIApp:
// このクラスの実装については、othelloAI.cpp を参照してください。
//

class CothelloAIApp : public CWinApp
{
public:
	CothelloAIApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CothelloAIApp theApp;