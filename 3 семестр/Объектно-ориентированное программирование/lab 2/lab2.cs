using System;

public partial class Car
{
    // Поля класса (закрытые)
    private readonly int id;
    private string make;
    private string model;
    private int year;
    private string color;
    private decimal price;
    private string registrationNumber;

    // Константа
    private const int maxAge = 30;

    // Статическое поле для подсчета количества объектов
    private static int carCount;

    // Свойства (публичные)
    public string Make
    {
        get { return make; }
        set { if (!string.IsNullOrEmpty(value)) make = value; }
    }

    public string Model
    {
        get { return model; }
        set { if (!string.IsNullOrEmpty(value)) model = value; }
    }

    public int Year
    {
        get { return year; }
        set { if (value > 1885 && value <= DateTime.Now.Year) year = value; }
    }

    public string Color
    {
        get { return color; }
        set { if (!string.IsNullOrEmpty(value)) color = value; }
    }

    public decimal Price
    {
        get { return price; }
        set { if (value > 0) price = value; }
    }

    public string RegistrationNumber
    {
        get { return registrationNumber; }
        set { if (!string.IsNullOrEmpty(value)) registrationNumber = value; }
    }

    // Конструкторы
    public Car()
    {
        id = GenerateUniqueID();
        carCount++;
    }

    public Car(string make, string model, int year, string color, decimal price, string registrationNumber)
        : this()
    {
        Make = make;
        Model = model;
        Year = year;
        Color = color;
        Price = price;
        RegistrationNumber = registrationNumber;
    }

    public Car(string make, string model, decimal price)
        : this(make, model, DateTime.Now.Year, "Неизвестен", price, "Неизвестен")
    { }

    // Статический конструктор
    static Car()
    {
        carCount = 0;
    }

    // Закрытый конструктор
    private Car(string specialCar)
    {
        id = GenerateUniqueID();
        Console.WriteLine("\nСоздан специальный автомобиль: " + specialCar);
        carCount++;
    }

    // Метод создания объекта через закрытый конструктор
    public static Car CreateSpecialCar()
    {
        return new Car("Суперкар");
    }

    // Поле только для чтения
    public int ID
    {
        get { return id; }
    }


    // Метод для вывода возраста машины
    public int GetCarAge()
    {
        return DateTime.Now.Year - year;
    }

    // Метод для работы с ref и out параметрами
    public void GetCarInfo(out string info, ref string prefix)
    {
        info = $"{prefix} Марка: {Make}, Модель: {Model}, Год выпуска: {Year}, Цвет: {Color}, Цена: {Price}";
    }

    // Статический метод для вывода информации о классе
    public static void ShowCarCount()
    {
        Console.WriteLine($"\nСоздано {carCount} автомобилей\n");
    }

    // Переопределение методов Object
    public override bool Equals(object obj)
    {
        if (obj is Car otherCar)
        {
            return id == otherCar.id;
        }
        return false;
    }

    public override int GetHashCode()
    {
        return id.GetHashCode();
    }

    public override string ToString()
    {
        return $"ID: {id}, Марка: {Make}, Модель: {Model}, Год выпуска: {Year}, Цвет: {Color}, Цена: {Price}";
    }

    // Метод для генерации уникального ID
    private int GenerateUniqueID()
    {
        return carCount + 1; // Используем количество машин для генерации уникального ID
    }
}

// Пример работы с классом
public class Program
{
    public static void Main()
    {
        // Создание объектов
        Car car1 = new Car("Toyota", "Corolla", 2015, "Белый", 850000, "123ABC");
        Car car2 = new Car("Ford", "Focus", 2020, "Синий", 1200000, "456DEF");
        Car specialCar = Car.CreateSpecialCar();

        // Вызов методов
        string info;
        string prefix = "Информация: ";
        car1.GetCarInfo(out info, ref prefix);
        Console.WriteLine(info);

        // Вывод количества объектов
        Car.ShowCarCount();

        // Сравнение объектов
        Console.WriteLine(car1.Equals(car2));

        // Массив объектов
        Car[] cars = { car1, car2 };

        // Список автомобилей заданной марки
        string targetMake = "Toyota";
        Console.WriteLine($"\nАвтомобили марки {targetMake}:");
        foreach (var car in cars)
        {
            if (car.Make == targetMake)
            {
                Console.WriteLine(car);
            }
        }

        // Список автомобилей заданной модели, которые эксплуатируются больше n лет
        string targetModel = "Corolla";
        int n = 5;
        Console.WriteLine($"\nАвтомобили модели {targetModel}, эксплуатируемые более {n} лет:");
        foreach (var car in cars)
        {
            if (car.Model == targetModel && car.GetCarAge() > n)
            {
                Console.WriteLine(car);
            }
        }

        // Анонимный тип
        var anonymousCar = new { car1.Make, car1.Model, car1.Year };
        Console.WriteLine($"\nАнонимный тип: {anonymousCar.Make}, {anonymousCar.Model}, {anonymousCar.Year}\n");
    }
}
