#include "stdafx.h"
#include "HexBinWorker.h"
#include "HexBinWorkerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MARK: About Dialog 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// MARK: CHexBinWorkerDlg 
CHexBinWorkerDlg::CHexBinWorkerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexBinWorkerDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CHexBinWorkerDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEXEDIT, _hexEdit);
}
BEGIN_MESSAGE_MAP(CHexBinWorkerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CHexBinWorkerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE, &CHexBinWorkerDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_HEX_TO_BIN, &CHexBinWorkerDlg::OnBnClickedHexToBin)
	ON_BN_CLICKED(IDC_BIN_TO_HEX, &CHexBinWorkerDlg::OnBnClickedBinToHex)
	ON_BN_CLICKED(IDC_BTN_FILEREPLICATION, &CHexBinWorkerDlg::OnBnClickedBtnFilereplication)
END_MESSAGE_MAP()


// CHexBinWorkerDlg Message Process
BOOL CHexBinWorkerDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	findAvailableCom();

	_editFont.CreateFont(-12, 0,0,0,0,0,0,0,0,0,0,0,0, _T("Consolas"));
	GetDlgItem(IDC_HEXFILEFIELD)->SetFont(&_editFont);
	
    static_cast<CEdit*>(GetDlgItem(IDC_HEXFILEFIELD))->SetLimitText(0); // =2147483646 (0x7FFFFFFE)

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CHexBinWorkerDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CHexBinWorkerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示
HCURSOR CHexBinWorkerDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


// MARK: -User Interaction
void CHexBinWorkerDlg::showFilePath() {
	// clear
	GetDlgItem(IDC_HEX_PATH)->SetWindowText(_T(""));
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(_T(""));

	CString hexPath, binPath;
	_hbController.getFilePath(hexPath, binPath);

	GetDlgItem(IDC_HEX_PATH)->SetWindowText(hexPath);
	GetDlgItem(IDC_BIN_PATH)->SetWindowText(binPath);
}
void CHexBinWorkerDlg::showHexEditText(bool needClear) {
	if (needClear){
        GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(_T(""));  // clear
	}
	
	CString hexText;
	_hbController.getHexText(hexText);
	GetDlgItem(IDC_HEXFILEFIELD)->SetWindowText(hexText);
}
void CHexBinWorkerDlg::showBinEditText(bool needClear) {
    if (needClear) {
        _hexEdit.Clear();
    }

	BYTE* pDatas = NULL;
	int dataSize = 0;
	_hbController.getBinDatas(pDatas, dataSize);

	_hexEdit.SetData(pDatas, dataSize);
}
void CHexBinWorkerDlg::getHexEditText(string& hexText){
	CString hexTextCStr;
	GetDlgItem(IDC_HEXFILEFIELD)->GetWindowText(hexTextCStr);
	hexText = CT2A(hexTextCStr, CP_UTF8);
}
void CHexBinWorkerDlg::getBinEditText(BYTE *pDatas, int dataSize) {
	_hexEdit.GetData(pDatas, dataSize);
}

