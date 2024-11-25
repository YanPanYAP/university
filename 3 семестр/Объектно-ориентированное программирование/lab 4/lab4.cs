using System;

// Абстрактный класс Product, представляющий общий тип товара
abstract class Product
{
    // Свойства имени и цены продукта
    public string Name { get; set; }
    public double Price { get; set; }

    // Конструктор для инициализации имени и цены продукта
    public Product(string name, double price)
    {
        Name = name;
        Price = price;
    }

    // Переопределение метода ToString для отображения информации о продукте
    public override string ToString()
    {
        return $"Продукт: {Name}, Цена: {Price}";
    }

    // Переопределение метода Equals для сравнения объектов по имени и цене
    public override bool Equals(object obj)
    {
        if (obj is Product product)
        {
            return Name == product.Name && Price == product.Price;
        }
        return false;
    }

    // Переопределение метода GetHashCode для создания уникального хэш-кода на основе имени и цены
    public override int GetHashCode()
    {
        return Name.GetHashCode() ^ Price.GetHashCode();
    }

    // Абстрактный метод для вывода информации о продукте, реализуется в подклассах
    public abstract void ShowInfo();
}

// Интерфейс, который описывает операции над продуктами
interface IProductOperations
{
    void DisplayProductType(); // Метод для отображения типа продукта
    bool DoClone(); // Метод для клонирования продукта
}

// Абстрактный класс Goods, наследуется от Product, представляет группу товаров
abstract class Goods : Product
{
    // Конструктор для инициализации имени и цены товара
    public Goods(string name, double price) : base(name, price) { }

    // Переопределение абстрактного метода ShowInfo для товаров
    public override void ShowInfo()
    {
        Console.WriteLine($"Товар: {Name}, Цена: {Price}");
    }
}

// Класс Flowers, представляющий цветы, наследуется от Goods
class Flowers : Goods
{
    // Конструктор для инициализации имени и цены цветов
    public Flowers(string name, double price) : base(name, price) { }

    // Переопределение метода ToString для отображения информации о цветах
    public override string ToString()
    {
        return $"\nЦветы: {Name}, Цена: {Price}";
    }
}

// Класс Watch, представляющий часы, наследуется от Goods и реализует интерфейс IProductOperations
class Watch : Goods, IProductOperations
{
    // Конструктор для инициализации имени и цены часов
    public Watch(string name, double price) : base(name, price) { }

    // Переопределение метода ToString для отображения информации о часах
    public override string ToString()
    {
        return $"\nЧасы: {Name}, Цена: {Price}";
    }

    // Реализация метода интерфейса для отображения типа продукта (часы)
    public void DisplayProductType()
    {
        Console.WriteLine("\nТип товара: Часы");
    }

    // Реализация метода клонирования (возвращает false как условие)
    public bool DoClone()
    {
        return false; // Простая реализация
    }
}

// Абстрактный класс Confectionery, наследуется от Product, представляет группу кондитерских изделий
abstract class Confectionery : Product
{
    // Конструктор для инициализации имени и цены кондитерского изделия
    public Confectionery(string name, double price) : base(name, price) { }

    // Переопределение метода ShowInfo для кондитерских изделий
    public override void ShowInfo()
    {
        Console.WriteLine($"\nКондитерское изделие: {Name}, Цена: {Price}");
    }
}

// Запечатанный класс Cake, представляющий торт, наследуется от Confectionery
sealed class Cake : Confectionery
{
    // Конструктор для инициализации имени и цены торта
    public Cake(string name, double price) : base(name, price) { }

    // Переопределение метода ToString для отображения информации о торте
    public override string ToString()
    {
        return $"\nТорт: {Name}, Цена: {Price}";
    }
}

// Класс Candy, представляющий конфеты, наследуется от Confectionery и реализует интерфейс IProductOperations
class Candy : Confectionery, IProductOperations
{
    // Конструктор для инициализации имени и цены конфет
    public Candy(string name, double price) : base(name, price) { }

    // Переопределение метода ToString для отображения информации о конфетах
    public override string ToString()
    {
        return $"\nКонфеты: {Name}, Цена: {Price}";
    }

    // Реализация метода интерфейса для отображения типа продукта (конфеты)
    public void DisplayProductType()
    {
        Console.WriteLine("\nТип товара: Конфеты");
    }

    // Реализация метода клонирования (возвращает true как условие)
    public bool DoClone()
    {
        return true; // Простая реализация
    }
}

// Класс Printer для вывода информации о продукте через метод IAmPrinting
class Printer
{
    // Метод для вывода информации о продукте с использованием метода ToString
    public void IAmPrinting(Product product)
    {
        Console.WriteLine(product.ToString());
    }
}

// Основная программа
class Program
{
    static void Main()
    {
        // Создаем объекты различных классов
        Product flower = new Flowers("Розы", 150);
        Product watch = new Watch("Rolex", 5000);
        Product cake = new Cake("Шоколадный торт", 1000);
        Product candy = new Candy("Карамель", 200);

        // Массив ссылок на продукты
        Product[] products = { flower, watch, cake, candy };

        // Создаем объект класса Printer
        Printer printer = new Printer();

        // Последовательный вызов метода IAmPrinting для каждого продукта в массиве
        foreach (Product product in products)
        {
            printer.IAmPrinting(product);
        }

        // Работа с объектами через ссылки на интерфейсы
        IProductOperations watchOperations = watch as IProductOperations; // Приведение типа
        if (watchOperations != null)
        {
            // Вызов методов интерфейса
            watchOperations.DisplayProductType();
            Console.WriteLine("\nКлонирование возможно: " + watchOperations.DoClone());
        }

        IProductOperations candyOperations = candy as IProductOperations; // Приведение типа
        if (candyOperations != null)
        {
            // Вызов методов интерфейса
            candyOperations.DisplayProductType();
            Console.WriteLine("\nКлонирование возможно: " + candyOperations.DoClone());
        }
    }
}