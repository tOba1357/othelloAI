
// othelloAIDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "othelloAI.h"
#include "othelloAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CothelloAIDlg �_�C�A���O


CothelloAIDlg::CothelloAIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CothelloAIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	thread = NULL;
}

void CothelloAIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT, pc_board);
	DDX_Control(pDX, IDC_BUTTON1, btn_start);
	DDX_Control(pDX, IDC_BUTTON2, btn_stop);
	DDX_Control(pDX, IDC_EDIT1, pc_black);
	DDX_Control(pDX, IDC_EDIT2, pc_white);
}

BEGIN_MESSAGE_MAP(CothelloAIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CothelloAIDlg::OnClickedStartBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &CothelloAIDlg::OnClickedStopBtn)
	ON_STN_CLICKED(IDC_EDIT, &CothelloAIDlg::OnClickedBoard)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON3, &CothelloAIDlg::OnClickedDebugBTN)
END_MESSAGE_MAP()


// CothelloAIDlg ���b�Z�[�W �n���h���[

BOOL CothelloAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CothelloAIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//�_�C�A���O�S�̂̑傫���擾
		CRect rect;
		GetClientRect(&rect);

		//�{�[�h�̃T�C�Y�ʒu����
		int leng = min(rect.Width() - 100, rect.Height()) - 20;
		pc_board.MoveWindow(10, 10, leng, leng);
		board_size = leng - 10;

		//�{�^���A�e�L�X�g�{�b�N�X�̈ʒu����
		int x, y;
		int width = 170;
		leng = rect.Width() - leng - 20;
		x = leng / 2 - width/2 + leng + 40;
		y = 40;
		pc_black.MoveWindow(x, y, width, 25);

		y = 70;
		pc_white.MoveWindow(x, y, width, 25);

		x = rect.Width() - 120;
		y = rect.Height() - 50;
		btn_stop.MoveWindow(x, y, 100, 40);
		btn_start.MoveWindow(x - 120, y, 100, 40);
		show();

		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CothelloAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CothelloAIDlg::OnClickedStartBtn()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CSelectDig sd;
	sd.setOthello(&o);
	if (sd.DoModal() != 1) return;
	o.setDef();
	if (thread != NULL){
		thread->SuspendThread();
	}
	thread = AfxBeginThread(TreadOthello, this);
}


void CothelloAIDlg::OnClickedStopBtn()
{
	if (thread != NULL){
		thread->SuspendThread();
	}
}


void CothelloAIDlg::OnClickedBoard()
{
	CPoint cp;
	GetCursorPos(&cp);
	CRect rect;
	pc_board.GetWindowRect(&rect);
	int board_square_size = board_size / 8.0 + 2;
	int x = (cp.x - rect.left) / board_square_size;
	int y = (cp.y - rect.top) / board_square_size;
	if (o.isPut(x+1, y+1, o.getTurn())){
		o.AIs[0]->setOrder(x, y);
	}
	else{
		CString str;
		str.Format(_T("%d,%d�ɂ͑łĂ܂���B"), x+1, y+1);
		MessageBox(str);
	}
}

