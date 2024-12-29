using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
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

public interface ISet<T>
{
    void Add(T item);
    bool Remove(T item);
    bool Contains(T item);
    void Display();
}

public class ImageSet : ISet<Image>
{
    private LinkedList<Image> images;

    public ImageSet()
    {
        images = new LinkedList<Image>();
    }

    public void Add(Image image)
    {
        images.AddLast(image);
    }

    public bool Remove(Image image)
    {
        return images.Remove(image);
    }

    public bool Contains(Image image)
    {
        return images.Contains(image);
    }

    public void Display()
    {
        foreach (var image in images)
        {
            Console.WriteLine(image);
        }
    }
}

public class UniversalCollection<T>
{
    private LinkedList<T> collection;

    public UniversalCollection()
    {
        collection = new LinkedList<T>();
    }

    public void Add(T item)
    {
        collection.AddLast(item);
    }

    public void RemoveRange(int n)
    {
        var current = collection.First;
        for (int i = 0; i < n && current != null; i++)
        {
            var next = current.Next;
            collection.Remove(current);
            current = next;
        }
    }

    public void Display()
    {
        foreach (var item in collection)
        {
            Console.WriteLine(item);
        }
    }

    public void CopyTo(Dictionary<int, T> dictionary)
    {
        int key = 0;
        foreach (var item in collection)
        {
            dictionary[key++] = item;
        }
    }
}

public class Program
{
    public static void Main()
    {
        // 1
        Console.WriteLine("Операции с ImageSet:");
        var imageSet = new ImageSet();
        var image1 = new Image("Природа", "1920x1080");
        var image2 = new Image("Городской пейзаж", "2560x1440");

        imageSet.Add(image1);
        imageSet.Add(image2);

        Console.WriteLine("Все изображения:");
        imageSet.Display();

        imageSet.Remove(image1);
        Console.WriteLine("\nПосле удаления изображения Природа:");
        imageSet.Display();

        // 2
        Console.WriteLine("\nОперации с UniversalCollection<int>:");
        var intCollection = new UniversalCollection<int>();
        intCollection.Add(1);
        intCollection.Add(2);
        intCollection.Add(3);
        intCollection.Add(4);

        Console.WriteLine("Начальная коллекция:");
        intCollection.Display();

        // Удаляем 2 элемента
        intCollection.RemoveRange(2);

        Console.WriteLine("\nПосле удаления 2 элементов:");
        intCollection.Display();

        // Добавляем новые элементы
        intCollection.Add(5);
        intCollection.Add(6);

        Console.WriteLine("\nПосле добавления элементов:");
        intCollection.Display();

        // Копируем в Dictionary
        var dictionary = new Dictionary<int, int>();
        intCollection.CopyTo(dictionary);

        Console.WriteLine("\nВторая коллекция (Dictionary):");
        foreach (var kvp in dictionary)
        {
            Console.WriteLine($"{kvp.Key}: {kvp.Value}");
        }

        // Ищем заданное значение
        var value = 5;
        if (dictionary.ContainsValue(value))
        {
            Console.WriteLine($"\nЗначение {value} найдено в словаре.");
        }

        // 3
        Console.WriteLine("\nОперации с ObservableCollection<Image>:");
        ObservableCollection<Image> observableImages = new ObservableCollection<Image>();
        observableImages.CollectionChanged += ObservableImages_CollectionChanged;

        observableImages.Add(new Image("Закат", "1920x1080"));
        observableImages.Add(new Image("Океан", "2560x1440"));
        observableImages.Add(new Image("Горы", "3840x2160"));

        Console.WriteLine("\nИзображения после добавления:");
        foreach (var image in observableImages)
        {
            Console.WriteLine(image);
        }


        observableImages.RemoveAt(0);
        Console.WriteLine("\nИзображения после удаления:");
        foreach (var image in observableImages)
        {
            Console.WriteLine(image);
        }
    }

    private static void ObservableImages_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
    {
        Console.WriteLine($"\nКоллекция изменена: {e.Action}");
    }


}

/*
List<int> numbers = new List<int> { 1, 2, 3, 4, 5 };
numbers.Add(6);
numbers.Remove(3);
numbers.ForEach(n => Console.WriteLine(n));
foreach (int number in numbers)
{
    Console.WriteLine(number);
}
 */