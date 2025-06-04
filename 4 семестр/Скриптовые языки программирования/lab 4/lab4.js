//задание 1
console.log("задание 1\n");

const myPromise = new Promise((resolve, reject) => {
  setTimeout(() => {
    const randomNumber = Math.random();
    resolve(randomNumber);
  }, 3000);
});

myPromise.then((result) => {
  console.log(result);
});

//задание 2
console.log("\nзадание 2\n");

function createPromise(delay) {
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      const randomNumber = Math.random();
      resolve(randomNumber);
    }, delay);
  });
}

const promises = [
  createPromise(3000),
  createPromise(3000),
  createPromise(3000),
];

Promise.all(promises)
  .then((results) => {
    console.log(results);
  })
  .catch((error) => {
    console.error(error);
  });

//задание 3
console.log("\nзадание 3\n");

let pr = new Promise((res, rej) => {
  rej("ku");
});

pr.then(() => console.log(1))
  .catch(() => console.log(2))
  .catch(() => console.log(3))
  .then(() => console.log(4))
  .then(() => console.log(5));

//задание 4
console.log("\nзадание 4\n");

const promise = new Promise((resolve, reject) => {
  resolve(21);
});

promise
  .then((result) => {
    console.log(result);
    return result;
  })
  .then((result) => {
    console.log(result * 2);
  });

//задание 5
console.log("\nзадание 5\n");

async function run() {
  const promise = new Promise((resolve, reject) => {
    resolve(21);
  });

  const result1 = await promise;
  console.log(result1);

  const result2 = result1 * 2;
  console.log(result2);
}

run();
