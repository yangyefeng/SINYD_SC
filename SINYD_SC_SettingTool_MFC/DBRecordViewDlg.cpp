// DBRecordViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SINYD_SC_SettingTool_MFC.h"
#include "DBRecordViewDlg.h"
#include "afxdialogex.h"
#include <corecrt_io.h>
#include "DBModule.h"
#include <atlconv.h>
#include "CppSqlite3\CppSQLite3.h"
#include "MeasureDef.h"


// CDBRecordViewDlg 对话框

IMPLEMENT_DYNAMIC(CDBRecordViewDlg, CDialogEx)

CDBRecordViewDlg::CDBRecordViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DBView, pParent)
{

}

CDBRecordViewDlg::~CDBRecordViewDlg()
{
}

void CDBRecordViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DBRecord, m_recordListCtrl);
	DDX_Control(pDX, IDC_LIST_DBFiles, m_dbFiles);
}


BEGIN_MESSAGE_MAP(CDBRecordViewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RefreshRecord, &CDBRecordViewDlg::OnBnClickedButtonRefreshrecord)
	ON_BN_CLICKED(IDC_BUTTON_RefreshFile, &CDBRecordViewDlg::OnBnClickedButtonRefreshfile)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DBFiles, &CDBRecordViewDlg::OnNMDblclkListDbfiles)
END_MESSAGE_MAP()


// CDBRecordViewDlg 消息处理程序


BOOL CDBRecordViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowWindow(SW_MAXIMIZE);

	m_dbFiles.InsertColumn(0, L"DB文件", 0, 170);
	m_dbFiles.InsertColumn(1, L"传输状态", 0, 150);

	RefreshDBFileList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDBRecordViewDlg::RefreshDBFileList()
{
	m_dbFiles.DeleteAllItems();

	bool bGetBP = false;
	string DataFileName;
	int DataID;
	time_t DataTime;

	string fileName = string(CONFIG_RUNTIME_PATH) + "SC_TransBreakPoint.db";
	try {
		if (PathFileExistsA(fileName.c_str())) {
			CppSQLite3DB db;
			db.open(fileName.c_str());

			string selectSql = "select * from BreakPointData where FieldName='" STR_PRODUCT_BD_NAME "';";
			CppSQLite3Query sqlQuery = db.execQuery(selectSql.c_str());
			const char* retValue = NULL;
			if (!sqlQuery.eof()) {
				DataFileName = string(sqlQuery.getStringField(1));
				DataID = sqlQuery.getIntField(2);

				tm tm_ = { 0 };
				sscanf(DataFileName.c_str() + (strlen(DataFileName.c_str()) - 22), "%d-%d-%d %d_%d_%d.db", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
				tm_.tm_year = tm_.tm_year - 1900;
				tm_.tm_mon = tm_.tm_mon - 1;
				DataTime = mktime(&tm_);

				bGetBP = true;
			}
		}
	}
	catch (...) {
		
	}


	_finddata_t fileDir;
	long lfDir;

	if ((lfDir = _findfirst(".\\DB\\*.db", &fileDir)) != -1l)
	{
		do {
			if (fileDir.attrib & _A_SUBDIR) {
				continue;
			}
			if (strstr(fileDir.name, ".db") != NULL && strstr(fileDir.name, "product") != NULL)
			{
				tm tm_ = { 0 };
				sscanf(fileDir.name + (strlen(fileDir.name) - 22), "%d-%d-%d %d_%d_%d.db", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
				tm_.tm_year = tm_.tm_year - 1900;
				tm_.tm_mon = tm_.tm_mon - 1;
				time_t time = mktime(&tm_);

				m_dbFiles.InsertItem(0, NULL);
				m_dbFiles.SetItemText(0, 0, CA2T(fileDir.name));

				if (bGetBP)
				{
					if (time < DataTime)
					{
						m_dbFiles.SetItemText(0, 1, L"已传输");
					}
					else if (time > DataTime)
					{
						m_dbFiles.SetItemText(0, 1, L"未传输");
					}
					else
					{
						CString stat;
						stat.Format(L"正在传输至%d", DataID);
						m_dbFiles.SetItemText(0, 1, stat);
					}
				}
				else
				{
					m_dbFiles.SetItemText(0, 1, L"未传输");
				}

			}
		} while (_findnext(lfDir, &fileDir) == 0);
	}
	_findclose(lfDir);

	int netstat = GetPrivateProfileInt(L"Socket", L"connect_state", 0, L".\\ConfigFile_RunTime\\SocketState.ini");
	if (netstat == 1)
	{
		SetDlgItemText(IDC_STATIC_NetStat, L"已连接");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_NetStat, L"未连接");
	}
}


void CDBRecordViewDlg::OnBnClickedButtonRefreshrecord()
{
	// TODO: 在此添加控件通知处理程序代码

	while (m_recordListCtrl.DeleteColumn(0));
	m_recordListCtrl.DeleteAllItems();

	if (PathFileExists(m_curDBFileName))
	{
		IDBOperator* pdbo = OpenDBOperator(CT2A(m_curDBFileName).m_psz, true);
		if (pdbo)
		{
			m_recordListCtrl.InsertColumn(0, L"序号", 0, 30);

			DBInfo info = pdbo->GetDBInfo();
			auto itEnd = info.fields.end();
			int num = 1;
			for (auto it = info.fields.begin(); it != itEnd; it++)
			{
				string colname = it->fieldOriginalName;
				if (it->fieldName.find("_C") != -1)
				{
					colname += "计算值";
				}
				else if (it->fieldName.find("_R") != -1)
				{
					colname += "称重值";
				}
				else if (it->fieldName.find("_D") != -1)
				{
					colname += "设定值";
				}
				m_recordListCtrl.InsertColumn(num, CA2T(colname.c_str()), 0, 100);
				num++;
			}

			IDBQueryRow* prow = pdbo->Query(0, -1);
			if (prow)
			{
				while (!prow->eof())
				{
					m_recordListCtrl.InsertItem(0, NULL);
					TCHAR pk[10] = { 0 };
					_itot(prow->RowPK(), pk, 10);
					m_recordListCtrl.SetItemText(0, 0, pk);
					for (int i = 1; i < num; i++)
					{
						const char* data = prow->GetStringDataField(i - 1);
						m_recordListCtrl.SetItemText(0, i, data ? CA2T(data) : L"");
					}

					prow->nextRow();
				}

				pdbo->QueryFinalize(prow);
			}

			ReleaseDBOperator(pdbo);
		}
	}
}



void CDBRecordViewDlg::OnBnClickedButtonRefreshfile()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshDBFileList();
}


void CDBRecordViewDlg::OnNMDblclkListDbfiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (m_dbFiles.GetSelectedCount() <= 0)
	{
		return;
	}

	TCHAR fileName[MAX_PATH] = { 0 };
	m_curDBFileName = m_dbFiles.GetItemText(m_dbFiles.GetSelectionMark(), 0);
	m_curDBFileName = L".\\DB\\" + m_curDBFileName;
	OnBnClickedButtonRefreshrecord();
}
