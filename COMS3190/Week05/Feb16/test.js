let test = {
    name: "Tom",
    age: 41,
};

console.log(test);

// same thing, different method
console.log(test.name);
console.log(test["name"])


// can also change values using these notations
test["name"] = "Bob"
console.log(test.name);

test.name = "Jake"
console.log(test.name)

// can also change inner object declaration
let selection = "name";
test.selection = "Gary";
console.log(test.selection);

test['likesBeer'] = true;
delete test[age];