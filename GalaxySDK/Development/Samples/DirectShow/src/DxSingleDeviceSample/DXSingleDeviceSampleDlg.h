
// DXSingleDeviceSampleDlg.h : header file
//

#pragma once

#include "Def.h"

#include <iostream>  
#include <stdlib.h>
#include <sstream>
#include <string>

#include "Device.h"
#include "DeviceManager.h"
#include "CvvImage.h"

using namespace std;

// CDXSingleDeviceSampleDlg dialog
class CDXSingleDeviceSampleDlg : public CDialog
{
// Construction
public:
	///Constructor
	CDXSingleDeviceSampleDlg(CWnd* pParent = NULL);	// standard constructor

	///Destructor
    ~CDXSingleDeviceSampleDlg();

// Dialog Data
	enum { IDD = IDD_DXSINGLEDEVICE2010_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	///Click to open the device button
    afx_msg void OnBnClickedButtonOpenDevice();

	///Click the close device button
	afx_msg void OnBnClickedButtonCloseDevice();

	///Click the Start Acquisition button
    afx_msg void OnBnClickedButtonStartAcquisition();

	///Click to stop collecting button
    afx_msg void OnBnClickedButtonStopAcquisition();

	///Exposure time edit box loses focus
	afx_msg void OnEnKillfocusEditExposure();

	///Gain edit box loses focus
	afx_msg void OnEnKillfocusEditGain();

	///Select the value in the white balance drop-down box
	afx_msg void OnCbnSelchangeComboWhiteBalance();

	///Click the Filter property page button
	afx_msg void OnBnClickedButtonOpenFilterPage();

	///Click the Pin property page button
	afx_msg void OnBnClickedButtonOpenPinPage();

	///Click the save image control
	afx_msg void OnBnClickedCheckSavePicture();

	///Close the Program
	afx_msg void OnClose();

	///Timer refreshes the White Balance
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	///Preprocess message
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void OnOK();

private:
	///Bullet box error message
	void __UpdateMessageBox(std::string strError);

	///Update control enable status
	void __UpdateUI(void);

	///Update current parameters in the exposure time control
	void __UpdateExposureTimeParam(long lExposureTime);

	///Update the current parameters in the gain control
	void __UpdateGainParam(long lGain);

	///Update the current parameters in the white balance control
	void __UpdateWhiteBalanceParam(long lWhiteBalance);

	///Update the range of parameters displayed on the control
	void __UpdateParamRange(CStatic *pDlgItem, std::string strDlgName);

	///Update the current parameters in the white balance control
	void __UpdateWhiteBalanceRange(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange);

	///Update the white balance control enable
	void __UpdateWhiteBalanceEnable(bool bEnable);

	///Update the controlled Device
	void __UpdateDevice(void);

	///Display exposure time parameter
	DEVICE_STATUS __DisplayExposureTime(void);

	///Display gain parameter
	DEVICE_STATUS __DisplayGain(void);

	///Display white balance parameters
	DEVICE_STATUS __DisplayWhiteBalance(void);

	///Determine if the current input is a number
	BOOL __IsNumber(MSG* pMsg, const CString strText, bool bIsSelectAll);

	///Long type to string type
	std::string __LongToString(long lTemp);

private:
	int        m_nDeviceID;        ///<Currently device ID
	CDevice    *m_pCamDevice;      ///<CDevice pointer object
};