void CothelloAIDlg::show(){

	CDC* pDC = pc_board.GetDC();

	//�f�o�C�X�R���e�L�X�g�̑傫���擾
	CRect rect;
	pc_board.GetClientRect(&rect);

	//�{�[�h�̐F�i�΁j
	CPen pen_board(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush br_board(RGB(51, 153, 0));

	CPen* oldpen = pDC->SelectObject(&pen_board);
	CBrush* oldbr = pDC->SelectObject(&br_board);

	//picturboard�̑傫�����擾
	const int height = rect.bottom - rect.top;
	const int width = rect.right - rect.left;
	int line_width;
	if (height > width) line_width = width / 8;
	else line_width = height / 8;

	//�ݒu�ʒu�̌���
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			int lx, ly, rx, ry;
			lx = i*line_width;
			ly = j*line_width;
			rx = (i + 1)*line_width;
			ry = (j + 1)*line_width;
			pDC->Rectangle(lx, ly, rx, ry);
		}
	}
	CPen pen_black(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush br_black(RGB(0, 0, 0));
	CPen pen_white(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush br_white(RGB(255, 255, 255));
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			int lx, ly, rx, ry;
			lx = i*line_width + 2;
			ly = j*line_width + 2;
			rx = (i + 1)*line_width - 2;
			ry = (j + 1)*line_width - 2;
			if (o.read_board[j][i] == BLACK){
				pDC->SelectObject(&pen_black);
				pDC->SelectObject(&br_black);
				pDC->Ellipse(lx, ly, rx, ry);
			}
			if (o.read_board[j][i] == WHITE){
				pDC->SelectObject(&pen_white);
				pDC->SelectObject(&br_white);
				pDC->Ellipse(lx, ly, rx, ry);
			}
		}
	}

	//�΂̐��\��
	int black_num = o.getBlackNum();
	int white_num = o.getWhiteNum();
	CString tmp;


	//������
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbr);

	//�ԐF�쐬
	CPen pen_red(PS_SOLID, 1, RGB(255, 0, 0));
	CBrush br_red(RGB(255, 0, 0));

	int turn = o.getTurn();
	//�΂̐��\��
	pc_black.GetClientRect(&rect);
	pDC = pc_black.GetDC();
	//�w�i�𔒂�
	pDC->Rectangle(&rect);
	oldpen = pDC->SelectObject(&pen_red);
	oldbr = pDC->SelectObject(&br_red);
	if (turn == BLACK){
		//pDC->Rectangle(0, 0, rect.bottom, rect.bottom);
		pDC->Ellipse(2, 2, rect.bottom-2, rect.bottom-2);
	}
	pDC->SelectObject(&pen_black);
	pDC->SelectObject(&br_black);
	pDC->Ellipse(rect.bottom+2, 2, rect.bottom * 2-2, rect.bottom-2);
	tmp.Format(_T("%-10s�F%3d"), o.getBlackName(), black_num);
	pDC->TextOut(rect.bottom*2+1, 2, tmp);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbr);
	
	pc_white.GetClientRect(&rect);
	pDC = pc_white.GetDC();
	pDC->Rectangle(&rect);
	oldpen = pDC->SelectObject(&pen_red);
	oldbr = pDC->SelectObject(&br_red);
	if (turn == WHITE){
		pDC->Ellipse(2, 2, rect.bottom-2, rect.bottom-2);
	}
	oldpen = pDC->SelectObject(&pen_black);
	oldbr = pDC->SelectObject(&br_white);
	pDC->Ellipse(rect.bottom+2, 2, rect.bottom*2-2, rect.bottom-2);
	tmp.Format(_T("%-10s�F%3d"), o.getWhiteName(), white_num);
	pDC->TextOut(rect.bottom*2+1, 2, tmp);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbr);

	UpdateWindow();

}



UINT CothelloAIDlg::TreadOthello(LPVOID pParam)
{
	CothelloAIDlg* pObject = (CothelloAIDlg*)pParam;

	if (pObject == NULL || !pObject->IsKindOf(RUNTIME_CLASS(CothelloAIDlg))) return 1;   // if pObject is not valid
	while (true){
		pObject->show();
		if (pObject->stop_flag) return 0;
		int turn = pObject->o.getTurn();
		if (turn == pObject->BLANK) break;
		pObject->o.nextStep();
		//SleepEx(500, true);
	}

	int winner = pObject->o.getWiner();
	CString str;
	int black_num = pObject->o.getBlackNum();
	int white_num = pObject->o.getWhiteNum();
	if (winner == pObject->BLANK){
		str.Format(_T("��������\n%d-%d"), black_num, white_num);
	}
	else{
		if (winner == pObject->BLACK)
			str.Format(_T("�� %s�̏���\n%d-%d"), pObject->o.getBlackName(), black_num, white_num);
		else
			str.Format(_T("�� %s�̏���\n%d-%d"), pObject->o.getWhiteName(), black_num, white_num);
	}
	pObject->MessageBox(str);
	pObject->is_end_game = true;
	pObject->is_tread_start = false;

	return 0;
}

void CothelloAIDlg::setFont()
{
	//�t�H���g�̐ݒ�
	LOGFONT log_font;
	log_font.lfHeight = 10;
	log_font.lfWidth = 10;
	log_font.lfEscapement = 0;
	log_font.lfOrientation = 0;
	log_font.lfWeight = FW_BOLD;
	log_font.lfItalic = FALSE;
	log_font.lfUnderline = FALSE;
	log_font.lfStrikeOut = FALSE;
	log_font.lfCharSet = DEFAULT_CHARSET;
	log_font.lfOutPrecision = OUT_DEFAULT_PRECIS;
	log_font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	log_font.lfQuality = DEFAULT_QUALITY;
	log_font.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy_s(log_font.lfFaceName, _T("�l�r �o�S�V�b�N"));
	CFont font;
	font.CreatePointFont(1000, _T("�l�r ����"));
	font.CreateFontIndirectW(&log_font);
	btn_start.SetFont(&font);
}


void CothelloAIDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	OnPaint();
}



void CothelloAIDlg::OnClickedDebugBTN()
{



}
