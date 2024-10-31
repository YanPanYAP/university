//задание 1
console.log('Задание 1');

let person = {
    name: 'John',
    age: 30,

    greet: function() {
        console.log(`Hello, my name is ${this.name}`);
    },
    ageAfterYears: function(years) {
        return this.age + years;
    }
};

person.greet(); 
console.log(person.ageAfterYears(5));
  
//задание 2
console.log('\nЗадание 2');

let car = {
    model: 'Tesla Model S',
    year: 2020,

    getInfo: function() {
        console.log(`Car model: ${this.model}, Year: ${this.year}`);
    }
};
  
car.getInfo();
  
//задание 3
console.log('\nЗадание 3');

function Book(title, author) {
    this.title = title;
    this.author = author;
  
    this.getTitle = function() {
      return this.title;
    };
  
    this.getAuthor = function() {
      return this.author;
    };
}
  
let myBook = new Book('1984', 'George Orwell');
console.log(myBook.getTitle());
console.log(myBook.getAuthor());
  
//задание 4
console.log('\nЗадание 4');

let team = {
    players: ['Alice', 'Bob', 'Charlie'],

    showPlayers: function() {
      this.players.forEach(function(player) {
        console.log(player);
      });
    }
};
  
team.showPlayers();
  
//задание 5
console.log('\nЗадание 5');

const counter = (function() {
    let count = 0;
  
    return {
      increment: function() {
        return ++count;
      },
      decrement: function() {
        return --count;
      },
      getCount: function() {
        return count;
      }
    };
})();
  
console.log(counter.increment());
console.log(counter.increment());
console.log(counter.decrement());
console.log(counter.getCount());
  

//задание 6
console.log('\nЗадание 6');

let item = {};
Object.defineProperty(item, 'price', {
    value: 100,
    writable: true,
    configurable: true
});

console.log(item.price);

Object.defineProperty(item, 'price', {
    writable: false,
    configurable: false
});

item.price = 200;
console.log(item.price);

//задание 7
console.log('\nЗадание 7');

let circle = {
    radius: 5,

    get area() {
      return Math.PI * this.radius ** 2;
    },
    get getRadius() {
      return this.radius;
    },
    set setRadius(value) {
      this.radius = value;
    }
};
  
console.log(circle.area);
circle.setRadius = 10;
console.log(circle.area);

//задание 8
console.log('\nЗадание 8');

let car2 = {};
Object.defineProperties(car2, {
    make: { value: 'Tesla', writable: true, configurable: true },
    model: { value: 'Model X', writable: true, configurable: true },
    year: { value: 2021, writable: true, configurable: true }
});

console.log(car2.make); // Tesla

Object.defineProperties(car2, {
    make: { writable: false, configurable: false },
    model: { writable: false, configurable: false },
    year: { writable: false, configurable: false }
});

car2.make = 'BMW'; // Ошибка
console.log(car2.make);

//задание 9
console.log('\nЗадание 9');

let numbers = [1, 2, 3];
Object.defineProperty(numbers, 'sum', {
    get: function() {
        return this.reduce((acc, val) => acc + val, 0);
    },
    configurable: false,
    enumerable: false
});

console.log(numbers.sum);

//задание 10
console.log('\nЗадание 10');

let rectangle = {
    width: 10,
    height: 5,
    get area() {
      return this.width * this.height;
    },
    get getWidth() {
      return this.width;
    },
    set setWidth(value) {
      this.width = value;
    },
    get getHeight() {
      return this.height;
    },
    set setHeight(value) {
      this.height = value;
    }
};
  
console.log(rectangle.area);
rectangle.setWidth = 20;
rectangle.setHeight = 10;
console.log(rectangle.area);

//задание 11
console.log('\nЗадание 11');

let user = {
    firstName: 'John',
    lastName: 'Doe',

    get fullName() {
      return `${this.firstName} ${this.lastName}`;
    },
    set fullName(name) {
      [this.firstName, this.lastName] = name.split(' ');
    }
};
  
console.log(user.fullName);
user.fullName = 'Jane Smith';
console.log(user.fullName);
  