using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class Image
{
    public string Name { get; set; }
    public string Resolution { get; set; }

    public Image(string name, string resolution)
    {
        Name = name;
        Resolution = resolution;
    }

    public override string ToString()
    {
        return $"{Name} ({Resolution})";
    }
}

public class Car
{
    public string Make { get; set; }
    public string Model { get; set; }
    public int Year { get; set; }
    public string Color { get; set; }
    public decimal Price { get; set; }

    public Car(string make, string model, int year, string color, decimal price)
    {
        Make = make;
        Model = model;
        Year = year;
        Color = color;
        Price = price;
    }
}

public class Program
{
    public static void Main()
    {
        string[] months = new string[] { "June", "July", "May", "December", "January", "March", "February", "August", "October", "April", "November", "September" };

        // 1. Месяцы с длиной строки равной n
        int n = 5;
        var monthsWithLengthN = months.Where(month => month.Length == n);
        Console.WriteLine($"Месяцы с длиной строки равной {n}:");
        foreach (var month in monthsWithLengthN)
        {
            Console.WriteLine(month);
        }

        // 2. Летние и зимние месяцы
        var summerAndWinterMonths = months.Where(month => new string[] { "June", "July", "August", "December", "January", "February" }.Contains(month));
        Console.WriteLine("\nЛетние и зимние месяцы:");
        foreach (var month in summerAndWinterMonths)
        {
            Console.WriteLine(month);
        }

        // 3. Месяцы в алфавитном порядке
        var sortedMonths = months.OrderBy(month => month);
        Console.WriteLine("\nМесяцы в алфавитном порядке:");
        foreach (var month in sortedMonths)
        {
            Console.WriteLine(month);
        }

        // 4. Месяцы, содержащие букву 'u' и длиной не менее 4
        var monthsWithUAndMinLength = months.Where(month => month.Contains('u') && month.Length >= 4);
        Console.WriteLine("\nМесяцы, содержащие букву 'u' и длиной не менее 4:");
        foreach (var month in monthsWithUAndMinLength)
        {
            Console.WriteLine(month);
        }

        // Задание 2: Коллекция Image
        List<Image> images = new List<Image>
        {
            new Image("Nature", "1920x1080"),
            new Image("Cityscape", "2560x1440"),
            new Image("Ocean", "3840x2160"),
            new Image("Mountain", "1280x720"),
            new Image("Sunset", "1920x1080"),
            new Image("Forest", "1920x1080"),
            new Image("Desert", "1920x1080"),
            new Image("Sky", "1280x720"),
            new Image("River", "1920x1080"),
            new Image("Lake", "2560x1440")
        };

        // Запрос: Изображения с разрешением 1920x1080
        var filteredImages = images.Where(img => img.Resolution == "1920x1080");
        Console.WriteLine("\nИзображения с разрешением 1920x1080:");
        foreach (var img in filteredImages)
        {
            Console.WriteLine(img);
        }

        // Задание 3: Пример запросов LINQ
        int[] numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        // Фильтрация четных чисел
        var evenNumbers = numbers.Where(num => num % 2 == 0);
        Console.WriteLine("\nЧетные числа:");
        foreach (var num in evenNumbers)
        {
            Console.WriteLine(num);
        }

        // Сортировка по убыванию
        var sortedNumbers = numbers.OrderBy(num => num);
        Console.WriteLine("\nЧисла в порядке убывания:");
        foreach (var num in sortedNumbers)
        {
            Console.WriteLine(num);
        }

        // Подсчет суммы чисел
        var sum = numbers.Sum();
        Console.WriteLine($"\nСумма чисел: {sum}");

        // Запрос с 5 операторами
        var result = numbers.Where(num => num % 2 == 0) // Условие (четные числа)
                            .OrderBy(num => num) // Упорядочивание по возрастанию
                            .Select(num => num * 2) // Проекция (умножение на 2)
                            .Take(3) // Выборка первых 3 элементов
                            .Sum(); // Агрегирование (сумма)

        Console.WriteLine("\nРезультат запроса с 5 операторами: " + result);

        // Задание 5: Запрос с оператором Join
        List<Car> cars = new List<Car>
        {
            new Car("Toyota", "Corolla", 2010, "Red", 5000),
            new Car("Toyota", "Camry", 2015, "Blue", 15000),
            new Car("Honda", "Civic", 2018, "Red", 12000),
            new Car("Ford", "Focus", 2019, "Black", 20000),
            new Car("Toyota", "Corolla", 2020, "White", 25000),
            new Car("Honda", "Accord", 2021, "Red", 30000),
            new Car("Nissan", "Altima", 2022, "Silver", 28000),
            new Car("Ford", "Fusion", 2017, "Blue", 17000)
        };

        // Пример запроса для получения всех автомобилей заданной марки
        var toyotaCars = cars.Where(car => car.Make == "Toyota");
        Console.WriteLine("\nВсе автомобили марки Toyota:");
        foreach (var car in toyotaCars)
        {
            Console.WriteLine($"{car.Make} {car.Model} ({car.Year})");
        }

        // Пример запроса для получения автомобилей, которые эксплуатируются больше 5 лет
        var oldCars = cars.Where(car => DateTime.Now.Year - car.Year > 5);
        Console.WriteLine("\nАвтомобили, которые эксплуатируются больше 5 лет:");
        foreach (var car in oldCars)
        {
            Console.WriteLine($"{car.Make} {car.Model} ({car.Year})");
        }

        // Пример запроса для подсчета автомобилей заданного цвета и диапазона цены
        var colorCount = cars.Where(car => car.Color == "Red" && car.Price >= 5000 && car.Price <= 20000).Count();
        Console.WriteLine($"\nКоличество красных автомобилей в ценовом диапазоне от 5000 до 20000: {colorCount}");
    }
}


/*
 int[] numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

var evenNumbers = numbers.Where(n => n % 2 == 0);
var sortedNumbers = numbers.OrderByDescending(n => n);
var sum = numbers.Sum();
var doubledNumbers = numbers.Select(n => n * 2);
 */
