using System;
using System.IO;
using System.Xml.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Xml.XPath;

public interface ISerializer
{
    void Serialize(object obj, string filePath);
    object Deserialize(string filePath, Type objType);
}

public class BinarySerializer : ISerializer
{
    public void Serialize(object obj, string filePath)
    {
        using (Stream stream = File.Open(filePath, FileMode.Create))
        {
            BinaryFormatter formatter = new BinaryFormatter();
            formatter.Serialize(stream, obj);
        }
    }

    public object Deserialize(string filePath, Type objType)
    {
        using (Stream stream = File.Open(filePath, FileMode.Open))
        {
            BinaryFormatter formatter = new BinaryFormatter();
            return formatter.Deserialize(stream);
        }
    }
}

public class JsonSerializer : ISerializer
{
    public void Serialize(object obj, string filePath)
    {
        string json = JsonConvert.SerializeObject(obj);
        File.WriteAllText(filePath, json);
    }

    public object Deserialize(string filePath, Type objType)
    {
        string json = File.ReadAllText(filePath);
        return JsonConvert.DeserializeObject(json, objType);
    }
}

public class XmlSerializer : ISerializer
{
    public void Serialize(object obj, string filePath)
    {
        using (Stream stream = File.Open(filePath, FileMode.Create))
        {
            System.Xml.Serialization.XmlSerializer formatter = new System.Xml.Serialization.XmlSerializer(obj.GetType());
            formatter.Serialize(stream, obj);
        }
    }

    public object Deserialize(string filePath, Type objType)
    {
        using (Stream stream = File.Open(filePath, FileMode.Open))
        {
            System.Xml.Serialization.XmlSerializer formatter = new System.Xml.Serialization.XmlSerializer(objType);
            return formatter.Deserialize(stream);
        }
    }
}

public class CustomSerializer
{
    public void Serialize(object obj, string format, string filePath)
    {
        ISerializer serializer = GetSerializer(format);
        serializer.Serialize(obj, filePath);
    }

    public object Deserialize(string format, string filePath, Type objType)
    {
        ISerializer serializer = GetSerializer(format);
        return serializer.Deserialize(filePath, objType);
    }

    private ISerializer GetSerializer(string format)
    {
        switch (format.ToLower())
        {
            case "binary":
                return new BinarySerializer();
            case "json":
                return new JsonSerializer();
            case "xml":
                return new XmlSerializer();
            default:
                throw new ArgumentException("Неизвестный формат сериализации");
        }
    }
}

[Serializable]
[XmlInclude(typeof(Flowers))]
[XmlInclude(typeof(Watch))]
[XmlInclude(typeof(Cake))]
[XmlInclude(typeof(Candy))]
public class Product
{
    public string Name { get; set; }
    public double Price { get; set; }

    public string Description;

    public Product() { }

    public Product(string name, double price, string description = "")
    {
        Name = name;
        Price = price;
        Description = description;
    }

    public override string ToString() => $"Продукт: {Name}, Цена: {Price}, Описание: {Description}";
}

[Serializable]
public class Flowers : Product
{
    public Flowers() { }

    public Flowers(string name, double price, string description = "") : base(name, price, description) { }
}

[Serializable]
public class Watch : Product
{
    public Watch() { }

    public Watch(string name, double price, string description = "") : base(name, price, description) { }
}

[Serializable]
public class Cake : Product
{
    public Cake() { }

    public Cake(string name, double price, string description = "") : base(name, price, description) { }
}

[Serializable]
public class Candy : Product
{
    public Candy() { }

    public Candy(string name, double price, string description = "") : base(name, price, description) { }
}

class Program
{
    static void Main()
    {
        // Создаем коллекцию объектов
        var products = new List<Product>
        {
            new Flowers("Роза", 150.0, "Красная роза"),
            new Watch("Часы", 500.0, "Часы с кожаным ремнем"),
            new Cake("Торт", 300.0, "Шоколадный торт"),
            new Candy("Конфеты", 100.0, "Молочные конфеты")
        };

        // Создаем объект класса CustomSerializer
        var customSerializer = new CustomSerializer();

        // Сериализация коллекции в формат JSON
        customSerializer.Serialize(products, "json", "C:/oop13/products.json");

        // Десериализация коллекции из JSON файла
        var deserializedProducts = (List<Product>)customSerializer.Deserialize("json", "C:/oop13/products.json", typeof(List<Product>));

        Console.WriteLine("Десериализованные продукты из JSON:");
        foreach (var product in deserializedProducts)
        {
            Console.WriteLine(product);
        }

        // Дополнительные демонстрации сериализации в других форматах
        customSerializer.Serialize(products, "binary", "C:/oop13/products.bin");
        customSerializer.Serialize(products, "xml", "C:/oop13/products.xml");

        // Десериализация из бинарного файла
        var binaryDeserialized = (List<Product>)customSerializer.Deserialize("binary", "C:/oop13/products.bin", typeof(List<Product>));
        Console.WriteLine("\nДесериализация из бинарного формата:");
        foreach (var product in binaryDeserialized)
        {
            Console.WriteLine(product);
        }

        // Десериализация из XML файла
        var xmlDeserialized = (List<Product>)customSerializer.Deserialize("xml", "C:/oop13/products.xml", typeof(List<Product>));
        Console.WriteLine("\nДесериализация из XML формата:");
        foreach (var product in xmlDeserialized)
        {
            Console.WriteLine(product);
        }

        // Путь к XML-файлу
        string xmlFilePath = "C:/oop13/products.xml";

        // Загрузка XML-документа
        XPathDocument document = new XPathDocument(xmlFilePath);
        XPathNavigator navigator = document.CreateNavigator();

        // XPath: Извлечение всех названий продуктов
        Console.WriteLine("Названия продуктов:");
        XPathNodeIterator nameIterator = navigator.Select("//Product/Name");
        while (nameIterator.MoveNext())
        {
            Console.WriteLine(nameIterator.Current.Value);
        }

        // XPath: Извлечение продуктов с ценой выше 200
        Console.WriteLine("\nПродукты с ценой выше 200:");
        XPathNodeIterator expensiveProductsIterator = navigator.Select("//Product[Price > 200]");
        while (expensiveProductsIterator.MoveNext())
        {
            string productName = expensiveProductsIterator.Current.SelectSingleNode("Name").Value;
            Console.WriteLine(productName);
        }
    }
}
