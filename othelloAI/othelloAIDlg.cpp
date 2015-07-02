
// othelloAIDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "othelloAI.h"
#include "othelloAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CothelloAIDlg ダイアログ


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


// CothelloAIDlg メッセージ ハンドラー

BOOL CothelloAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CothelloAIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//ダイアログ全体の大きさ取得
		CRect rect;
		GetClientRect(&rect);

		//ボードのサイズ位置調整
		int leng = min(rect.Width() - 100, rect.Height()) - 20;
		pc_board.MoveWindow(10, 10, leng, leng);
		board_size = leng - 10;

		//ボタン、テキストボックスの位置調整
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

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CothelloAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CothelloAIDlg::OnClickedStartBtn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
		str.Format(_T("%d,%dには打てません。"), x+1, y+1);
		MessageBox(str);
	}
}

void CothelloAIDlg::show(){

	CDC* pDC = pc_board.GetDC();

	//デバイスコンテキストの大きさ取得
	CRect rect;
	pc_board.GetClientRect(&rect);

	//ボードの色（緑）
	CPen pen_board(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush br_board(RGB(51, 153, 0));

	CPen* oldpen = pDC->SelectObject(&pen_board);
	CBrush* oldbr = pDC->SelectObject(&br_board);

	//picturboardの大きさを取得
	const int height = rect.bottom - rect.top;
	const int width = rect.right - rect.left;
	int line_width;
	if (height > width) line_width = width / 8;
	else line_width = height / 8;

	//設置位置の決定
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

	//石の数表示
	int black_num = o.getBlackNum();
	int white_num = o.getWhiteNum();
	CString tmp;


	//初期化
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbr);

	//赤色作成
	CPen pen_red(PS_SOLID, 1, RGB(255, 0, 0));
	CBrush br_red(RGB(255, 0, 0));

	int turn = o.getTurn();
	//石の数表示
	pc_black.GetClientRect(&rect);
	pDC = pc_black.GetDC();
	//背景を白に
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
	tmp.Format(_T("%-10s：%3d"), o.getBlackName(), black_num);
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
	tmp.Format(_T("%-10s：%3d"), o.getWhiteName(), white_num);
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
		str.Format(_T("引き分け\n%d-%d"), black_num, white_num);
	}
	else{
		if (winner == pObject->BLACK)
			str.Format(_T("黒 %sの勝利\n%d-%d"), pObject->o.getBlackName(), black_num, white_num);
		else
			str.Format(_T("白 %sの勝利\n%d-%d"), pObject->o.getWhiteName(), black_num, white_num);
	}
	pObject->MessageBox(str);
	pObject->is_end_game = true;
	pObject->is_tread_start = false;

	return 0;
}

void CothelloAIDlg::setFont()
{
	//フォントの設定
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
	wcscpy_s(log_font.lfFaceName, _T("ＭＳ Ｐゴシック"));
	CFont font;
	font.CreatePointFont(1000, _T("ＭＳ 明朝"));
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
