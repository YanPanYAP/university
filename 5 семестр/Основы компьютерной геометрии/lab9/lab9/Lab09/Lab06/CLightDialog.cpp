#include "CLightDialog.h"
#include "resource.h"

CLightDialog::CLightDialog(CMatrix& light, COLORREF& color, CWnd* pParent)
	: CDialog(IDD_LIGHT_DIALOG, pParent)
	, m_Light(light)
	, m_Color(color)
{
}

void CLightDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLightDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

COLORREF CLightDialog::HexToColor(const CString& hexStr)
{
	CString hex = hexStr;
	hex.Trim();
	
	// Убрать # если есть
	if (hex.GetLength() > 0 && hex.GetAt(0) == L'#')
		hex = hex.Mid(1);
	
	// Должно быть 6 символов
	if (hex.GetLength() != 6)
		return RGB(0, 0, 0);
	
	// Преобразовать hex в числа
	wchar_t* endPtr;
	unsigned long colorVal = wcstoul(hex, &endPtr, 16);
	
	if (*endPtr != 0)
		return RGB(0, 0, 0);
	
	// Извлечь R, G, B
	BYTE r = (BYTE)((colorVal >> 16) & 0xFF);
	BYTE g = (BYTE)((colorVal >> 8) & 0xFF);
	BYTE b = (BYTE)(colorVal & 0xFF);
	
	return RGB(r, g, b);
}

CString CLightDialog::ColorToHex(COLORREF color)
{
	BYTE r = GetRValue(color);
	BYTE g = GetGValue(color);
	BYTE b = GetBValue(color);
	
	CString hex;
	hex.Format(L"%02X%02X%02X", r, g, b);
	return hex;
}

BOOL CLightDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Установить текущие значения координат источника света
	CString str;
	
	str.Format(L"%.2f", m_Light(0));
	SetDlgItemText(IDC_EDIT_R, str);
	
	str.Format(L"%.2f", m_Light(1));
	SetDlgItemText(IDC_EDIT_FI, str);
	
	str.Format(L"%.2f", m_Light(2));
	SetDlgItemText(IDC_EDIT_Q, str);

	// Установить цвет в hex формате
	CString hexColor = ColorToHex(m_Color);
	SetDlgItemText(IDC_EDIT_COLOR, hexColor);

	return TRUE;
}

void CLightDialog::OnOK()
{
	// Получить значения из полей редактирования источника света
	CString strR, strFi, strQ;
	
	GetDlgItemText(IDC_EDIT_R, strR);
	GetDlgItemText(IDC_EDIT_FI, strFi);
	GetDlgItemText(IDC_EDIT_Q, strQ);

	// Преобразовать строки в числа
	double r = _wtof(strR);
	double fi = _wtof(strFi);
	double q = _wtof(strQ);

	// Проверка на валидность значений источника света
	if (r <= 0)
	{
		MessageBox(L"Радиус должен быть положительным числом!", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	if (q < 0 || q > 180)
	{
		MessageBox(L"Угол q должен быть в диапазоне от 0 до 180 градусов!", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	// Обновить координаты источника света
	m_Light(0) = r;
	m_Light(1) = fi;
	m_Light(2) = q;

	// Получить и обновить цвет
	CString strColor;
	GetDlgItemText(IDC_EDIT_COLOR, strColor);
	COLORREF newColor = HexToColor(strColor);
	
	// Проверка на валидность цвета (если преобразование не удалось и это не черный цвет)
	if (newColor == RGB(0, 0, 0))
	{
		CString hex = strColor;
		hex.Trim();
		if (hex.GetLength() > 0 && hex.GetAt(0) == L'#')
			hex = hex.Mid(1);
		if (hex.CompareNoCase(L"000000") != 0)
		{
			MessageBox(L"Неверный формат цвета! Используйте формат RRGGBB (например, EC6486)", L"Ошибка", MB_OK | MB_ICONERROR);
			return;
		}
	}
	m_Color = newColor;

	CDialog::OnOK();
}

