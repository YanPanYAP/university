//задание 1
console.log('задание 1');

let products1 = new Set();

let apple = { name: 'Apple' };
let orange = { name: 'orange' };
let potato = { name: 'potato' };

products1.add(apple);
products1.add(orange);
products1.add(potato);
products1.delete(potato);

console.log(products1);
console.log(products1.size);
console.log(products1.has(apple));

//задание 2
console.log('\nзадание 2');

class Student {
    constructor(id, group, fullName) {
        this.id = id;
        this.group = group;
        this.fullName = fullName;
    }
}

const students = new Set();

function addStudent(id, group, fullName) {
    const student = new Student(id, group, fullName);
    students.add(student);
}

function removeStudentById(id) {
    for (let student of students) {
        if (student.id === id) {
            students.delete(student);
            break;
        }
    }
}

function filterStudentsByGroup(group) {
    return [...students].filter(student => student.group === group);
}

function sortStudentsById() {
    return [...students].sort((a, b) => a.id - b.id);
}

addStudent(101, '1', 'Иванов Иван');
addStudent(102, '2', 'Петров Петр');
addStudent(103, '1', 'Сидоров Сидор');

console.log('Список студентов:', [...students]);
removeStudentById(102);
console.log('После удаления студента с id 102:', [...students]);
console.log('Фильтрация по группе A1:', filterStudentsByGroup('1'));
console.log('Сортировка по номеру зачетки:', sortStudentsById());

//задание 3
console.log('\nзадание 3');

class Product {
    constructor(id, productName, count, price) {
        this.id = id;
        this.productName = productName;
        this.count = count;
        this.price = price;
    }
}

const products = new Map();

function addProduct(id, productName, count, price) {
    const product = new Product(id, productName, count, price);
    products.set(id, product);
}

function removeProduct(id) {
    products.delete(id);
}

function removeProductByName(name) {
    for (let [id, product] of products) {
        if (product.name === name) {
            products.delete(id);
        }
    }
}

function updateProductQuantity(id, newCount) {
    if (products.has(id)) {
        products.get(id).count = newCount;
    }
}

function updateProductPrice(id, newPrice) {
    if (products.has(id)) {
        products.get(id).price = newPrice;
    }
}

function calculateTotal() {
    let totalItems = 0;
    let totalPrice = 0;

    for (let product of products.values()) {
        totalItems += product.count;
        totalPrice += product.count * product.price;
    }

    return { totalItems, totalPrice };
}

addProduct(1, 'Apple', 10, 1.5);
addProduct(2, 'Banana', 5, 2.0);
addProduct(3, 'Orange', 8, 1.8);

console.log('Список продуктов:', [...products.values()]);

removeProduct(2);
console.log('После удаления Banana:', [...products.values()]);

updateProductQuantity(3, 12);
updateProductPrice(3, 2.0);
console.log('После обновления Orange:', [...products.values()]);

removeProductByName('Apple');
console.log('После удаления Apple:', [...products.values()]);

const totals = calculateTotal();
console.log(`Всего позиций: ${totals.totalItems}`);
console.log(`Общая стоимость: ${totals.totalPrice}`);

//задание 4
console.log('\nзадание 4');

const cache = new WeakMap();

function calculate(param) {
    if (cache.has(param)) {
        console.log('Значение взято из кэша');
        return cache.get(param);
    }

    const result = param.value * 2;
    
    cache.set(param, result);
    console.log('Значение вычислено и сохранено в кэш');
    
    return result;
}

const obj = { value: 5 };
console.log(calculate(obj));
console.log(calculate(obj));
