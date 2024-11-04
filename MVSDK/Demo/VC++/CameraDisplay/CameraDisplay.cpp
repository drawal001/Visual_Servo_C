
// CameraDisplay.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CameraDisplay.h"
#include "CameraDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCameraDisplayApp

BEGIN_MESSAGE_MAP(CCameraDisplayApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCameraDisplayApp ����

CCameraDisplayApp::CCameraDisplayApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCameraDisplayApp ����

CCameraDisplayApp theApp;
int gLanguage = 0;


int GetRegLanguage()
{
	LONG status;
	char strPath[MAX_PATH];
	char szCompany[64];
	HKEY hkey = NULL;
	DWORD dwType = REG_SZ;
	DWORD dwSize = MAX_PATH;
	BYTE abyValue[MAX_PATH];

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Industry Camera", 0, KEY_READ, &hkey))
		return 0;
	dwSize = sizeof(szCompany);
	status = RegQueryValueEx(hkey, "Company", NULL, &dwType, (LPBYTE)szCompany, &dwSize);
	RegCloseKey(hkey);
	hkey = NULL;
	if (status != ERROR_SUCCESS)
		return 0;

	sprintf(strPath, "Software\\%s\\Settings", szCompany);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, strPath, 0, KEY_READ, &hkey);

	if (NULL != hkey)
	{
		memset(abyValue, 0x00, MAX_PATH);
		dwType = REG_SZ;
		dwSize = MAX_PATH;
		LONG status = RegQueryValueEx(hkey, "Language", NULL, &dwType, abyValue, &dwSize);
		RegCloseKey(hkey);

		if (ERROR_SUCCESS == status)
		{
			abyValue[MAX_PATH-1] = '\0';

			if (strcmp((const char *)abyValue,"Chinese") == 0)
			{
				return 1;
			}
			else if(strcmp((const char *)abyValue,"TradChinese") == 0)
			{
				return 2;
			}
			else
			{
				return 0;
			}

		}
	}

	return 0;
}

// CCameraDisplayApp ��ʼ��

BOOL CCameraDisplayApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;	

	gLanguage = GetRegLanguage();

	CCameraDisplayDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

