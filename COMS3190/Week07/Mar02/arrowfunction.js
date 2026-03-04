// Regular function
function add(a, b) {
return a + b;
}
// Arrow function
const subtract = (a, b) => {
return a - b;
};
// Using arrow function for a one-liner
const multiply = (a, b) => a * b;
// Using arrow function for a function with no parameters
const greet = () => console.log("Hello, world!");
console.log(add(5, 3)); // Output: 8
console.log(subtract(10, 4)); // Output: 6
console.log(multiply(3, 7)); // Output: 21
greet(); // Output: Hello, world!

let addNumbersTF = (arr) => {
    let total = 0;
    for(let num of arr) {
        total += num
    }
    return total;
}