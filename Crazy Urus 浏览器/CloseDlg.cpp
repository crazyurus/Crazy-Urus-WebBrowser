// CloseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebBrowser.h"
#include "CloseDlg.h"
#include "afxdialogex.h"


// CCloseDlg �Ի���

IMPLEMENT_DYNAMIC(CCloseDlg, CDialogEx)

CCloseDlg::CCloseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCloseDlg::IDD, pParent)
{

}

CCloseDlg::~CCloseDlg()
{
}

void CCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCloseDlg, CDialogEx)
    ON_BN_CLICKED(IDALL, &CCloseDlg::OnBnClickedAll)
    ON_BN_CLICKED(IDCUR, &CCloseDlg::OnBnClickedCur)
END_MESSAGE_MAP()


// CCloseDlg ��Ϣ�������

void CCloseDlg::OnBnClickedAll()
{
    EndDialog(IDALL);
    SendMessage(WM_CLOSE);
}

void CCloseDlg::OnBnClickedCur()
{
    EndDialog(IDCUR);
    SendMessage(WM_CLOSE);
}
