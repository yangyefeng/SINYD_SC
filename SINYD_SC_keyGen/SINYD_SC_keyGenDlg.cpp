
// SINYD_SC_keyGenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SINYD_SC_keyGen.h"
#include "SINYD_SC_keyGenDlg.h"
#include "afxdialogex.h"
#include <string>
#include "..\SINYD_SC_Crypt\CryptTool.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSINYD_SC_keyGenDlg �Ի���

const std::string privateKey = "30820274020100300D06092A864886F70D01010105000482025E3082025A02010002818100BD67EF1B311EEB88A3B04674E035B10A48B73782B2C2A9BEAD1714326C512C857B25592ED5B9ECEB0944F4A4AF1F3AF067CCA4592C30EAD087076627656FDF9273EFA72B05E755E12509088E67C20A16561479240271FF47BC5FB96422A907990E4EE8A0744FE3672277523A23BD3C03DF2EF7C79770DA17E1917F8D532343B50201110281801BDA941A96484FD0544EA0F311E9C73637DEB556FC2BAF8CFB5637AD0FEDD1D764EEE777D422DF13815CF6CCEC93A6C90034AEC1D1CAF55AE6AE43BA7FDBBEFEB1E386896212D5961233D2E6259722399B5978AD1CDEC8F9C72909B2BD8D3211E0F766FD087A04B69B948F041EBB089EFA8E96C447D84E61466BE13C59849EE1024100F195AD5F65B068FBFF96199A441FE1C9E5D5764AB583D3482B8DF0F3A239D4F2B9E6469487081D052377E436C23AE257B7AFD2DEA76C99EC36E4341C18E76F89024100C8B533BF055073B57678EE70BD9E72C4B64501735569A229E0212017777BDE2CC1097F534D3D3FEEA9DA07E75D57523FE61BF07F0778919632361CA3A64F9BCD024071AFD91DD58031678755B1B201F0E2B95D19289BA0B6818B5FCA53457984A072395D4E64034F1CB71FBFF2EC97A33D56748EF9D230AB93BA742F2794C06CE93102403B081E473DCC5E446E23916C74016D0CAE144BB882887AFD41EBA006E6E8324965F3BC09711203BEAA6D4D9E66BF546D258FBF346B9BEE95964C268A7C35882D024100D53C6B063F033F626AF93E4CAFE8C63D44BE77CB3813D6B4828B28633FA7812D58132D73CE9D79A175FDD76FD3924ED441303DBAF6BDD2D74148E49EE9FFBD9E";

CSINYD_SC_keyGenDlg::CSINYD_SC_keyGenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SINYD_SC_KEYGEN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSINYD_SC_keyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMachineID);
	DDX_Control(pDX, IDC_EDIT2, m_editSerialKey);
	DDX_Control(pDX, IDC_CHECK1, m_INFINTECheck);
	//DDX_Control(pDX, IDC_EDIT3, m_editDueTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DTCtrl);
}

BEGIN_MESSAGE_MAP(CSINYD_SC_keyGenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTNGen, &CSINYD_SC_keyGenDlg::OnBnClickedBtngen)
	ON_BN_CLICKED(IDC_BTNSerialKeyCopy, &CSINYD_SC_keyGenDlg::OnBnClickedBtnserialkeycopy)
	ON_BN_CLICKED(IDC_CHECK1, &CSINYD_SC_keyGenDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CSINYD_SC_keyGenDlg ��Ϣ�������

BOOL CSINYD_SC_keyGenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	m_DTCtrl.SetTime(systime);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSINYD_SC_keyGenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSINYD_SC_keyGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSINYD_SC_keyGenDlg::OnBnClickedBtngen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMachineCode;
	GetDlgItemText(IDC_EDIT1, strMachineCode);
// 	CString strKeyDuration;
// 	GetDlgItemText(IDC_EDIT3, strKeyDuration);
	SYSTEMTIME systime = { 0 };
	m_DTCtrl.GetTime(&systime);
	char duetime[20] = { 0 };
	sprintf_s(duetime, "%04d-%02d-%02d-%02d-%02d-%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);


	if (strMachineCode.GetLength() <= 0)
	{
		MessageBox(L"�����������", L"��ʾ", MB_OK);
	}
// 	else if (strKeyDuration.GetLength() <= 0)
// 	{
// 		MessageBox(L"��������Ȩʱ��", L"��ʾ", MB_OK);
// 	}
	else
	{
		if (strMachineCode.GetLength() != MACHINE_CODE_LEN)
		{
			MessageBox(L"��������Ч����������ȷ�Ļ�����", L"��ʾ", MB_OK);
		}
		else
		{
// 			int iKeyDuration = _ttoi(strKeyDuration);
// 			if (iKeyDuration == 0 || iKeyDuration < -1)
// 			{
// 				MessageBox(L"��Ȩʱ����Ч����������ȷ����Ȩʱ��", L"��ʾ", MB_OK);
// 			}
			if (m_INFINTECheck.GetCheck())
			{
				CCryptTool tool;
				string key = tool.SignText(privateKey, string(CT2A(strMachineCode)), INFINITE_KEY);
				SetDlgItemText(IDC_EDIT2, CA2T(key.c_str()));
			}
			else
			{
				CCryptTool tool;
				string key = tool.SignText(privateKey, string(CT2A(strMachineCode)), duetime);
				SetDlgItemText(IDC_EDIT2, CA2T(key.c_str()));
			}
		}
	}
}


void CSINYD_SC_keyGenDlg::OnBnClickedBtnserialkeycopy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strKey;
	GetDlgItemText(IDC_EDIT2, strKey);
	if (strKey.GetLength() <= 0)
	{
		MessageBox(L"δ����ע���룬�޷�����", L"��ʾ", MB_OK);
	}
	else
	{
		if (!OpenClipboard())
		{
			AfxMessageBox(_T("Cannot open the Clipboard"));
			return;
		}
		// Remove the current Clipboard contents   
		if (!EmptyClipboard())
		{
			AfxMessageBox(_T("Cannot empty the Clipboard"));
			return;
		}

		// global memory of data
		size_t cbStr = (strKey.GetLength() + 1) * sizeof(TCHAR);
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
		memcpy_s(GlobalLock(hData), cbStr, strKey.LockBuffer(), cbStr);
		GlobalUnlock(hData);
		strKey.UnlockBuffer();

		// For the appropriate data formats...
		UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
		if (::SetClipboardData(uiFormat, hData) == NULL)
		{
			AfxMessageBox(_T("Unable to set Clipboard data"));
			CloseClipboard();
			return;
		}

		CloseClipboard();
	}

}


void CSINYD_SC_keyGenDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_INFINTECheck.GetCheck())
	{
		//SetDlgItemText(IDC_EDIT3, L"-1");
		//m_editDueTime.EnableWindow(FALSE);
		m_DTCtrl.EnableWindow(FALSE);
	}
	else
	{
		//SetDlgItemText(IDC_EDIT3, L"");
		//m_editDueTime.EnableWindow(TRUE);
		m_DTCtrl.EnableWindow(TRUE);
	}
	
}
