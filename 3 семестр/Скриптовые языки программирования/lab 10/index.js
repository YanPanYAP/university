// задание 1
console.log('Задание 1');
const set = new Set([1, 1, 2, 3, 4]);
console.log(set);

// задание 3
console.log('\nЗадание 3');

const nami = "Lydia";
age = 21;
console.log(delete nami);
console.log(delete age);

// задание 4
console.log('\nЗадание 4');

const numbers = [ 1, 2, 3, 4, 5 ];
const [ y ] = numbers;
console.log(y);

// задание 5
console.log('\nЗадание 5');

const user = { name: "Lydia", age: 21 };
const admin = { admin: true, ...user };
console.log(admin);

// задание 6
console.log('\nЗадание 6');

const person = { name: "Lydia" };
Object.defineProperty(person, "age", { value: 21 });
console.log(person);
console.log(Object.keys(person));

// задание 7
console.log('\nЗадание 7');

const a = {};
const b = { key: "b" };
const c = { key: "c" };
a[b] = 123;
a[c] = 456;
console.log(a[b]);

// задание 8
console.log('\nЗадание 8');

let num = 10;
const increaseNumber = () => num++;
const increasePassedNumber = number => number++;
const num1 = increaseNumber();
const num2 = increasePassedNumber(num1);
console.log(num1);
console.log(num2);

// задание 9
console.log('\nЗадание 9');

const value = {number: 10};
const multiply = (x = {...value}) => {
    console.log((x.number *= 2));
};

multiply();
multiply();
multiply(value);
multiply(value);

// задание 10
console.log('\nЗадание 10');

[ 1, 2, 3, 4 ].reduce((x, y) => console.log(x, y));