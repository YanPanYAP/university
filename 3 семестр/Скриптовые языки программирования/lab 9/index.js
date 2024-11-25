// задание 1
console.log('\nЗадание 1');

class Shape {
    constructor(color) {
        this.color = color;
    }
}

class Square extends Shape {
    constructor(color) {
        super(color);
        this.type = 'square';
        this.size = 'medium';
    }
}

class Circle extends Shape {
    constructor(color) {
        super(color);
        this.type = 'circle';
    }
}

class Triangle extends Shape {
    constructor(color, lines) {
        super(color);
        this.type = 'triangle';
        this.lines = lines;
    }
}

const yellowSquare = new Square('yellow');
const whiteCircle = new Circle('white');
const greenCircle = new Circle('green');
const triangleThree = new Triangle('black', 3);
const smallYellowSquare = new Square('yellow');

const greenCircleProperties = {
    color: greenCircle.color,
    type: greenCircle.type,
};

const triangleProperties = {
    color: triangleThree.color,
    type: triangleThree.type,
    lines: triangleThree.lines,
};

const property = smallYellowSquare.hasOwnProperty('color');

console.log('Зеленый круг:', greenCircleProperties);
console.log('Треугольник с тремя линиями:', triangleProperties);
console.log('Маленький квадрат имеет собственное свойство цвета:', property);

// задание 2
console.log('\nЗадание 2');

class Human {
    constructor(firstName, lastName, yearOfBirth, address) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.yearOfBirth = yearOfBirth;
        this.address = address;
    }

    set age(value) {
        this.yearOfBirth = new Date().getFullYear() - value;
    }

    get age() {
        return new Date().getFullYear() - this.yearOfBirth;
    }

    setAddress(newAddress) {
        this.address = newAddress;
    }
}

class Student extends Human {
    constructor(firstName, lastName, yearOfBirth, address, faculty, course, group, recordBookNumber) {
        super(firstName, lastName, yearOfBirth, address);
        this.faculty = faculty;
        this.course = course;
        this.group = group;
        this.recordBookNumber = recordBookNumber;
    }

    setCourse(newCourse) {
        this.course = newCourse;
    }

    setGroup(newGroup) {
        this.group = newGroup;
    }

    getFullName() {
        return `${this.firstName} ${this.lastName}`;
    }
}

class Faculty {
    constructor(name, groupCount, studentCount) {
        this.name = name;
        this.groupCount = groupCount;
        this.studentCount = studentCount;
    }

    setGroupCount(newCount) {
        this.groupCount = newCount;
    }

    setStudentCount(newCount) {
        this.studentCount = newCount;
    }

    getDev(students) {
        return students.filter(student => student.recordBookNumber[0] === '3').length;
    }

    getGroup(students, group) {
        return students.filter(student => student.group === group);
    }
}

// Пример использования классов
const student1 = new Student('Yan', 'Pan', 2000, 'Minsk', 'FIT', 2, '1', '71201300');
const student2 = new Student('Pan', 'Yan', 2001, 'Minsk', 'FIT', 2, '1', '71201301');

const faculty = new Faculty('ФИТ', 10, 100);
const students = [student1, student2];

// Пример получения информации
console.log(student1.getFullName());
console.log(faculty.getDev(students));
console.log(faculty.getGroup(students, '1'));