// MARK: -Button
void CHexBinWorkerDlg::OnBnClickedOk()
{
	// Open
	CString fileFormatter(_T("Inter Hex (*.hex)|*.hex|Bin File (*.bin)|*.bin||"));
	CFileDialog pHexFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, fileFormatter); 

	if(pHexFileDlg.DoModal() == IDOK) {

		CString filePathName = pHexFileDlg.GetPathName(); 

		// Process
		_hbController.init(filePathName);
		_hbController.read(filePathName);

		// Show 
		showHexEditText();
		showBinEditText(true);

		showFilePath();
	}
}
void CHexBinWorkerDlg::OnBnClickedHexToBin() {
	_hbController.typeHexToBin();

	string hexText;
	getHexEditText(hexText);

	bool parseOK = _hbController.parseHex(hexText); //TODO: block with 3 sec
	if (!parseOK) {
		MessageBox(_T("Hex 文件无法解析到 Bin 文件，格式错误"));
        return;
	}

	showBinEditText();
}
void CHexBinWorkerDlg::OnBnClickedBinToHex() {
	_hbController.typeBinToHex();

	BYTE* pDatas = NULL;
	int dataSize = 0;

	_hbController.getBinDatas(pDatas, dataSize); 
	_hexEdit.GetData(pDatas, dataSize);
	bool parseOK = _hbController.parseBin(pDatas, dataSize);
	if (!parseOK) {
		MessageBox(_T("bin 文件无法解析到 HEX 文件"));
        return;
	}

	showHexEditText();
}
void CHexBinWorkerDlg::OnBnClickedSave() {
	// refresh hex data
	string hexText;
	getHexEditText(hexText);
	
	bool verifyOK = _hbController.parseHex(hexText);
	if (!verifyOK) {
		MessageBox(_T("检测到 Hex 文件格式存在错误，中止全部保存"));
		return;
	}

	// refreash bin data
	BYTE* pDatas = NULL;
	int dataSize = 0;
	_hbController.getBinDatas(pDatas, dataSize);
    getBinEditText(pDatas, dataSize);
    _hbController.setBinDatas(pDatas, dataSize);
	

	// save
	bool writeHexOK = _hbController.writeHex();
	bool writeBinOK = _hbController.writeBin();

	if (writeHexOK && writeBinOK) {
		MessageBox(_T("Intex Hex 和对应 Bin 文件保存成功"), NULL);
	} else {
		MessageBox(_T("保存失败"));
	}
}
void CHexBinWorkerDlg::OnBnClickedBtnFilereplication()
{
    // get COM Serial Number
    CString comSerialStr;
	int currentComSerial = ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCurSel();
	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetLBText(currentComSerial, comSerialStr);
    CString comNumberCStr = comSerialStr.Mid(3);
	int comNumber = _ttoi(comNumberCStr);

    // open COM
    bool openOK = _comController.openCom(comNumber);
    if (!openOK) {
        CString errMessage;
        errMessage.Format(_T("无法打开串口：COM%d"), comNumber);
        MessageBox(errMessage);
        return;
    }

    // get data
    BYTE* pDatas = NULL;
	int dataSize = 0;
    _hbController.getBinDatas(pDatas, dataSize);

    if (dataSize == 0) {
        _hbController.parse();
        _hbController.getBinDatas(pDatas, dataSize);

        if (dataSize == 0) {
            MessageBox(_T("未发现烧录的数据"));
            return;
        }
    }

    // TODO: how to reset ARM status to recive write command again?

    //bool eraseOK = _comController.eraseMemory();
    //if (!eraseOK) {
    //    MessageBox(_T("无法擦除 Flash"));
    //}

    //_comController.getCommand();

    // write data
    bool writeOK = _comController.writeMemory(pDatas, dataSize);
    if (writeOK) {
        MessageBox(_T("当前 Intel Hex 文件烧录成功"));
    } else {
        MessageBox(_T("烧录过程中发生错误，未烧录"));
    }
}

// Mark: -Com 
void CHexBinWorkerDlg::findAvailableCom() {

	HANDLE hCom;
	// find com number [COM1-16]
	const int comSerialSize = 16;
	vector<CString> availableComSerial;
	
	for (int iComNumber = 1; iComNumber <= comSerialSize; iComNumber++) {
		CString comNumber;
		comNumber.Format(_T("COM%d"), iComNumber);

		hCom = CreateFile(comNumber, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (hCom != INVALID_HANDLE_VALUE) {
			availableComSerial.push_back(comNumber);
		}

		CloseHandle(hCom);
	}


	showAvailableCom(availableComSerial);
}
void CHexBinWorkerDlg::showAvailableCom(const vector<CString> &aCom) {
	const int availableComSize = aCom.size();
	if (availableComSize == 0) {
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(FALSE);
		MessageBox(_T("没有侦测到可用的串口"));
	}

	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(TRUE);
	for (int i=0; i<availableComSize; i++) {
		((CComboBox *)GetDlgItem(IDC_COMBO_COM))->AddString(aCom[i]);
	}

	int comCount = ((CComboBox *)GetDlgItem((IDC_COMBO_COM)))->GetCount();
	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(comCount - 1);
}

