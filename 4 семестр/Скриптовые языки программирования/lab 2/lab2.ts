//задание 1
console.log('задание 1\n');

interface Student {
    id: number,
    name: string,
    group: number
}

const array: Student[] = [
    { id: 1, name: 'Vasya', group: 10 },
    { id: 2, name: 'Ivan', group: 11 },
    { id: 3, name: 'Masha', group: 12 },
    { id: 4, name: 'Petya', group: 10 },
    { id: 5, name: 'Kira', group: 11 },
];

//задание 2
console.log('\nзадание 2\n');

interface CarsType {
    manufacturer: string,
    model: string
}

let car: CarsType = { manufacturer: "", model: "" };
car.manufacturer = "manufacturer";
car.model = "model";

//задание 3
console.log('\nзадание 3\n');

interface ArrayCarsType {
    cars: CarsType[];
}

const car1: CarsType = { manufacturer: "manufacturer", model: "model" };
const car2: CarsType = { manufacturer: "manufacturer", model: "model" };

const arrayCars: ArrayCarsType[] = [{ cars: [car1, car2] }];

//задание 4
console.log('\nзадание 4\n');

type MarkFilterType = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10;
type GroupFilterType = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12;
type DoneType = boolean;

interface MarkType {
    subject: string;
    mark: MarkFilterType;
    done: DoneType;
}

interface StudentType {
    id: number;
    name: string;
    group: GroupFilterType;
    marks: MarkType[];
}

interface GroupType {
    students: StudentType[];
    studentsFilter: (group: GroupFilterType) => StudentType[];
    marksFilter: (mark: MarkFilterType) => StudentType[];
    deleteStudent: (id: number) => void;
}

let students: StudentType[] = [
    {id: 1, name: 'Yan', group:1, marks: [{subject: 'Math', mark: 5, done: true}]},
    {id: 2, name: 'Ya', group:2, marks: [{subject: 'Math', mark: 4, done: true}]},
    {id: 3, name: 'Y', group:1, marks: [{subject: 'Math', mark: 3, done: true}]}
];

class Group implements GroupType {
    constructor(public students: StudentType[]) {}
    
    studentsFilter = (group: GroupFilterType) => this.students.filter(s => s.group === group);
    marksFilter = (mark: MarkFilterType) => this.students.filter(s => s.marks.some(m => m.mark === mark));
    deleteStudent = (id: number) => this.students = this.students.filter(s => s.id !== id);
    
}

let group = new Group(students);

console.log(group.studentsFilter(1));
console.log(group.marksFilter(5));
group.deleteStudent(2);
console.log(group.students);
