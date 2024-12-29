using System;
using System.Collections.Generic;

namespace OnlineStore
{
    // Базовый класс пользователя
    public abstract class User
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Email { get; set; }
        public string Password { get; set; }

        public void SignIn()
        {
            Console.WriteLine($"{Name} вошел в систему.");
        }

        public void SignOut()
        {
            Console.WriteLine($"{Name} вышел из системы.");
        }
    }

    // Класс клиента
    public class Client : User
    {
        public List<Order> Orders { get; set; } = new List<Order>();

        public void CreateOrder(Order order)
        {
            Orders.Add(order);
            Console.WriteLine($"Заказ №{order.Id} создан.");
        }

        public void CancelOrder(int orderId)
        {
            var order = Orders.Find(o => o.Id == orderId);
            if (order != null)
            {
                Orders.Remove(order);
                Console.WriteLine($"Заказ №{orderId} отменен.");
            }
            else
            {
                Console.WriteLine($"Заказ №{orderId} не найден.");
            }
        }
    }

    // Класс администратора
    public class Admin : User
    {
        public void AddProduct(Product product, List<Product> catalog)
        {
            catalog.Add(product);
            Console.WriteLine($"Товар {product.Name} добавлен в каталог.");
        }

        public void RemoveProduct(int productId, List<Product> catalog)
        {
            var product = catalog.Find(p => p.Id == productId);
            if (product != null)
            {
                catalog.Remove(product);
                Console.WriteLine($"Товар {product.Name} удален из каталога.");
            }
            else
            {
                Console.WriteLine($"Товар с ID {productId} не найден.");
            }
        }

        public void ManageBlacklist(Client client, List<Client> blacklist)
        {
            if (!blacklist.Contains(client))
            {
                blacklist.Add(client);
                Console.WriteLine($"{client.Name} добавлен в черный список.");
            }
            else
            {
                Console.WriteLine($"{client.Name} уже в черном списке.");
            }
        }
    }

    // Класс товара
    public class Product
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public decimal Price { get; set; }
        public int Stock { get; set; }
    }

    // Класс заказа
    public class Order
    {
        public int Id { get; set; }
        public List<Product> Products { get; set; } = new List<Product>();
        public decimal TotalPrice { get; set; }
        public string Status { get; set; } = "Создан";

        public void CalculateTotalPrice()
        {
            TotalPrice = 0;
            foreach (var product in Products)
            {
                TotalPrice += product.Price;
            }
            Console.WriteLine($"Общая стоимость заказа: {TotalPrice} руб.");
        }

        public void Pay()
        {
            if (Products.Count > 0)
            {
                Status = "Оплачен";
                Console.WriteLine($"Заказ №{Id} оплачен.");
            }
            else
            {
                Console.WriteLine("Нельзя оплатить пустой заказ.");
            }
        }
    }

    // Тестовая реализация
    class Program
    {
        static void Main(string[] args)
        {
            // Каталог товаров
            List<Product> catalog = new List<Product>();
            // Черный список
            List<Client> blacklist = new List<Client>();

            // Создаем администратора и клиента
            Admin admin = new Admin { Id = 1, Name = "Админ" };
            Client client = new Client { Id = 2, Name = "Клиент", Email = "client@example.com" };

            // Администратор добавляет товары
            admin.AddProduct(new Product { Id = 1, Name = "Ноутбук", Price = 50000, Stock = 10 }, catalog);
            admin.AddProduct(new Product { Id = 2, Name = "Смартфон", Price = 30000, Stock = 20 }, catalog);

            // Клиент создает заказ
            client.SignIn();
            Order order = new Order { Id = 1 };
            order.Products.Add(catalog[0]);
            order.Products.Add(catalog[1]);
            order.CalculateTotalPrice();
            client.CreateOrder(order);

            // Клиент оплачивает заказ
            order.Pay();

            // Администратор добавляет клиента в черный список
            admin.ManageBlacklist(client, blacklist);

            // Клиент выходит из системы
            client.SignOut();
        }
    }
}
