using System;
using System.Collections;
using System.Collections.Generic;

public enum Category
{
    Flowers,
    Watches,
    Confectionery
}

public struct Dimension
{
    public double Height { get; set; }
    public double Width { get; set; }
    public double Length { get; set; }

    public Dimension(double height, double width, double length)
    {
        Height = height;
        Width = width;
        Length = length;
    }

    public double CalculateVolume() => Height * Width * Length;

    public override string ToString()
    {
        return $"Габариты (ВxШxД): {Height}x{Width}x{Length}";
    }
}

public abstract partial class Product
{
    public string Name { get; set; }
    public double Price { get; set; }
    public Dimension ProductDimension { get; set; }

    public Product(string name, double price, Dimension dimension)
    {
        Name = name;
        Price = price;
        ProductDimension = dimension;
    }

    public override string ToString()
    {
        return $"Продукт: {Name}, Цена: {Price}, {ProductDimension}";
    }

    public abstract void ShowInfo();
}

public abstract partial class Product
{
    public int CompareTo(Product other)
    {
        return Price.CompareTo(other.Price);
    }
}

public class Flowers : Goods
{
    public Flowers(string name, double price, Dimension dimension) : base(name, price, dimension) { }
    public override string ToString() => $"\nЦветы: {Name}, Цена: {Price}, {ProductDimension}";
}

public class Watch : Goods
{
    public Watch(string name, double price, Dimension dimension) : base(name, price, dimension) { }
    public override string ToString() => $"\nЧасы: {Name}, Цена: {Price}, {ProductDimension}";
}

public abstract class Goods : Product
{
    public Goods(string name, double price, Dimension dimension) : base(name, price, dimension) { }
    public override void ShowInfo() => Console.WriteLine($"Товар: {Name}, Цена: {Price}");
}

public class Container<T> : IEnumerable<T> where T : Product
{
    private List<T> items = new List<T>();

    public void AddItem(T item) => items.Add(item);
    public void RemoveItem(T item) => items.Remove(item);

    public void DisplayItems() {
        foreach (var item in items)
        {
            Console.WriteLine(item);
        }
    }

    public T FindItemWithMinMass()
    {
        if (items.Count == 0) return null;
        T minItem = items[0];
        foreach (var item in items)
        {
            if (item.ProductDimension.CalculateVolume() < minItem.ProductDimension.CalculateVolume())
            {
                minItem = item;
            }
        }
        return minItem;
    }


    public void SortByDimensions() 
    {
        items.Sort((a, b) => a.ProductDimension.CalculateVolume().CompareTo(b.ProductDimension.CalculateVolume()));
    }

    public IEnumerator<T> GetEnumerator() 
    {
        return items.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() 
    {
        return GetEnumerator();
    }
}

public class Controller
{
    private Container<Product> container;

    public Controller(Container<Product> container)
    {
        this.container = container;
    }

    public double CalculateTotalPrice() 
    {
        double total = 0;
        foreach (var item in container)
        {
            total += item.Price;
        }
        return total;
    }

    public void FindAndDisplayMinMassItem() 
    {
        var minItem = container.FindItemWithMinMass();
        Console.WriteLine($"Компонент с наименьшей массой: {minItem}");
    }

    public void SortAndDisplay() 
    {
        container.SortByDimensions();
        Console.WriteLine("Список после сортировки по габаритам:");
        container.DisplayItems();
    }
}

// Основная программа
public class Program
{
    public static void Main()
    {
        Product flower = new Flowers("Розы", 150, new Dimension(10, 5, 3));
        Product watch = new Watch("Rolex", 5000, new Dimension(4, 4, 1));

        var container = new Container<Product>();
        container.AddItem(flower);
        container.AddItem(watch);

        var controller = new Controller(container);
        Console.WriteLine($"Общая цена подарка: {controller.CalculateTotalPrice()}");
        controller.FindAndDisplayMinMassItem();
        controller.SortAndDisplay();
    }
}

/*
public struct Point
{
    public int X;
    public int Y;

    public Point(int x, int y)
    {
        X = x;
        Y = y;
    }

    public void Display()
    {
        Console.WriteLine($"Point: X = {X}, Y = {Y}");
    }
}

class Program
{
    static void Main()
    {
        Point p1 = new Point(5, 10);
        p1.Display();
    }
}
*/
/*
 public enum DayOfWeek
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
}

class Program
{
    static void Main()
    {
        DayOfWeek today = DayOfWeek.Tuesday;
        Console.WriteLine($"Today is {today}");  // Вывод: Today is Tuesday
    }
}
*/
/*
class Program
{
    static void Main()
    {
        List<int> numbers = new List<int> { 1, 2, 3, 4, 5 };
        numbers.Add(6);
        numbers.Remove(3);

        foreach (int number in numbers)
        {
            Console.WriteLine(number);
        }
    }
}*/





enum Poit
{

}