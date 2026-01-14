#include "pch.h"

//------------------------------------------------------------------------------

CBlade::CBlade()
// Конструктор по умолчанию - задает параметры лопасти
{
    angle = 0;
    delta_angle = 5.0;  // Угол поворота за один шаг (градусы)

    // Задаем параметры лопасти по умолчанию
    h = 5.0;          // Высота треугольника
    phi = 30.0;       // Угол при вершине (градусы)
    R = 1.0;          // Радиус описанной окружности
    clockwise = true; // по умолчанию вращается по часовой стрелке

    // Определяем область в мировых координатах
    SpaceRect.SetRectD(-8, 8, 8, -8);

    // Вычисляем координаты точек
    CalculatePoints();
}

//------------------------------------------------------------------------------

CBlade::CBlade(double height, double angle_phi, double radius)
// Конструктор с параметрами
// height - высота треугольника
// angle_phi - угол при вершине (градусы)
// radius - радиус описанной окружности
{
    angle = 0;
    delta_angle = 5.0;

    h = height;
    phi = angle_phi;
    R = radius;

    SpaceRect.SetRectD(-8, 8, 8, -8);

    CalculatePoints();
}

//------------------------------------------------------------------------------

void CBlade::CalculatePoints()
// Вычисляет координаты точек лопасти на основе параметров h, phi, R
{
    Points.RedimMatrix(3, 6);  // 3 координаты (x,y,z=1) для 6 точек

    // Вычисляем половину угла при вершине в радианах
    double half_phi = (phi / 2.0) * M_PI / 180.0;

    // Вычисляем ширину основания треугольника
    double base_width = 2.0 * h * tan(half_phi);

    // Левый треугольник (вершина направлена влево)
    // Точка 0 - центральная вершина
    Points(0, 0) = -R;
    Points(1, 0) = 0;
    Points(2, 0) = 1;

    // Точка 1 - верхняя точка основания
    Points(0, 1) = -R - h;
    Points(1, 1) = base_width / 2.0;
    Points(2, 1) = 1;

    // Точка 2 - нижняя точка основания
    Points(0, 2) = -R - h;
    Points(1, 2) = -base_width / 2.0;
    Points(2, 2) = 1;

    // Правый треугольник (вершина направлена вправо)
    // Точка 3 - центральная вершина
    Points(0, 3) = R;
    Points(1, 3) = 0;
    Points(2, 3) = 1;

    // Точка 4 - верхняя точка основания
    Points(0, 4) = R + h;
    Points(1, 4) = base_width / 2.0;
    Points(2, 4) = 1;

    // Точка 5 - нижняя точка основания
    Points(0, 5) = R + h;
    Points(1, 5) = -base_width / 2.0;
    Points(2, 5) = 1;
}

//------------------------------------------------------------------------------

void CBlade::SetWinRect(CRect& rw)
// Устанавливает область окна для рисования
// rw - прямоугольник в окне
{
    WinRect = rw;
}

//------------------------------------------------------------------------------

void CBlade::Draw(CDC& dc)
// Рисует лопасть в окне (два треугольника)
// dc - контекст устройства
{


    // Матрица поворота вокруг начала координат
    //CMatrix MR = CreateRotate2D(angle);

    // Поворачиваем точки
    //CMatrix RotatedPoints = MR * Points;

    // Матрица преобразования из мировых координат в оконные
    CMatrix MSW = SpaceToWindow(SpaceRect, WinRect);

    // Преобразуем в оконные координаты
    //CMatrix WinPoints = MSW * RotatedPoints;

    // Создаем перо и кисть для треугольников (КРАСНЫЙ)
    //CPen penRed(PS_SOLID, 2, RGB(255, 0, 0));
    //CBrush brushRed(RGB(255, 100, 100));
    //CPen* oldPen = dc.SelectObject(&penRed);
    //CBrush* oldBrush = dc.SelectObject(&brushRed);

CPen penRed(PS_SOLID, 2, RGB(0, 0, 255));
CBrush brushRed(RGB(100, 100, 255));
CPen* oldPen = dc.SelectObject(&penRed);
CBrush* oldBrush = dc.SelectObject(&brushRed);


    // Рисуем 3 лопасти с шагом 120°
    for (int i = 0; i < 3; i++)
    {
        double blade_angle = angle + i * 120.0;

        // Матрица поворота для текущей лопасти
        CMatrix MR = CreateRotate2D(blade_angle);

        // Поворачиваем точки
        CMatrix RotatedPoints = MR * Points;

        // Преобразуем в оконные координаты
        CMatrix WinPoints = MSW * RotatedPoints;

        // Рисуем левый треугольник
        POINT pts1[3];
        for (int j = 0; j < 3; j++)
        {
            pts1[j].x = (int)WinPoints(0, j);
            pts1[j].y = (int)WinPoints(1, j);
        }
        dc.Polygon(pts1, 3);

    }


    //// Рисуем левый треугольник
    //POINT pts1[3];
    //for (int i = 0; i < 3; i++)
    //{
    //    pts1[i].x = (int)WinPoints(0, i);
    //    pts1[i].y = (int)WinPoints(1, i);
    //}
    //dc.Polygon(pts1, 3);

    //// Рисуем правый треугольник
    //POINT pts2[3];
    //for (int i = 0; i < 3; i++)
    //{
    //    pts2[i].x = (int)WinPoints(0, i + 3);
    //    pts2[i].y = (int)WinPoints(1, i + 3);
    //}
    //dc.Polygon(pts2, 3);

    // Рисуем центральную окружность (ЗЕЛЕНЫЙ)
    CPen penGreen(PS_SOLID, 2, RGB(0, 200, 0));
    CBrush brushGreen(RGB(100, 255, 100));
    dc.SelectObject(&penGreen);
    dc.SelectObject(&brushGreen);

    // Преобразуем центр окружности в оконные координаты
    CMatrix center(3, 1);
    center(0, 0) = 0;
    center(1, 0) = 0;
    center(2, 0) = 1;
    CMatrix centerWin = MSW * center;

    int cx = (int)centerWin(0, 0);
    int cy = (int)centerWin(1, 0);

    // Вычисляем радиус в пикселях
    double scale = (WinRect.Width()) / (SpaceRect.right - SpaceRect.left);
    int radius_pixels = (int)(R * scale);

    dc.Ellipse(cx - radius_pixels, cy - radius_pixels,
        cx + radius_pixels, cy + radius_pixels);

    // Восстанавливаем старые объекты GDI
    dc.SelectObject(oldPen);
    dc.SelectObject(oldBrush);
}

//------------------------------------------------------------------------------

void CBlade::Rotate()
// Поворачивает лопасть на delta_angle градусов
{
    if (clockwise)
        angle -= delta_angle;   // по часовой
    else
        angle += delta_angle;   // против часовой

    if (angle >= 360)
        angle -= 360;
    else if (angle < 0)
        angle += 360;
}

//------------------------------------------------------------------------------

void CBlade::SetDirection(bool cw)
// Устанавливает направление вращения пропеллера
{
    clockwise = cw;
}

//------------------------------------------------------------------------------

void CBlade::ToggleDirection()
// Изменяет направление вращения пропеллера
{
    clockwise = !clockwise;
}

//------------------------------------------------------------------------------