class Task {
    constructor(id, name, isCompleted = false) {
        this.id = id;
        this.name = name;
        this.isCompleted = isCompleted;
    }

    changeName(newName) {
        this.name = newName;
    }
    toggleState() {
        this.isCompleted = !this.isCompleted;
    }
    getDetails() {
        return `id: ${this.id}, название: ${this.name}, выполнена: ${this.isCompleted}`;
    }
}

class Todolist {
    constructor(id, name) {
        this.id = id;
        this.name = name;
        this.tasks = [];
    }

    changeName(newName) {
        this.name = newName;
    }
    addTask(task) {
        this.tasks.push(task);
    }
    showTasks() {
        return this.tasks.map(task => task.getDetails()).join('\n');
    }
}

const task1 = new Task(1, "купить");
const task2 = new Task(2, "убрать", true);
const task3 = new Task(3, "закончить");

const myTodolist = new Todolist(1, "задачи");

myTodolist.addTask(task1);
myTodolist.addTask(task2);
myTodolist.addTask(task3);

console.log("все задачи:");
console.log(myTodolist.showTasks());

task1.changeName("купить продукты и фрукты");
task1.toggleState();

console.log("\nизмененные данные задачи:");
console.log(task1.getDetails());

console.log("\nобновленные задачи в списке:");
console.log(myTodolist.showTasks());