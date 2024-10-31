using System;

class Program
{
    static void Main()
    {
        TypesDemo();
        StringDemo();
        ArrayDemo();
        TupleDemo();
        LocalFunctionDemo();
        CheckedUncheckedDemo();

        Console.WriteLine("Нажмите любую клавишу, чтобы закрыть окно...");
        Console.ReadKey();
    }

    // 1) Типы
    static void TypesDemo()
    {
        Console.WriteLine("=== Демонстрация типов ===");

        // a. Примитивные типы
        bool boolVar = true;
        byte byteVar = 255;
        sbyte sbyteVar = -128;
        char charVar = 'A';
        decimal decimalVar = 100.5m;
        double doubleVar = 99.9;
        float floatVar = 8.8f;
        int intVar = 42;
        uint uintVar = 999999;
        long longVar = 1234567890;
        ulong ulongVar = 9876543210;
        short shortVar = -12345;
        ushort ushortVar = 54321;

        Console.WriteLine($"Логический: {boolVar}");
        Console.WriteLine($"Байт: {byteVar}");
        Console.WriteLine($"Знаковый байт: {sbyteVar}");
        Console.WriteLine($"Символ: {charVar}");
        Console.WriteLine($"Десятичный: {decimalVar}");
        Console.WriteLine($"Двойной: {doubleVar}");
        Console.WriteLine($"Плавающая точка: {floatVar}");
        Console.WriteLine($"Целое: {intVar}");
        Console.WriteLine($"Беззнаковое целое: {uintVar}");
        Console.WriteLine($"Длинное: {longVar}");
        Console.WriteLine($"Беззнаковое длинное: {ulongVar}");
        Console.WriteLine($"Короткое: {shortVar}");
        Console.WriteLine($"Беззнаковое короткое: {ushortVar}");

        Console.WriteLine("\n");

        // b. Неявное и явное приведение
        int intVar2 = 10;
        long longVar2 = intVar2;
        double doubleVar2 = intVar2;
        float floatVar2 = intVar2;

        char charVar2 = 'A';
        int intFromChar = charVar2;

        double doubleVar3 = 123.45;
        int intVar3 = (int)doubleVar3;

        long longVar3 = 987654321;
        int intVar4 = (int)longVar3;

        decimal decimalVar2 = 99.99m;
        int intVar5 = (int)decimalVar2;

        Console.WriteLine($"longVar: {longVar2}, doubleVar: {doubleVar2}, floatVar: {floatVar2}");
        Console.WriteLine($"intFromChar: {intFromChar}");
        Console.WriteLine($"intVar3 (из double): {intVar3}");
        Console.WriteLine($"intVar4 (из long): {intVar4}");
        Console.WriteLine($"intVar5 (из decimal): {intVar5}");

        // c. Упаковка и распаковка
        Console.WriteLine("\n");
        int x = 10;
        object obj = x;
        int y = (int)obj;
        Console.WriteLine($"Упакованное значение: {obj}, Распакованное значение: {y}");

        // d. Неявно типизированная переменная
        Console.WriteLine("\n");
        var number = 10;
        Console.WriteLine($"Тип переменной number: {number.GetType()}");

        // e. Nullable переменные
        Console.WriteLine("\n");
        int? nullableInt = null;
        if (nullableInt.HasValue) {
            Console.WriteLine($"Значение: {nullableInt.Value}");
        } else {
            Console.WriteLine("Значение отсутствует (null)");
        }

        // f. Работа с var
        Console.WriteLine("\n");
        var value = 42;
        // value = "Hello"; // Ошибка: Нельзя присвоить строку переменной типа int
    }

    // 2) Строки
    static void StringDemo()
    {
        Console.WriteLine("\n=== Демонстрация строк ===");

        // a. Сравнение строк
        string str1 = "Hello";
        string str2 = "World";
        string str3 = "Hello";

        Console.WriteLine($"str1 == str2: {str1 == str2}");
        Console.WriteLine($"str1 == str3: {str1 == str3}");

        // b. Операции со строками
        Console.WriteLine("\n");
        string str4 = "Hello";
        string str5 = "World";
        string str6 = "C#";

        string concatenated = string.Concat(str4, " ", str5);
        Console.WriteLine($"Сцепление: {concatenated}");

        string copied = string.Copy(str4);
        Console.WriteLine($"Копирование: {copied}");

        string substring = str4.Substring(1, 3);
        Console.WriteLine($"Подстрока: {substring}");

        string[] words = concatenated.Split(' ');
        Console.WriteLine("Разделенная строка: " + string.Join(", ", words));

        string inserted = concatenated.Insert(6, "Beautiful ");
        Console.WriteLine($"Вставка: {inserted}");

        string removed = concatenated.Remove(5, 1);
        Console.WriteLine($"Удаление: {removed}");

        string interpolated = $"Добро пожаловать в {str6}";
        Console.WriteLine($"Интерполяция: {interpolated}");

        // c. Пустая и null строка
        Console.WriteLine("\n");
        string emptyStr = "";
        string nullStr = null;

        Console.WriteLine($"IsNullOrEmpty(emptyStr): {string.IsNullOrEmpty(emptyStr)}");
        Console.WriteLine($"IsNullOrEmpty(nullStr): {string.IsNullOrEmpty(nullStr)}");

        Console.WriteLine($"Длина пустой строки: {emptyStr.Length}");
        // Console.WriteLine(nullStr.Length); // Ошибка
    }

