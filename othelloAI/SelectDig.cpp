// SelectDig.cpp : 実装ファイル
//

#include "stdafx.h"
#include "othelloAI.h"
#include "SelectDig.h"
#include "afxdialogex.h"


// CSelectDig ダイアログ

IMPLEMENT_DYNAMIC(CSelectDig, CDialogEx)

CSelectDig::CSelectDig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectDig::IDD, pParent)
{

}

CSelectDig::~CSelectDig()
{
	for (int i = 0; i < radio_balck.size(); i++){
		delete radio_balck[i];
		delete radio_white[i];
	}
}

void CSelectDig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectDig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSelectDig::OnClickedOKBtn)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSelectDig メッセージ ハンドラー

void CSelectDig::setOthello(othello *a){
	o = a;
}


void CSelectDig::OnClickedOKBtn()
{
	int first, second;
	for (int i = 0; i < radio_balck.size(); i++){
		if (radio_balck[i]->GetState() == BST_CHECKED) first = i;
		if (radio_white[i]->GetState() == BST_CHECKED) second = i;
	}
	o->setAI(first, second);
	EndDialog(1);
}


int CSelectDig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	int x, y;
	x = 70; y = 70;
	for (int i = 0; i < o->AIs.size(); i++){
		radio_balck.push_back(new CButton);
		if (i == 0){
			radio_balck[i]->Create(o->AIs[i]->getName(), WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, CRect(x, y, x+100, y+20), this, nextID);
		}
		else{
			radio_balck[i]->Create(o->AIs[i]->getName(), WS_VISIBLE | BS_AUTORADIOBUTTON | WS_CHILD, CRect(x, y + i*30, x+100, y+20 + i*30), this, nextID);
		}
		nextID++;
	}

	x = 280; y = 70;
	for (int i = 0; i < o->AIs.size(); i++){
		radio_white.push_back(new CButton);
		if (i == 0){
			radio_white[i]->Create(o->AIs[i]->getName(), WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, CRect(x, y, x + 100, y + 20), this, nextID);
		}
		else{
			radio_white[i]->Create(o->AIs[i]->getName(), WS_VISIBLE | BS_AUTORADIOBUTTON | WS_CHILD, CRect(x, y + i * 30, x + 100, y + 20 + i * 30), this, nextID);
		}
		nextID++;
	}
	return 0;
}
