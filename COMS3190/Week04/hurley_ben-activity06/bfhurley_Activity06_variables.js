/*
Ben Hurley
Feb 13, 2026
Activity 06 - Variables
*/
console.log("---- I am in V A R I A B L E S ----")

// Q1 : Is it permitted the next ?
console.log("Q1 ---------------")
var var1 = "Iowa";
console.log(var1);
var var1 = 124;
console.log(var1);
// Is it permitted ?
console.log("Yes");

// Q2 : Is it valid ?
console.log("Q2 ----------------");
let var2 = "Ames";
console.log(var2);
let var2 = 124;
// Is it valid ?
console.log("No");

// Q3 : Is it valid ?
console.log("Q3 ----------------");
let var3 = "ISU";
console.log(var3);
var3 = 2023;
console.log(var3);
console.log("It is Valid")

// Q4 : Explain the Error.
console.log("Q4 ----------------");
let var4;
const var5;
// Uncaught SyntaxError: missing = in const declaration debugger eval code:2:7
console.log("What's the error : const needs a declaration")

// Q5 : Explain the Error.
console.log("Q5 ----------------");
const var6 = 3.1415;
var6 = 2.8;
// Uncaught TypeError: invalid assignment to const 'var6'
console.log("What's the error : cannot change a const variable")

// Q6: Explain the Error.
let first name = “Sarfaraz";
console.log("there is a space between first and name.");
let 2numbers = [1,2];
console.log("Cannot have a number at the start of a variable name.");
let city-state = "Ames Iowa";
console.log("Cannot have a dash in a variable name");

// Q7 : What !! ??
let MainCity = "DesMoines";
console.log("This is the Capital :", MainCity)
// no error?
console.log("It prints the string right after the main quotations")
 
// Q8: "let" and "const" scope vs "var" scope
if (5 === 5) {
    var var7 = 100;
}
console.log(var7);

if(5 === 5) {
    let var8 = 100;
}
// Uncaught ReferenceError: var8 is not defined
console.log(var8);

console.log("var variables can live outisde of an if statement, but let only allows the variable to live in the if statement then is removed.")