    // 3) Массивы
    static void ArrayDemo()
    {
        Console.WriteLine("\n=== Демонстрация массивов ===");

        // a. Двумерный массив
        int[,] matrix = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };

        for (int i = 0; i < matrix.GetLength(0); i++)
        {
            for (int j = 0; j < matrix.GetLength(1); j++)
            {
                Console.Write($"{matrix[i, j]} ");
            }
            Console.WriteLine();
        }

        // b. Одномерный массив строк
        Console.WriteLine("\n");
        string[] strings = { "Apple", "Banana", "Cherry" };
        Console.WriteLine($"Длина массива: {strings.Length}");
        for (int i = 0; i < strings.Length; i++)
        {
            Console.WriteLine($"Элемент {i}: {strings[i]}");
        }

        Console.Write("Введите индекс для замены: ");
        int index = int.Parse(Console.ReadLine());
        Console.Write("Введите новое значение: ");
        strings[index] = Console.ReadLine();

        Console.WriteLine("Обновленный массив:");
        foreach (var str in strings)
        {
            Console.WriteLine(str);
        }

        // c. Ступенчатый массив
        Console.WriteLine("\n");
        double[][] jaggedArray = new double[3][];
        jaggedArray[0] = new double[2];
        jaggedArray[1] = new double[3];
        jaggedArray[2] = new double[4];

        for (int i = 0; i < jaggedArray.Length; i++)
        {
            Console.WriteLine($"Введите {jaggedArray[i].Length} элементов для строки {i + 1}:");
            for (int j = 0; j < jaggedArray[i].Length; j++)
            {
                jaggedArray[i][j] = double.Parse(Console.ReadLine());
            }
        }

        foreach (var row in jaggedArray)
        {
            foreach (var value in row)
            {
                Console.Write($"{value} ");
            }
            Console.WriteLine();
        }

        // d. Неявно типизированные массивы
        Console.WriteLine("\n");
        var numbers = new[] { 1, 2, 3, 4, 5 };
        var text = "Hello World";

        Console.WriteLine($"Массив: {string.Join(", ", numbers)}");
        Console.WriteLine($"Строка: {text}");
    }

    // 4) Кортежи
    static void TupleDemo()
    {
        Console.WriteLine("\n=== Демонстрация кортежей ===");

        // a. Кортеж из 5 элементов
        var tuple = (42, "Hello", 'A', "World", 123456789UL);
        Console.WriteLine($"Кортеж: {tuple}");
        Console.WriteLine($"1-й элемент: {tuple.Item1}, 3-й элемент: {tuple.Item3}, 4-й элемент: {tuple.Item4}");

        // b. Распаковка кортежа
        Console.WriteLine("\n");
        (int number, string word, char symbol, string anotherWord, ulong longNumber) = tuple;
        Console.WriteLine($"Распакованный кортеж: {number}, {word}, {symbol}, {anotherWord}, {longNumber}");
    }

    // 5) Локальные функции
    static void LocalFunctionDemo()
    {
        Console.WriteLine("\n=== Демонстрация локальных функций ===");

        // Локальная функция для вычисления факториала
        int Factorial(int n)
        {
            if (n == 0) return 1;
            return n * Factorial(n - 1);
        }

        int number = 5;
        Console.WriteLine($"Факториал {number} равен {Factorial(number)}");
    }

    // 6) Checked/Unchecked
    static void CheckedUncheckedDemo()
    {
        Console.WriteLine("\n=== Демонстрация Checked/Unchecked ===");

        // a. Checked
        try
        {
            checked
            {
                int maxValue = int.MaxValue;
                Console.WriteLine($"Максимальное значение int: {maxValue}");
                int overflow = maxValue + 1;
            }
        }
        catch (OverflowException e)
        {
            Console.WriteLine($"OverflowException (checked): {e.Message}");
        }

        // b. Unchecked
        unchecked
        {
            int maxValue = int.MaxValue;
            int overflow = maxValue + 1;
            Console.WriteLine($"Overflow (unchecked): {overflow}");
        }
    }
}
