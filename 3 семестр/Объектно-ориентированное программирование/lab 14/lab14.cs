using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Threading;

class Program
{
    static void Main()
    {
        // Задача 1: Отображение запущенных процессов
        Console.WriteLine("\nЗадача 1: Запущенные процессы\n");
        foreach (Process process in Process.GetProcesses())
        {
            try
            {
                Console.WriteLine($"ID: {process.Id}, Имя: {process.ProcessName}, Приоритет: {process.BasePriority}, Время запуска: {process.StartTime}, Состояние: {process.Responding}, Общее время процессора: {process.TotalProcessorTime}");
            }
            catch { }
        }

        // Задача 2: Исследование домена приложения
        Console.WriteLine("\nЗадача 2: Текущий домен приложения\n");
        AppDomain currentDomain = AppDomain.CurrentDomain;
        Console.WriteLine($"Имя домена: {currentDomain.FriendlyName}");
        Console.WriteLine("Загруженные сборки:");
        foreach (Assembly assembly in currentDomain.GetAssemblies())
        {
            Console.WriteLine($" - {assembly.FullName}");
        }

        AppDomain newDomain = AppDomain.CreateDomain("НовыйДомен");
        Console.WriteLine("Создан новый домен.");
        AppDomain.Unload(newDomain);
        Console.WriteLine("Новый домен выгружен.");

        // Задача 3: Поток для вычисления простых чисел
        Console.WriteLine("\nЗадача 3: Вычисление простых чисел\n");
        Thread primeThread = new Thread(() => CalculatePrimes(50));
        primeThread.Name = "ПотокПростыхЧисел";
        primeThread.Priority = ThreadPriority.Normal;
        primeThread.Start();
        primeThread.Join();

        // Задача 4: Два потока для чётных и нечётных чисел
        Console.WriteLine("\nЗадача 4: Чётные и нечётные числа\n");
        int n = 20;
        Thread evenThread = new Thread(() => WriteNumbers(true, n));
        Thread oddThread = new Thread(() => WriteNumbers(false, n));

        evenThread.Priority = ThreadPriority.AboveNormal;
        evenThread.Start();
        oddThread.Start();

        evenThread.Join();
        oddThread.Join();

        // Задача 5: Повторяющаяся задача с использованием Timer
        Console.WriteLine("\nЗадача 5: Таймер\n");
        Timer timer = new Timer(TimerTask, null, 0, 2000);
        Thread.Sleep(10000); // Позволить таймеру работать 10 секунд
        timer.Dispose();
    }

    // Вспомогательные методы

    static void CalculatePrimes(int n)
    {
        StreamWriter writer = null;
        try
        {
            writer = new StreamWriter("primes.txt", false);
            Console.WriteLine($"Поток {Thread.CurrentThread.Name} с ID {Thread.CurrentThread.ManagedThreadId} запущен.");

            for (int i = 2; i <= n; i++)
            {
                if (IsPrime(i))
                {
                    Console.WriteLine(i);
                    writer.WriteLine(i);
                    Thread.Sleep(100); // Эмуляция работы
                }
            }
            Console.WriteLine($"Поток {Thread.CurrentThread.Name} завершён.");
        }
        finally
        {
            writer?.Dispose();
        }
    }

    static bool IsPrime(int number)
    {
        if (number < 2) return false;
        for (int i = 2; i <= Math.Sqrt(number); i++)
        {
            if (number % i == 0) return false;
        }
        return true;
    }

    static readonly object fileLock = new object();

    static void WriteNumbers(bool even, int n)
    {
        lock (fileLock)
        {
            StreamWriter writer = null;
            try
            {
                writer = new StreamWriter("numbers.txt", true);
                for (int i = (even ? 2 : 1); i <= n; i += 2)
                {
                    Console.WriteLine(i);
                    writer.WriteLine(i);
                    Thread.Sleep(even ? 100 : 200); // Разная скорость
                }
            }
            finally
            {
                writer?.Dispose();
            }
        }
    }


    static void TimerTask(object state)
    {
        Console.WriteLine($"Задача таймера выполнена в {DateTime.Now}");
    }
}
