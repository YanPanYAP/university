using System;
using System.Collections.Generic;
using System.Diagnostics;

public class ProductException : Exception
{
    public ProductException(string message) : base(message) { }
}

public class InvalidDimensionException : ProductException
{
    public InvalidDimensionException(string message) : base(message) { }
}

public class NegativePriceException : ProductException
{
    public NegativePriceException(string message) : base(message) { }
}

public class ItemNotFoundException : ProductException
{
    public ItemNotFoundException(string message) : base(message) { }
}

public struct Dimension
{
    public double Height { get; set; }
    public double Width { get; set; }
    public double Length { get; set; }

    public Dimension(double height, double width, double length)
    {
        if (height <= 0 || width <= 0 || length <= 0)
        {
            throw new InvalidDimensionException("Значения размеров должны быть положительными.");
        }
        Height = height;
        Width = width;
        Length = length;
    }

    public double CalculateVolume() => Height * Width * Length;

    public override string ToString() => $"Габариты (ВxШxД): {Height}x{Width}x{Length}";
}

public abstract class Product
{
    public string Name { get; set; }
    public double Price { get; set; }
    public Dimension ProductDimension { get; set; }

    public Product(string name, double price, Dimension dimension)
    {
        if (price < 0)
        {
            throw new NegativePriceException("Цена не может быть отрицательной.");
        }
        Name = name;
        Price = price;
        ProductDimension = dimension;
    }

    public abstract void ShowInfo();
}

public class Flowers : Product
{
    public Flowers(string name, double price, Dimension dimension) : base(name, price, dimension) { }
    public override void ShowInfo() => Console.WriteLine($"Цветы: {Name}, Цена: {Price}, {ProductDimension}");
}

public class Program
{
    public static void Main()
    {
        try
        {
            //int[] array = null;
            //Debug.Assert(array != null, "Массив не должен быть null");

            // Создание объектов с обработкой исключений
            Product flower = null;

            try
            {
                flower = new Flowers("Розы", 150, new Dimension(-10, 5, 3));
            }
            catch (InvalidDimensionException ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }

            try
            {
                Product watch = new Flowers("Rolex", -5000, new Dimension(4, 4, 1));
            }
            catch (NegativePriceException ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }

            List<Product> products = new List<Product>();

            try
            {
                if (flower == null)
                    throw new ItemNotFoundException("Цветы не добавлены в список продуктов.");
                products.Add(flower);
            }
            catch (ItemNotFoundException ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }

            // Универсальный обработчик исключений
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Общее исключение: {ex.Message}");
        }
        finally
        {
            Console.WriteLine("Завершение выполнения программы.");
        }
        
    }

}

/*
int a = 10;
int b = 0;
try
{
    int result = a / b;
    Console.WriteLine($"Результат: {result}");
}
catch
{
    Console.WriteLine("Ошибка: деление на ноль!");
}
finally
{
    Console.WriteLine("Завершение выполнения программы.");
}
}
*/


