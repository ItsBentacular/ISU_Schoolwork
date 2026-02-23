/*
Ben Hurley
Feb 13, 2026
Activity 06 - Variables
*/

console.log("---- I am in A R R A Y S ----")

// Q1 : Index of each color
let colors = ['red', 'blue', 'orange']
for(let color in colors) {
    console.log(color);
}

// Q2 : Add a new element 'green'
colors[3] = 'green';
colors.push('brown')

// Q3 : Length of the Array
console.log(colors);
console.log(colors.length)