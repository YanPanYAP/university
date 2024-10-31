using System;
using System.Linq;

public class List
{
    private Node head;

    // Вложенный класс узла
    public class Node
    {
        public int Data { get; set; }
        public Node Next { get; set; }

        public Node(int data)
        {
            Data = data;
            Next = null;
        }
    }

    // Вложенный класс Production
    public class Production
    {
        public int Id { get; set; }
        public string OrganizationName { get; set; }

        public Production(int id, string organizationName)
        {
            Id = id;
            OrganizationName = organizationName;
        }
    }

    // Вложенный класс Developer
    public class Developer
    {
        public string FullName { get; set; }
        public int Id { get; set; }
        public string Department { get; set; }

        public Developer(string fullName, int id, string department)
        {
            FullName = fullName;
            Id = id;
            Department = department;
        }
    }

    // Конструктор по умолчанию
    public List()
    {
        head = null;
    }

    // Конструктор для инициализации списка из массива
    public List(int[] items)
    {
        head = null; // Обнуляем голову списка перед добавлением элементов
        foreach (var item in items)
        {
            Add(item);
        }
    }

    // Метод добавления элемента в конец списка
    public void Add(int item)
    {
        if (head == null)
        {
            head = new Node(item);
        }
        else
        {
            Node current = head;
            while (current.Next != null)
            {
                current = current.Next;
            }
            current.Next = new Node(item);
        }
    }

    // Свойство для получения количества элементов
    public int Count
    {
        get
        {
            int count = 0;
            Node current = head;
            while (current != null)
            {
                count++;
                current = current.Next;
            }
            return count;
        }
    }

    // Свойство для доступа к первому элементу
    public Node Head
    {
        get => head;
        set => head = value;
    }

    // Перегрузка оператора +
    public static List operator +(List list1, List list2)
    {
        List result = new List();
        Node current = list1.head;

        while (current != null)
        {
            result.Add(current.Data);
            current = current.Next;
        }

        current = list2.head;
        while (current != null)
        {
            result.Add(current.Data);
            current = current.Next;
        }

        return result;
    }

    // Перегрузка оператора --
    public static List operator --(List list)
    {
        if (list.head != null)
        {
            list.head = list.head.Next;
        }
        return list;
    }

    // Перегрузка оператора ==
    public static bool operator ==(List list1, List list2)
    {
        if (ReferenceEquals(list1, list2)) return true;
        if (list1 is null || list2 is null) return false;

        Node current1 = list1.head;
        Node current2 = list2.head;

        while (current1 != null && current2 != null)
        {
            if (current1.Data != current2.Data)
                return false;
            current1 = current1.Next;
            current2 = current2.Next;
        }

        return current1 == null && current2 == null;
    }

    // Перегрузка оператора !=
    public static bool operator !=(List list1, List list2)
    {
        return !(list1 == list2);
    }

    // Перегрузка операторов true и false
    public static implicit operator bool(List list)
    {
        return list.head == null; // возвращает true, если список пуст
    }

    // Индексатор
    public int this[int index]
    {
        get
        {
            if (index < 0) throw new ArgumentOutOfRangeException(nameof(index), "Индекс должен быть положительным.");

            Node current = head;
            for (int i = 0; i < index; i++)
            {
                if (current == null) throw new IndexOutOfRangeException();
                current = current.Next;
            }
            if (current == null) throw new IndexOutOfRangeException();
            return current.Data;
        }
    }

    // Переопределение методов
    public override bool Equals(object obj)
    {
        if (obj is List otherList)
        {
            return this == otherList;
        }
        return false;
    }

    public override int GetHashCode()
    {
        return head?.GetHashCode() ?? 0;
    }
}

public static class StatisticOperation
{
    public static int Sum(List list)
    {
        int sum = 0;
        var current = list.Head;
        while (current != null)
        {
            sum += current.Data;
            current = current.Next;
        }
        return sum;
    }

    public static int DifferenceMaxMin(List list)
    {
        if (list.Head == null) throw new InvalidOperationException("Список пуст.");

        int max = int.MinValue;
        int min = int.MaxValue;

        var current = list.Head;
        while (current != null)
        {
            if (current.Data > max) max = current.Data;
            if (current.Data < min) min = current.Data;
            current = current.Next;
        }

        return max - min;
    }

    public static int Count(List list)
    {
        return list.Count;
    }

    // Метод расширения для типа string
    public static int LastNumber(this string str)
    {
        var words = str.Split(new char[] { ' ', ',', '.', '!', '?' }, StringSplitOptions.RemoveEmptyEntries);
        foreach (var word in words.Reverse())
        {
            if (int.TryParse(word, out int number))
            {
                return number;
            }
        }
        throw new InvalidOperationException("В строке нет чисел.");
    }

    // Метод расширения для удаления заданного элемента из списка
    public static void RemoveItem(this List list, int item)
    {
        if (list.Head == null) return;

        if (list.Head.Data == item)
        {
            list.Head = list.Head.Next;
            return;
        }

        List.Node current = list.Head;
        while (current.Next != null)
        {
            if (current.Next.Data == item)
            {
                current.Next = current.Next.Next;
                return;
            }
            current = current.Next;
        }
    }
}

// Точка входа в программу
class Program
{
    static void Main(string[] args)
    {
        // Инициализация объектов Production и Developer
        var production = new List.Production(1, "YAP corporation");
        var developer = new List.Developer("Павлович Ян Андреевич", 1, "Разработка");

        // Создание списка
        List myList1 = new List(new int[] { 1, 2, 3, 4, 5 });
        List myList2 = new List(new int[] { 6, 7, 8 });

        // Тестирование перегруженных операторов
        List combinedList = myList1 + myList2;
        Console.WriteLine("\nОбъединённый список:");
        for (int i = 0; i < combinedList.Count; i++)
        {
            Console.Write(combinedList[i] + " ");
        }
        Console.WriteLine();

        // Удаление первого элемента из списка
        --myList1;
        Console.WriteLine("\nСписок после удаления первого элемента:");
        for (int i = 0; i < myList1.Count; i++)
        {
            Console.Write(myList1[i] + " ");
        }
        Console.WriteLine();

        // Проверка на равенство списков
        Console.WriteLine("\nmyList1 == myList2: " + (myList1 == myList2));

        // Проверка, пуст ли список
        Console.WriteLine("\nmyList1 пуст: " + (myList1 ? "да" : "нет"));

        // Пример использования статистических операций
        Console.WriteLine("\nСумма элементов: " + StatisticOperation.Sum(myList1));
        Console.WriteLine("Разница между максимумом и минимумом: " + StatisticOperation.DifferenceMaxMin(myList1));
        Console.WriteLine("Количество элементов: " + StatisticOperation.Count(myList1));

        // Пример методов расширения
        string testString = "The last number is 42 and the first is 7.";
        Console.WriteLine("\nПоследнее число в строке: " + testString.LastNumber());

        myList1.RemoveItem(3);
        Console.WriteLine("\nСписок после удаления элемента 3:");
        for (int i = 0; i < myList1.Count; i++)
        {
            Console.Write(myList1[i] + " ");
        }
        Console.WriteLine();

        // Вывод информации о производстве и разработчике
        Console.WriteLine($"\nПроизводство: {production.OrganizationName}");
        Console.WriteLine($"Разработчик: {developer.FullName}, Отдел: {developer.Department}");
    }
}

