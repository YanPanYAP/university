#pragma once
#include "resource.h"
#include "CMatrix.h"
#include <afxwin.h>

class CLightDialog : public CDialog
{
public:
	CLightDialog(CMatrix& light, COLORREF& color, CWnd* pParent = nullptr);

	// Данные диалога
	CMatrix& m_Light; // Ссылка на координаты источника света
	COLORREF& m_Color; // Ссылка на цвет пирамиды

	enum { IDD = IDD_LIGHT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV поддержка

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	
private:
	// Функция для преобразования hex строки в COLORREF
	COLORREF HexToColor(const CString& hexStr);
	// Функция для преобразования COLORREF в hex строку
	CString ColorToHex(COLORREF color);
};

