//Q1
function maxOfTwo(n1, n2) {
    if( n1 > n2) {
        return n1;
    }
    return n2;
}

let n1 = 21;
let n2 = 80;
console.log(`The max between ${n1} and ${n2} is :`, maxOfTwo(n1,n2));

//Q2
function maxOfArray(array) {
    let max = array[0];
    for(let val in array) {
        if(array[val] > max) {
            max = array[val];
        }
    }
    return max;
} 

let array = [10,11,1024,125,9,201];
console.log(maxOfArray(array));

//Q3
function showProperties(object) {
    console.log('list of keys:')
    for(let key in object) {
    console.log(key);
    }
    console.log('list of properties:')
    for (let key in object) {
        console.log(object[key]);
    }
}

const movie = {
title : 'Some movie',
releaseYear: 2018,
rating: 4.5,
director: 'Steven Spielberg'
};
showProperties(movie);

//Q4
let circle = {
    radius: 2,
    area : function() {
        return this.radius * this.radius * Math.PI;
    }
};

console.log(circle.area());

//Q5
let circle2 = {
    radius: 2,
    area : function() {
        return this.radius * this.radius * Math.PI;
    },
    get radiusValue() {
        return `${this.radius}`;
    },
    set radiusValue(radius) {
        this.radius = radius;
    }
}

console.log(`Area with ${circle2.radiusValue} :`,circle2.area());
circle2.radiusValue = 3;
console.log(`Area with ${circle2.radiusValue} :`,circle2.area());

//Q6
let circle3 = {
    radius: 2,
    area : function() {
        return this.radius * this.radius * Math.PI;
    },
    getRadiusValue : function() {
        return `${this.radius}`;
    },
    setRadiusValue : function(radius) {
        this.radius = radius;
    }
}

console.log(`Area with ${circle3.getRadiusValue()} :`,circle3.area());
circle3.setRadiusValue(3);
console.log(`Area with ${circle3.getRadiusValue()} :`,circle3.area());
