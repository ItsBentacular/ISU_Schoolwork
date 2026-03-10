function sum(a,b) {
    setTimeout(() => {
        console.log("Sum: ", a + b);
    }, 5000);
    console.log(1);
}

function sump(a,b) {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve(console.log("Promise Sum: ", a + b));
        }, 3000);
    });
}

function mult(a,b) {
    setTimeout(() => {
        console.log("Mult: ", a * b);
    }, 2000);
    console.log(2);
}



function runAsync() {
    let s  = sum(8,2);
    console.log(s);

    let m = mult(12,2);
    console.log(m);
}

console.log("before");
runAsync();
console.log("after")