#pragma once
#include "afxwin.h"
#include <list>
#include <map>
#include "..\SINYD_SC_CommonUtility\CCalibrationConfig.h"
#include "..\SINYD_SC_CommonUtility\CSettingConfig.h"
#include "..\SINYD_SC_CommonUtility\BitmapEx.h"
//#include "..\InterfaceInclude\Measurelogic.h"
//#include "..\InterfaceInclude\Measure.h"
#include "SCWriteConfigDef.h"
#include "CommonDef.h"
#include "RWData.h"

#define WM_MYMSG WM_USER + 110 //自定义消息，用于处理鼠标钩子发来的消息



typedef enum enWindowStatus
{
	STATUS_UNKNOWN,
	STATUS_FOREGROUND,
	STATUS_BACKGROUND,
	STATUS_MINIMIZE
}WINDOWSTATUS;

typedef struct _AreaRectsMap
{
	int areaID;
	RECT AreaRect;

} AreaRectsMap;

typedef struct _MeasurePointConfig
{
	_MeasurePointConfig() : _start(0), _start_delay(0), _end(0), _end_delay(0), _calc_method(0) {}
	_MeasurePointConfig(int start, int start_delay, int end, int end_delay, int calc_method)
		: _start(start), _start_delay(start_delay), _end(end), _end_delay(end_delay), _calc_method(calc_method) {}
	int _start;
	int _start_delay;
	int _end;
	int _end_delay;
	int _calc_method;

} MeasurePointConfig;

// SCCalibrationUIDlg 对话框

class SCCalibrationUIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SCCalibrationUIDlg)

public:
	SCCalibrationUIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SCCalibrationUIDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINYD_SC_CALIUI_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
private:
	CEdit edtSoftwareName;
	CEdit edtSoftwarePath;
	CEdit edtWindowName;
	CEdit edtWindowClass;
	CEdit edtWindowHeight;
	CEdit edtWindowWidth;
	//CButton btnExePath;
	CButton btnScreenshot;
	CButton btnWindowHandle;
	CButton btnSave;
	CEdit edtVSoftwareVer;
	CMenu m_Menu;
	HCURSOR m_hCursor;
	CStatic picScreen;
	CButton chkIsAdmixtureTo;
	CButton chkLocalBmp;
	CButton btnBmp;

	CCalibrationConfig CaliConfig;
	vector<AreaRectsMap> AreaRectMaps;

	//截图相关参数
	bool bGetWindow;
	HWND m_hScreenWindowHandle;
	HDC m_hdcWindow;
	HDC m_hdcMemDC;
	HBITMAP m_hbmScreen;
	CBitmapEx bitmapCali;
	CBitmapEx bitmapShow;
	CString localBmpPath;
	vector<CString> pngFileList;
	int pngCurPos;
	//static const int areaPicSide = 250;

	//鼠标框选相关参数
	bool mouseSelect;
	CPoint PDown, PMove;
	CPen pen;
	CPen * oldpen;

	//保存配置文件相关参数
	map<CString, HopperStru> mapHopper;
	list<AnalogSensor> AnalogList;
	list<DigitalSensor> DigitalList;
	BucketStru bucketHopper;
	MixerStru mixerHopper;
	int digitalsensor_total;
	int analogsensor_total;
	int weight_total;
	int bucket_total;
	int hopper_total;
	char* mix_machine_id;
	int admixtureTo;
	vector<HWND> m_parents;
	int m_parentPos;

	HHOOK hhookMs = NULL;
	BOOL UninstallKbHook();
	BOOL InstallKbHook(HWND hWnd);
	void ReleaseAllHDC();
	WINDOWSTATUS GetWindowStatus();
	bool CaptureBackgroudWindowByHook(CBitmapEx & bitmap);
	bool GetDesktopScreen(CBitmapEx & bitmap);
	HBITMAP GetScreenShot();
	void RefreshAllHDC(HWND hWnd);
	bool CaptureBackgroudWindow();
	void GetWindowHandle();
	int GetExePathFromHWND(HWND hwnd, CString& ExePath);

	bool InitCaliConfig();
	void InitAreaRects();
	void ShowDlg(const CCalibrationConfig* caliConfig);
	bool ReadDlgToEntry(CCalibrationConfig* caliConfig);
	void ShowPicture();
	void ShowRectangle();
	void ShowLocalBmp(CString localPath, bool isPNG);
	void read_png_to_bmp(const TCHAR* path, CBitmapEx& bitmap);

	void CancelRectangle(CPoint POrigin, CPoint PEnd);
	//float CalcAreaCoef(int height, int width);
	//RECT MakeRect(CPoint POrigin, CPoint PEnd);

	void GetMixInfo();
	void WriteConfigFile();
	void ClearWriteConfPara();
	void CreatHopper(HopperStru* hopper, CString hopperName);
	void InitMapHopper();
	void InitOtherHopper();
	void ClearHopper();
	void ClearSensorConfList();
	void BulidSensorConfList();
	void BulidHopperConf();
	void FillMapHopper(const MonitorArea* area, CString hopperName);
	void WriteMeasureConfig(CRWData& rw, char* serialTrue, char* anFileName, int measureType, const vector<MeasurePointConfig>& points);
	void WriteMeasureConfig(CRWData& rw, char* serialTrue, char* anFileName, const AreaMeasureFunction& func);
	void SaveSensorConf();
	void SaveHopperConf();
	void SaveMixSysConf();

public:
	afx_msg void OnMenubtnOpenold();
	afx_msg void OnMenubtnNew();
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();

	afx_msg LRESULT OnMyMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnPaint();
	//afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnMenubtnSaveas();

	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton1();
	void RefreshPNGList(const CString& localBmpPath);
	CComboBox m_ComboBoxCaptureType;
	afx_msg void OnBnClickedButtonBefore();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonBasebmp();
	afx_msg void OnBnClickedButtonParenthwnd();
	afx_msg void OnBnClickedButtonChildhwnd();
};

static HWND g_hWnd = NULL;
LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam);

