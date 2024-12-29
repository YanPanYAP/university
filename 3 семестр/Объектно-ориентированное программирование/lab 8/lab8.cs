using System;
using System.Collections.Generic;
using System.Linq;

public delegate void DeleteDelegate();
public class Programmer
{
    public event DeleteDelegate Delete; 
    public event Action Mutate;

    private List<string> tasks;

    public Programmer(List<string> initialTasks)
    {
        tasks = initialTasks;
    }

    public void OnDelete()
    {
        Delete?.Invoke();
    }

    public void OnMutate()
    {
        Mutate?.Invoke();
    }

    public void DeleteFirstOrLast()
    {
        if (tasks.Count > 0)
        {
            var rand = new Random();
            if (rand.Next(0, 2) == 0)
            {
                tasks.RemoveAt(0);
                Console.WriteLine("Удален первый элемент.");
            }
            else
            {
                tasks.RemoveAt(tasks.Count - 1);
                Console.WriteLine("Удален последний элемент.");
            }
        }
    }

    public void CyclicMutate()
    {
        if (tasks.Count > 1)
        {
            var temp = tasks[0];
            tasks.RemoveAt(0);
            tasks.Add(temp);
            Console.WriteLine("Произведено циклическое перемещение.");
        }
    }

    public void ShowTasks()
    {
        Console.WriteLine("Текущие задачи:");
        foreach (var task in tasks)
        {
            Console.WriteLine(task);
        }
    }
}

public class StringProcessor
{
    public static string RemovePunctuation(string input)
    {
        return new string(input.Where(c => !char.IsPunctuation(c)).ToArray());
    }

    public static string AddPrefix(string input, string prefix)
    {
        return prefix + input;
    }

    public static string ToUpperCase(string input)
    {
        return input.ToUpper();
    }

    public static string RemoveExtraSpaces(string input)
    {
        return string.Join(" ", input.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries));
    }

    public static bool IsEmptyOrShort(string input, int length)
    {
        return string.IsNullOrEmpty(input) || input.Length < length;
    }
}

public class TaskManager
{
    private List<Programmer> programmers;

    public TaskManager(List<Programmer> programmers)
    {
        this.programmers = programmers;
    }

    public void TriggerAllDelete()
    {
        foreach (var programmer in programmers)
            programmer.OnDelete();
    }

    public void TriggerAllMutate()
    {
        foreach (var programmer in programmers)
            programmer.OnMutate();
    }
}

class Program
{
    static void Main(string[] args)
    {
        // 1
        var programmer1 = new Programmer(new List<string> { "Задача 1", "Задача 2", "Задача 3" });
        var programmer2 = new Programmer(new List<string> { "Задача A", "Задача B", "Задача C" });

        // Подписка на события
        programmer1.Delete += programmer1.DeleteFirstOrLast;
        programmer1.Mutate += () => programmer1.CyclicMutate();

        programmer2.Delete += programmer2.DeleteFirstOrLast;
        programmer2.Mutate += () => programmer2.CyclicMutate();

        programmer1.Delete += () => Console.WriteLine("Событие удаления выполнено для programmer1.");
        programmer2.Mutate += () => Console.WriteLine("Событие мутации выполнено для programmer2.");

        var taskManager = new TaskManager(new List<Programmer> { programmer1, programmer2 });

        Console.WriteLine("Состояние до событий:");
        programmer1.ShowTasks();
        programmer2.ShowTasks();

        Console.WriteLine("\nЗапуск событий через TaskManager:");
        taskManager.TriggerAllDelete();
        taskManager.TriggerAllMutate();

        Console.WriteLine("\nСостояние после событий:");
        programmer1.ShowTasks();
        programmer2.ShowTasks();

        // 2
        string input = "  Hello, World!   This is a test.   ";

        // делегаты
        Action<string> print = str => Console.WriteLine(str);
        Func<string, string> removePunctuation = StringProcessor.RemovePunctuation;
        Func<string, string, string> addPrefix = StringProcessor.AddPrefix;
        Func<string, string> toUpperCase = StringProcessor.ToUpperCase;
        Func<string, string> removeExtraSpaces = StringProcessor.RemoveExtraSpaces;
        Predicate<string> isShort = str => StringProcessor.IsEmptyOrShort(str, 10);

        string result = removeExtraSpaces(toUpperCase(addPrefix(removePunctuation(input), "Prefix: ")));

        print($"Итоговая обработанная строка: {result}");

        bool isEmpty = isShort(input);
        print($"Строка пустая или слишком короткая: {isEmpty}");
    }
}

/*
Action myDelegate = () => Console.WriteLine("Hello!");
myDelegate();

Action<string> print = str => Console.WriteLine(str);
print("Итоговая");
*/
/*
public event Action MyEvent;

public void TriggerEvent() {
    MyEvent?.Invoke();
}

public void Subscribe() {
    MyEvent += () => Console.WriteLine("Событие произошло!");
}

public void Unsubscribe() {
    MyEvent -= () => Console.WriteLine("Событие произошло!");
}

 */
/*
 List<int> numbers = new List<int> { 1, 2, 3, 4, 5, 6 };
numbers.ForEach(n => Console.WriteLine(n));
 */