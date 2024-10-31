//задание 1
let a = 5;              //number
let name = "Name";      //string
let i = 0;              //number
let double = 0.23;      //number
let resualt = 1/0;      //number
let answer = true;      //boolean
let no = null;          //оbject

//задание 2
console.log("\nзадание 2");
let sideB = 5;
let sideA = 45;
let heightA = 21;

let squareB = sideB * sideB;
let squareA = sideA * heightA;

let count = Math.floor(squareA / squareB);
console.log(count);

//задание 3
console.log("\nзадание 3");
let j = 2;
let k = ++j;
let b = j++;

console.log(k === b ? "k равно b" : "л не равно b");

//задание 4
console.log("\nзадание 4");
console.log("Котик" == "котик" ? "Равны" : "Не равны");
console.log("Котик" == "китик" ? "Равны" : "Не равны");
console.log("Кот" == "Котик" ? "Равны" : "Не равны");
console.log("Привет" == "Пока" ? "Равны" : "Не равны");
console.log(73 == "53" ? "Равны" : "Не равны");
console.log(false == 0 ? "Равны" : "Не равны");
console.log(54 == true ? "Равны" : "Не равны");
console.log(123 == false ? "Равны" : "Не равны");
console.log(true == "3" ? "Равны" : "Не равны");
console.log(3 == "5мм" ? "Равны" : "Не равны");
console.log(8 == "-2" ? "Равны" : "Не равны");
console.log(34 == "34" ? "Равны" : "Не равны");
console.log(null == undefined ? "Равны" : "Не равны");

//задание 6
console.log("\nзадание 6");
let russian = true;
let math = false;
let english = false;

if (russian && math && english) {
    console.log("Переведен на следующий курс");
} else if (!russian && !math && !english) {
    console.log("Отчислен");
} else {
    console.log("Ожидает пересдача");
}

//задание 7
console.log("\nзадание 7");
true + true
0 + "5"
5 + "мм"
8 / Infinity
9 * "\n9"
null - 1
"5" - 2
"5px" - 3
true - 3
7 || 0

console.log(true + true);   // 2
console.log(0 + "5");       // "05"
console.log(5 + "мм");      // "5мм"
console.log(8 / Infinity);  // 0
console.log(9 * "\n9");     // 81
console.log(null - 1);      // -1
console.log("5" - 2);       // 3
console.log("5px" - 3);     // NaN
console.log(true - 3);      // -2
console.log(7 || 0);        // 7

//задание 8
console.log("\nзадание 8");
for (let i = 1; i <= 10; i++) {
    if (i % 2 === 0) {
        console.log(i + 2);
    } else {
        console.log(`${i}мм`);
    }
}

//задание 11
console.log("\nзадание 11");
function params(a, b) {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
}

const params2 = function(a, b) {
    return a === b ? 4 * a : a * b;
};

const params3 = (a, b) => a === b ? 4 * a : a * b;

console.log(params(5, 5));
console.log(params2(5, 10));
console.log(params3(10, 10));

