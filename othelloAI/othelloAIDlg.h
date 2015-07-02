
// othelloAIDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"
#include "othello.h"
#include "player.h"
#include "SelectDig.h"

// CothelloAIDlg �_�C�A���O
class CothelloAIDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CothelloAIDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[
	~CothelloAIDlg(){
		if (thread != NULL){
			thread->SuspendThread();
		}
	}

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OTHELLOAI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedStartBtn();
	afx_msg void OnClickedStopBtn();
	afx_msg void OnClickedBoard();
	CStatic pc_board;
	CStatic black_edit;
	CStatic white_edit;
	CButton btn_start;
	CButton btn_stop;
	CStatic pc_black;
	CStatic pc_white;
	void show();
private:
	int board_size;
	const int BLANK = 0;
	const int BLACK = 1;
	const int WHITE = 2;
	bool is_tread_start = false;
	bool stop_flag = false;
	othello o;
	player player1;
	CWinThread *thread = NULL;

	static UINT TreadOthello(LPVOID pParam);
	int black_time = 60;
	int white_time = 60;
	void startGame();
	void setFont();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	void showMassageBox(CString str);
	CStatic pc_arrow;
	bool is_end_game = false;
	CStatic pc_EvaluationValue;
	void showAnything(CString str);
	CButton btn_debug;
	afx_msg void OnClickedDebugBTN();
};
