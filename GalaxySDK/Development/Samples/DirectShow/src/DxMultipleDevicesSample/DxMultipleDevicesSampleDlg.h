
// DxMultipleDevicesSampleDlg.h : header file
//

#pragma once

#include "Def.h"

#include <iostream>  
#include <stdlib.h>
#include <sstream>
#include <string>
#include <map>

#include "Device.h"
#include "DeviceManager.h"
#include "CvvImage.h"

using namespace std;

// CDxMultipleDevicesSampleDlg dialog
class CDxMultipleDevicesSampleDlg : public CDialog
{
// Construction
public:
	CDxMultipleDevicesSampleDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CDxMultipleDevicesSampleDlg();

// Dialog Data
	enum { IDD = IDD_DXMULTIPLEDEVICESSAMPLE_DIALOG };

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
    ///Switch the selected drop-down box
    afx_msg void OnCbnSelchangeComboEnumDevice();
    ///Click to open the device button
    afx_msg void OnBnClickedButtonOpenDevice();
    ///Click the close device button
    afx_msg void OnBnClickedButtonCloseDevice();
    ///Click the Start Acquisition button
    afx_msg void OnBnClickedButtonStartAcquisition();
    ///Click to stop collecting button
    afx_msg void OnBnClickedButtonStopAcquisition();
    ///Exposure time edit box loses focus
    afx_msg void OnEnKillfocusEditExposureTime();
    ///Gain edit box loses focus
    afx_msg void OnEnKillfocusEditGain();
    ///Select the value in the white balance drop-down box
    afx_msg void OnCbnSelchangeComboWhiteBalance();
    ///Click the Filter property page button
    afx_msg void OnBnClickedButtonFilterProperty();
    ///Click the Pin property page button
    afx_msg void OnBnClickedButtonPinProperty();
    ///Close the Program
	afx_msg void OnClose();
	///Timer refreshes the White Balance
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    ///Preprocess message
    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    ///Update the contents of the enumeration device drop-down box
    void __UpdateEnumDeviceComboBox();
    ///Bullet box error message
    void __UpdateMessageBox(std::string strError);
    ///Update control enable status
    void __UpdateUI();
    ///Update the range of parameters displayed on the control
    void __UpdateParamRange(CStatic *pDlgItem, std::string strDlgName);
    ///Update current parameters in the exposure time control
    void __UpdateExposureTimeParam(long lExposureTime);
    ///Update the current parameters in the gain control
    void __UpdateGainParam(long lGain);
    ///Update the current parameters in the white balance control
    void __UpdateWhiteBalanceParam(long lWhiteBalance);
    ///Update the white balance control enable
    void __UpdateWhiteBalanceEnable(bool bEnable);
    ///Update the value displayed in the white balance drop-down box
    void __UpdateWhiteBalanceRange(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange);

    ///Display exposure time parameter
    DEVICE_STATUS __DisplayExposureTime();
    ///Display gain parameter
    DEVICE_STATUS __DisplayGain();
    ///Display white balance parameters
    DEVICE_STATUS __DisplayWhiteBalance();

    ///Long type to string type
    std::string __LongToString(long lTemp);
    ///Determine whether the current device ID is legal
    bool __CheckDeviceID();
    ///Determine if the current input is a number
    BOOL __IsNumber(MSG* pMsg, const CString strText, bool bIsSelectAll);

    virtual void OnOK();

private:
	int                       m_nSelectID;        ///<Currently selected ID
    int                       m_nSelectDeviceID;  ///<Currently selected device ID
    std::map<int, CDevice *>  m_mapDevice;        ///<Device map
};
