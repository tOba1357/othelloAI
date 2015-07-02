#pragma once
#include "afxwin.h"
#include "othello.h"
#include <vector>
// CSelectDig �_�C�A���O

class CSelectDig : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDig)

public:
	CSelectDig(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectDig();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	othello *o;
	void setOthello(othello *a);

	afx_msg void OnClickedOKBtn();
	std::vector<CButton*> radio_balck, radio_white;
	int select_first;
	int select_second;
	int nextID = 1300;
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
