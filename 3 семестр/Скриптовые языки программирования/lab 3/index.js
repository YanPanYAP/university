//задание 1
console.log('задание 1');

function handshake1(arr) {
    return arr.reduce((buf, num) => buf.concat(Array.isArray(num) ? handshake1(num) : num), []);
}

console.log(handshake1([1, [1, 2, [3, 4]], [2, 4]])); 

function handshake2(arr) {
    return arr.flat(Infinity);
}

console.log(handshake2([1, [1, 2, [3, 4]], [2, 4]])); 

//задание 2
console.log('\nзадание 2');

function sumHandshake(arr) {

    let flatArray = arr.flat(Infinity);
    let sum = flatArray.reduce((buf, num) => buf + num, 0);

    return sum;
}

console.log(sumHandshake([1, [1, 2, [3, 4]], [2, 4]]));
 
//задание 3
console.log('\nзадание 3');

function groupStudents(students) {
    return students
        .filter(student => student.age > 17)
        .reduce((acc, student) => {

            const { id } = student;
            if (!acc[id]) {
                acc[id] = [student];
            } else {
                acc[id] = [...acc[id], student];
            }
            return acc;
        }, {});
}

const students = [
    { name: 'Alice', age: 18, id: 1 },
    { name: 'Bob', age: 16, id: 1 },
    { name: 'Charlie', age: 19, id: 2 },
    { name: 'David', age: 20, id: 1 },
    { name: 'Eve', age: 17, id: 2 }
];

console.log(groupStudents(students));

//задание 4
console.log('\nзадание 4');

function unHandshake(phrase) {
    let splitPhrase = phrase.split('')
    .map(char => char.charCodeAt(0))
    .join('');
    
    let splitPhrase2 = splitPhrase.replace(/7/g, '1');

    let total1 = Number(splitPhrase);
    let total2 = Number(splitPhrase2);

    let resualt = total1 - total2;

    return resualt
}

console.log(unHandshake('ABC'));

//задание 5
console.log('\nзадание 5');

function extend(...objects) {
    return Object.assign({}, ...objects);
}

console.log(extend({ a: 1, b: 2 }, { c: 3 }));
console.log(extend({ a: 1, b: 2 }, { c: 3 }, { d: 4 }));
console.log(extend({ a: 1, b: 2 }, { a: 3, c: 3 }));

//задание 6
console.log('\nзадание 6');

function createPyramid(levels) {
    const pyramid = new Array(levels);

    for (let i = 0; i < levels; i++) {
        const spaces = ' '.repeat(levels - i - 1);
        const stars = '*'.repeat(2 * i + 1);

        pyramid[i] = spaces + stars + spaces;
    }

    return pyramid;
}

const pyramid = createPyramid(3);
console.log(JSON.stringify(pyramid, null, 2));