using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.Json;

public static class Reflector
{
    public static void WriteAssemblyInfo(string className, string fileName)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return;
        }

        string assemblyName = type.Assembly.FullName;
        bool hasPublicConstructors = type.GetConstructors(BindingFlags.Public | BindingFlags.Instance).Any();

        Console.WriteLine($"Имя сборки: {assemblyName}");
        Console.WriteLine($"Есть ли публичные конструкторы: {(hasPublicConstructors ? "Да" : "Нет")}");

        var info = new
        {
            AssemblyName = assemblyName,
            HasPublicConstructors = hasPublicConstructors
        };

        File.WriteAllText(fileName, JsonSerializer.Serialize(info, new JsonSerializerOptions { WriteIndented = true }));
    }

    public static IEnumerable<string> GetPublicMethods(string className)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return Enumerable.Empty<string>();
        }

        Console.WriteLine("Публичные методы:");
        var methods = type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(m => m.Name);

        foreach (var method in methods)
        {
            Console.WriteLine($"- {method}");
        }

        return methods;
    }

    public static IEnumerable<string> GetFieldsAndProperties(string className)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return Enumerable.Empty<string>();
        }

        var fields = type.GetFields(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(f => "Поле: " + f.Name);
        var properties = type.GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(p => "Свойство: " + p.Name);

        Console.WriteLine("Поля и свойства:");
        foreach (var item in fields.Concat(properties))
        {
            Console.WriteLine($"- {item}");
        }

        return fields.Concat(properties);
    }

    public static IEnumerable<string> GetImplementedInterfaces(string className)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return Enumerable.Empty<string>();
        }

        var interfaces = type.GetInterfaces().Select(i => i.Name);
        Console.WriteLine("Реализованные интерфейсы:");
        foreach (var iface in interfaces)
        {
            Console.WriteLine($"- {iface}");
        }

        return interfaces;
    }

    public static IEnumerable<string> GetMethodsWithParameterType(string className, string parameterType)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return Enumerable.Empty<string>();
        }

        Type paramType = Type.GetType(parameterType);
        if (paramType == null)
        {
            Console.WriteLine($"Тип параметра {parameterType} не найден.");
            return Enumerable.Empty<string>();
        }

        var methods = type.GetMethods().Where(m => m.GetParameters().Any(p => p.ParameterType == paramType)).Select(m => m.Name);

        Console.WriteLine($"Методы с параметром типа {parameterType}:");
        foreach (var method in methods)
        {
            Console.WriteLine($"- {method}");
        }

        return methods;
    }

    public static object Invoke(string className, string methodName, object[] parameters)
    {
        Type type = Type.GetType(className);
        if (type == null)
        {
            Console.WriteLine($"Класс {className} не найден.");
            return null;
        }

        MethodInfo method = type.GetMethod(methodName);
        if (method == null)
        {
            Console.WriteLine($"Метод {methodName} не найден в классе {className}.");
            return null;
        }

        object instance = Activator.CreateInstance(type);
        var result = method.Invoke(instance, parameters);

        Console.WriteLine($"Результат вызова метода {methodName}: {result}");
        return result;
    }

    public static T Create<T>() where T : class
    {
        ConstructorInfo constructor = typeof(T).GetConstructor(Type.EmptyTypes);
        if (constructor == null)
        {
            Console.WriteLine($"Класс {typeof(T).Name} не имеет конструктора без параметров.");
            return null;
        }

        T instance = (T)constructor.Invoke(null);
        Console.WriteLine($"Создан экземпляр класса {typeof(T).Name}.");
        return instance;
    }
}

// Пример использования
public class TestClass
{
    public int Value { get; set; }

    public TestClass() { }

    public TestClass(int value) => Value = value;

    public void Print(string message) => Console.WriteLine(message);

    public void Increment(int amount) => Value += amount;
}

public class Program
{
    public static void Main()
    {
        Reflector.WriteAssemblyInfo("TestClass", "\\\\Mac\\Home\\Documents\\fit\\ООП\\lab 11\\lab11\\lab11\\TestClassInfo.json");
        Reflector.GetPublicMethods("TestClass");
        Reflector.GetFieldsAndProperties("TestClass");
        Reflector.GetImplementedInterfaces("TestClass");
        Reflector.GetMethodsWithParameterType("TestClass", "System.String");
        Reflector.Invoke("TestClass", "Print", new object[] { "Привет, Рефлексия!" });
        Reflector.Create<TestClass>();
    }
}
