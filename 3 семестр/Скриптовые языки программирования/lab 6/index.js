//задание 1
console.log('Задание 1');

let numbers = [ 11, 12, 1, 2 ];
let [ first ] = numbers;
let [ , second, ...other ] = numbers;

console.log(first);
console.log(second);
console.log(other[0]);
console.log(other[1]);

//задание 2
console.log('\nЗадание 2');

let user = {
    name: 'Yan',
    age: 18
}

let { isAdmin = true, ...rest } = user;

let admin = {
    admin: isAdmin,
    ...rest
    // name: rest.name,
    // age: rest.age
}

console.log(admin);

const user2 = {
    name: 'John',
    age: 30
};

const admin2 = {
    admin: true,
    ...user
};

console.log(admin);

//задание 3
console.log('\nЗадание 3');

let store = {
    state: {
      profilePage: {
        posts: [
          { id: 1, message: 'Hi', likesCount: 12 },
          { id: 2, message: 'By', likesCount: 1 },
        ],
        newPostText: 'About me',
      },
      dialogsPage: {
        dialogs: [
          { id: 1, name: 'Andrey' },
          { id: 2, name: 'Viktor' },
          { id: 3, name: 'Sasha' },
          { id: 4, name: 'Valera' }
        ],
        messages: [
          { id: 1, message: 'hi' },
          { id: 2, message: 'hi hi' },
          { id: 3, message: 'hi hi hi' }
        ],
      },
      sidebar: []
    }
};
  
const {
    state: {
        profilePage: { posts, newPostText },
        dialogsPage: { dialogs, messages },
        sidebar
    }
} = store;

posts.forEach(post => console.log(post.likesCount));

const evenDialogs = dialogs.filter(dialog => dialog.id % 2 === 0);
console.log(evenDialogs);

const updatedMessages = messages.map(msg => ({
    ...msg, 
    message: 'Hello user'
}));
  
console.log(updatedMessages);

//задание 4
console.log('\nЗадание 4');

let tasks = [
    { id: 1, title: "HTML & CSS", isDone: true },
    { id: 2, title: "JS", isDone: true },
    { id: 3, title: "ReactJS", isDone: false },
    { id: 4, title: "Rest API", isDone: false },
    { id: 5, title: "GraphQL", isDone: false }
];

let newTask = { id: 6, title: "TypeScript", isDone: false };
let updatedTasks = [...tasks, newTask];

console.log(updatedTasks);
  
//задание 5
console.log('\nЗадание 5');

function sumValues(a, b, c) {
    return a + b + c;
}

const numbers2 = [1, 2, 3];
const result = sumValues(...numbers2);

console.log(result);