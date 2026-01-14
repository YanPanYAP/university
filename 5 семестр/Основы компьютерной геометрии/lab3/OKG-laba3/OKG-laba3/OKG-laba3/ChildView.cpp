
// ChildView.cpp: реализация класса CChildView
//

#include "stdafx.h"
#include "framework.h"
#include "OKG-laba3.h"
#include "ChildView.h"
#include <math.h>
#define MARGIN_CYCLE 10

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	Index = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_TESTS_F1, &CChildView::OnTestsF1)
	ON_COMMAND(ID_TESTS_F2, &CChildView::OnTestsF2)
	ON_COMMAND(ID_TESTS_F3, &CChildView::OnTestsF3)
    //ON_COMMAND(ID_TESTS_F4, &CChildView::OnTestsF4)
END_MESSAGE_MAP()



// Обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // контекст устройства для рисования 
	dc.SetMapMode(MM_ANISOTROPIC);

	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(255, 255, 255)); // Белый фон
	dc.FillRect(&rect, &brush);

    if (Index == 1 || Index == 2 || Index == 4)
	{
		Graph.Draw(dc, 1, 1);
	}
	else if (Index == 3)
	{
		// Перерисовываем восьмиугольник
		OnTestsF3();
	}
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

    // Обновляем область рисования при изменении размеров окна
    if (Index == 1 || Index == 2 || Index == 4)
	{
		// Автоматически изменяем размер области рисования
		int margin = 50;
		RW.SetRect(margin, margin, cx - margin, cy - margin);
		Graph.SetWindowRect(RW);
	}

	Invalidate(); // Перерисовываем
}

double CChildView::MyF1(double x)
{
	double y = sin(x) / x;
	return y;
}



double CChildView::MyF2(double x)
{
	double y = sqrt(abs(x)) * sin(x);
	return y;
}

double CChildView::MyF3(double x, double y)
{
    const double eps = 1e-9;
    return (fabs(x - y) < eps) ? 1.0 : 2.0;
}

/*double CChildView::MyF4(double xDeg)
{
    // f(0°)   = 5*x
    // f(90°)  = sqrt(x)
    // 0°<x<90°: e^(sin(x))
    const double eps = 1e-9;
    if (fabs(xDeg - 0.0) < eps) {
        return 5.0 * xDeg;
    }
    if (fabs(xDeg - 90.0) < eps) {
        return sqrt(xDeg);
    }
    if (xDeg > 0.0 && xDeg < 90.0) {
        double xRad = xDeg * 3.14159265358979323846 / 180.0;
        return exp(sin(xRad));
    }
    return std::numeric_limits<double>::quiet_NaN();
}*/


void CChildView::OnTestsF1() {
	double xL = -3 * pi;
	double xH = -xL;
	double deltaX = pi / 36;
	int n = (xH - xL) / deltaX;
	X.RedimMatrix(n + 1);
	Y.RedimMatrix(n + 1);
	for (int i = 0; i <= n; i++) {
		X(i) = xL + i * deltaX;
		Y(i) = MyF1(X(i));
	}
    PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
    PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	GetClientRect(&RW);
	int margin = 50;
	RW.DeflateRect(margin, margin);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	Invalidate();
}

void CChildView::OnTestsF2() {
	double xL = -4 * pi;
	double xH = -xL;
	double deltaX = pi / 36;
	int n = (xH - xL) / deltaX;
	X.RedimMatrix(n + 1);
	Y.RedimMatrix(n + 1);
	for (int i = 0; i <= n; i++) {
		X(i) = xL + i * deltaX;
		Y(i) = MyF2(X(i));
	}
    PenLine.Set(PS_DASH, 3, RGB(255, 0, 0));
    PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	GetClientRect(&RW);
	int margin = 50;
	RW.DeflateRect(margin, margin);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 2;
	Invalidate();
}

