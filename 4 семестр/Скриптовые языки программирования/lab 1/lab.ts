//задание 1
console.log('задание 1\n');

function getRandomNumber(numbers: number[]): number {
    let index = Math.floor(Math.random() * numbers.length);
    return numbers[index];
}

function createPhoneNumber(numbers: number[]): string {
    let phoneNumber: string = '(xxx) xxx-xxxx';
    while (phoneNumber.includes('x')) {
        phoneNumber = phoneNumber.replace('x', getRandomNumber(numbers).toString());
    }

    return phoneNumber;
}
  
function createPhoneNumber2(numbers: [number, number, number, number, number, number, number, number, number, number]): string {
    let phoneNumber: string = '(xxx) xxx-xxxx';
    while (phoneNumber.includes('x')) {
        phoneNumber = phoneNumber.replace('x', getRandomNumber(numbers).toString());
    }
    
    return phoneNumber;
}
  
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
console.log(createPhoneNumber(numbers));
console.log(createPhoneNumber2([1, 2, 3, 4, 5, 6, 7, 8, 9, 0]));
  
//задание 2
console.log('\nзадание 2\n');

class Challenge {
    static solution(number: number): number {
      if (number < 0) return 0;
      let sum = 0;
      for (let i = 1; i < number; i++) {
        if (i % 3 === 0 || i % 5 === 0) {
          sum += i;
        }
      }
      return sum;
    }
}  

console.log(Challenge.solution(10));

//задание 3
console.log('\nзадание 3\n');

function rotateArray(nums: number[], k: number): number[] {
    let n = nums.length;
    k = k % n;
    return [...nums.slice(n - k), ...nums.slice(0, n - k)];
}

console.log(rotateArray([1, 2, 3, 4, 5, 6, 7], 3));

//задание 4
console.log('\nзадание 4\n');

function findMedian(nums1: number[], nums2: number[]): number {
    let merged = [...nums1, ...nums2].sort((a, b) => a - b);
    let n = merged.length;
    
    if (n % 2 === 1) {
      return merged[Math.floor(n / 2)];
    } else {
      let mid1 = merged[Math.floor(n / 2) - 1];
      let mid2 = merged[Math.floor(n / 2)];
      return (mid1 + mid2) / 2;
    }
}

console.log(findMedian([1, 3], [2]));
console.log(findMedian([1, 2], [3, 4]));
  