using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
public interface ICollectionOperations<T>
{
    void Add(T item);
    void Remove(T item);
    T Find(string productName);  // Ищем по имени продукта
}

public class CollectionType<T> : ICollectionOperations<T> where T : Product
{
    private List<T> collection;

    public CollectionType()
    {
        collection = new List<T>();
    }

    public void Add(T item)
    {
        try
        {
            collection.Add(item);
            Console.WriteLine($"Элемент {item} добавлен.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при добавлении элемента: {ex.Message}");
        }
    }

    public void Remove(T item)
    {
        try
        {
            if (!collection.Contains(item))
            {
                throw new InvalidOperationException("Элемент не найден в коллекции.");
            }
            collection.Remove(item);
            Console.WriteLine($"Элемент {item} удалён.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при удалении элемента: {ex.Message}");
        }
    }

    // Поиск по имени продукта без делегатов
    public T Find(string productName)
    {
        foreach (var item in collection)
        {
            if (item.Name == productName)
            {
                return item;
            }
        }
        return null;
    }

    public void DisplayAll()
    {
        foreach (var item in collection)
        {
            Console.WriteLine(item);
        }
    }

    public void SaveToFile(string filePath)
    {
        string json = JsonConvert.SerializeObject(collection, Newtonsoft.Json.Formatting.Indented);
        File.WriteAllText(filePath, json);
    }

    public void LoadFromFile(string filePath)
    {
        if (!File.Exists(filePath))
            throw new FileNotFoundException("File not found", filePath);
        string json = File.ReadAllText(filePath);
        collection = JsonConvert.DeserializeObject<List<T>>(json) ?? new List<T>();
    }
}

public class Program
{
    public static void Main()
    {
        // Пример использования с пользовательским классом
        var productCollection = new CollectionType<Product>();
        var product1 = new Product("Product1", 100.5);
        var product2 = new Product("Product2", 200.75);
        productCollection.Add(product1);
        productCollection.Add(product2);
        Console.WriteLine("\nСодержимое коллекции с продуктами:");
        productCollection.DisplayAll();

        // Поиск продукта по имени без использования делегатов
        var foundProduct = productCollection.Find("Product1");
        if (foundProduct != null)
        {
            Console.WriteLine($"\nНайденный продукт: {foundProduct}");
        }

        // Удаление продукта
        productCollection.Remove(product1);
        Console.WriteLine("\nПосле удаления первого продукта:");
        productCollection.DisplayAll();

        // Сохранение в файл
        string filePath = "lab7.json";
        productCollection.SaveToFile(filePath);
        Console.WriteLine($"Collection saved to {filePath}");

        // Загрузка из файла
        CollectionType<Product> loadedCollection = new CollectionType<Product>();
        loadedCollection.LoadFromFile(filePath);
        Console.WriteLine("\nLoaded collection: ");
        loadedCollection.DisplayAll();
    }
}

// Пример пользовательского класса
public class Product
{
    public string Name { get; set; }
    public double Price { get; set; }

    public Product(string name, double price)
    {
        Name = name;
        Price = price;
    }

    public override string ToString()
    {
        return $"Продукт: {Name}, Цена: {Price}";
    }
}


/*
 public static void Swap<T>(ref T a, ref T b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
public static void Main()
    {
        int x = 5, y = 10;
        Swap(ref x, ref y);

        string a = "Hello", b = "World";
        Swap(ref a, ref b);
    }
 */










