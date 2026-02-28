fetch('./person1.json')
.then(function (response) {
return response.json();
})
.then(function (data) {
appendData(data);
})
.catch(function (err) {
console.log('error:' + err);
})


function appendData(data) {
    let mainContainer = document.getElementById("myData1");
    let div = document.createElement("div");
    div.innerHTML = `<br>
    <h1> Superhero : </h1>
    Firstname : ${data.firstName} <br>
    Lastname : ${data.lastName} <br>
    Job : ${data.job} <br>
    Roll : ${data.roll}`;
    mainContainer.appendChild(div);
} // end of function appendData
