<?php
// Подключение к базе данных
$servername = "ваш_хост";
$username = "ваше_имя_пользователя";
$password = "ваш_пароль";
$dbname = "ваша_база_данных";

$conn = new mysqli($servername, $username, $password, $dbname);

// Проверка подключения
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Получение данных из формы
$login = $_POST['login'];
$email = $_POST['email'];
$password = $_POST['password'];

// SQL-запрос для вставки данных в таблицу
$sql = "INSERT INTO ваша_таблица (login, email, password) VALUES ('$login', '$email', '$password')";

if ($conn->query($sql) === TRUE) {
    echo "Данные успешно добавлены в базу данных";
} else {
    echo "Ошибка: " . $sql . "<br>" . $conn->error;
}

// Закрытие соединения с базой данных
$conn->close();
?>