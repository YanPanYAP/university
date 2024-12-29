using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("=== Параллельные вычисления с использованием TPL ===");

        // 1. Поиск простых чисел с использованием задачи
        Task primeTask = Task.Factory.StartNew(() =>
        {
            int limit = 100000;
            var primes = SieveOfEratosthenes(limit);
            Console.WriteLine($"Найдено {primes.Count} простых чисел до {limit}");
        });

        Console.WriteLine($"ID задачи: {primeTask.Id}, Статус: {primeTask.Status}");
        primeTask.Wait();
        Console.WriteLine($"Статус после выполнения: {primeTask.Status}");

        // Оценка производительности
        var sw = Stopwatch.StartNew();
        SieveOfEratosthenes(100000);
        sw.Stop();
        Console.WriteLine($"Время выполнения: {sw.ElapsedMilliseconds} мс");

        // 2. Токен отмены
        CancellationTokenSource cts = new CancellationTokenSource();
        Task cancellableTask = Task.Factory.StartNew(() =>
        {
            for (int i = 0; i < 100; i++)
            {
                if (cts.Token.IsCancellationRequested)
                {
                    Console.WriteLine("Задача отменена");
                    return;
                }
                Thread.Sleep(50);
                Console.WriteLine($"Итерация {i}");
            }
        }, cts.Token);

        Thread.Sleep(500);
        cts.Cancel();
        try
        {
            cancellableTask.Wait();
        }
        catch (AggregateException) { }

        // 3. Три задачи с возвратом результата
        Task<int> task1 = Task.Factory.StartNew(() => Enumerable.Range(1, 1000).Sum());
        Task<int> task2 = Task.Factory.StartNew(() => Enumerable.Range(1, 1000).Select(x => x * x).Sum());
        Task<int> task3 = Task.Factory.StartNew(() => Enumerable.Range(1, 1000).Count());

        Task fourthTask = Task.Factory.ContinueWhenAll(new[] { task1, task2, task3 }, tasks =>
        {
            int result = tasks[0].Result + tasks[1].Result + tasks[2].Result;
            Console.WriteLine($"Результат четвертой задачи: {result}");
        });

        fourthTask.Wait();

        // 4. Continuation task
        Task.Factory.StartNew(() => "Hello").ContinueWith(t =>
        {
            Console.WriteLine($"Результат первой задачи: {t.Result}");
        }).Wait();

        // 5. Parallel.For и Parallel.ForEach
        var data = Enumerable.Range(1, 1000000).ToArray();
        Parallel.For(0, data.Length, i =>
        {
            data[i] *= 2;
        });
        Console.WriteLine($"Пример Parallel.For завершен");

        // 6. Parallel.Invoke
        Parallel.Invoke(
            () => Console.WriteLine("Первая операция"),
            () => Console.WriteLine("Вторая операция"),
            () => Console.WriteLine("Третья операция")
        );

        // 7. BlockingCollection
        BlockingCollection<string> warehouse = new BlockingCollection<string>();
        CancellationTokenSource warehouseCts = new CancellationTokenSource();

        Task.Factory.StartNew(() =>
        {
            string[] products = { "Холодильник", "Телевизор", "Микроволновка", "Пылесос", "Утюг" };
            foreach (var product in products)
            {
                warehouse.Add(product);
                Console.WriteLine($"Добавлено: {product}");
                Thread.Sleep(500);
            }
            warehouse.CompleteAdding();
        });

        Task.Factory.StartNew(() =>
        {
            while (!warehouse.IsCompleted)
            {
                if (warehouse.TryTake(out string item))
                {
                    Console.WriteLine($"Покупатель купил: {item}");
                }
                else
                {
                    Console.WriteLine("Склад пуст");
                }
                Thread.Sleep(300);
            }
        }).Wait();

        // 8. Асинхронный метод
        AsyncExample().Wait();
    }

    static async Task AsyncExample()
    {
        Console.WriteLine("Начало асинхронного метода");
        await Task.Delay(1000);
        Console.WriteLine("Асинхронный метод завершен");
    }

    static ConcurrentBag<int> SieveOfEratosthenes(int limit)
    {
        bool[] isPrime = new bool[limit + 1];
        for (int i = 0; i <= limit; i++) isPrime[i] = true;

        ConcurrentBag<int> primes = new ConcurrentBag<int>();
        isPrime[0] = isPrime[1] = false;

        Parallel.For(2, limit + 1, i =>
        {
            if (isPrime[i])
            {
                for (int j = i * 2; j <= limit; j += i)
                {
                    isPrime[j] = false;
                }
                primes.Add(i);
            }
        });
        return primes;
    }
}
