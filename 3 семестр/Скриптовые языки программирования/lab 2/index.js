//задание 1
function operation(operation, value1, value2) {
    let result;

    if (typeof value1 == 'number' && typeof value2 == 'number') {
        if (operation == '+') {
            result = value1 + value2;

        } else if (operation == '-') {
            result = value1 - value2;

        } else if (operation == '/') {
            result = value1 / value2;

        } else if (operation == '*') {
            result = value1 * value2;
            
        } else  {
            return 'Некорректный оператор!';
        }

        return result;
    }

    return 'Оба значения должны быть числами!';
}

console.log('задание 1');
console.log(operation('+', 5, 10));
console.log(operation('-', 10, 5));
console.log(operation('/', 10, 5));
console.log(operation('*', 5, 10));
console.log(operation('!', 5, 10));
console.log(operation('*', 'j', 10));

//задание 2
console.log('\nзадание 2');

let number = 2;
console.log(qubeSum(number));

function qubeSum(n) {
    let result = 0;

    for (let i = 1; i <= n; i++) {
        result += Math.pow(i, 3);
    }

    return result;
}

//задание 3
console.log('\nзадание 3');

function average(arr) {
    let sum = arr.reduce((buf, num) => buf + num, 0);
    return sum / arr.length;
}

console.log(average([1, 2, 3, 4, 5]));

//задание 4
console.log('\nзадание 4');

function reverse1(str) {
    let result = "";
    let lettersOnly = "";

    for (let i = 0; i < str.length; i++) {
        let char = str[i];

        if ((char >= 'A' && char <= 'Z') || (char >= 'a' && char <= 'z')) {
            lettersOnly += char;
        }
    }

    for (let i = lettersOnly.length - 1; i >= 0; i--) {
        result += lettersOnly[i];
    }

    return result;
}

console.log(reverse1("JavaScript"));
console.log(reverse1("JavaScr53э? ipt"));

function reverse2(str) {
    return str
        .split('')
        .filter(char => /[a-zA-Z]/.test(char))
        .reverse()
        .join('');
}

console.log(reverse2("JavaScript"));
console.log(reverse2("JavaScr53э? ipt"));

//задание 5
console.log('\nзадание 5');

function repeatString(n, s) {
    return s.repeat(n);
}

console.log(repeatString(3, "yan\n"));

//задание 6
console.log('\nзадание 6');

function arrayDifference(arr1, arr2) {
    return arr1.filter(item => !arr2.includes(item));
}

console.log(arrayDifference(["apple", "banana", "orange"], ["banana", "kiwi"]));