void CChildView::OnTestsF3() {
    Invalidate();
    Index = 3;
    CPaintDC dc(this);

    // Пера для восьмиугольника и окружности
    CPen penFigure(PS_SOLID, 3, RGB(255, 0, 0));
    CPen penCircle(PS_SOLID, 2, RGB(0, 0, 255));
    CPen penAxis(PS_SOLID, 1, RGB(0, 0, 0)); // оси — чёрные
    CFont font;
    font.CreatePointFont(90, _T("Arial")); // шрифт для меток
    CFont* pOldFont = dc.SelectObject(&font);

    CRect rect;
    GetClientRect(&rect);
    const int squareSize = min(rect.Width() - MARGIN_CYCLE * 2, rect.Height() - MARGIN_CYCLE * 2);

    // Центр и радиус
    const double radius = squareSize / 2.0;
    const double angle = 45.0;
    const double centerX = rect.left + radius + MARGIN_CYCLE;
    const double centerY = rect.top + radius + MARGIN_CYCLE;

    // Окружность
    const double circleLeft = centerX - radius;
    const double circleTop = centerY - radius;
    const double circleRight = centerX + radius;
    const double circleBottom = centerY + radius;

    dc.SelectObject(&penCircle);
    dc.Ellipse(circleLeft, circleTop, circleRight, circleBottom);

    // Восьмиугольник
    const int sides = 8;
    const double angleRadians = angle * 3.14159 / 180.0;
    const double startX = centerX + radius;
    const double startY = centerY;
    double currentX = startX;
    double currentY = startY;

    dc.SelectObject(&penFigure);
    for (int i = 0; i < sides; i++) {
        double nextX = centerX + radius * cos(angleRadians * i);
        double nextY = centerY - radius * sin(angleRadians * i);

        dc.MoveTo((int)currentX, (int)currentY);
        dc.LineTo((int)nextX, (int)nextY);

        currentX = nextX;
        currentY = nextY;
    }
    dc.MoveTo((int)currentX, (int)currentY);
    dc.LineTo((int)startX, (int)startY);

    // Оси координат
    dc.SelectObject(&penAxis);
    dc.MoveTo((int)circleLeft - 20, (int)centerY); // ось X
    dc.LineTo((int)circleRight + 20, (int)centerY);
    dc.MoveTo((int)centerX, (int)circleTop - 20); // ось Y
    dc.LineTo((int)centerX, (int)circleBottom + 20);

    // Метки на осях
    const int tickLength = 5;
    const int labelOffset = 15;
    const int step = squareSize / 4; // шаг между метками

    int maxTicks = (int)(radius / (step / 2)); // сколько меток помещается от центра до края

    for (int i = -maxTicks; i <= maxTicks; i++) {
        int x = (int)(centerX + i * step / 2);
        if (x < rect.left || x > rect.right) continue;

        dc.MoveTo(x, (int)centerY - tickLength);
        dc.LineTo(x, (int)centerY + tickLength);

        CString label;
        label.Format(_T("%d"), i);
        dc.TextOut(x - 5, (int)centerY + labelOffset, label);
    }

    for (int i = -maxTicks; i <= maxTicks; i++) {
        int y = (int)(centerY - i * step / 2);
        if (y < rect.top || y > rect.bottom) continue;

        dc.MoveTo((int)centerX - tickLength, y);
        dc.LineTo((int)centerX + tickLength, y);

        CString label;
        label.Format(_T("%d"), i);
        dc.TextOut((int)centerX + labelOffset, y - 5, label);
    }

    dc.SelectObject(pOldFont); // восстановить шрифт
}


/*void CChildView::OnTestsF4() {
    double xL = 0.0;      // градусы
    double xH = 90.0;     // градусы
    double deltaX = 1.0; 
    int n = (int)((xH - xL) / deltaX);
    X.RedimMatrix(n + 1);
    Y.RedimMatrix(n + 1);
    for (int i = 0; i <= n; i++) {
        X(i) = xL + i * deltaX;
        Y(i) = MyF4(X(i));
    }
    PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
    PenAxis.Set(PS_SOLID, 4, RGB(0, 0, 0));
    GetClientRect(&RW);
    int margin = 50;
    RW.DeflateRect(margin, margin);
    Graph.SetParams(X, Y, RW);
    Graph.SetPenLine(PenLine);
    Graph.SetPenAxis(PenAxis);
    Index = 4;
    Invalidate();
}*/
