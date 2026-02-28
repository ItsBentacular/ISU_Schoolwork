function getInputValue() {
    let movieName = document.forms["my_form"]["inputMovieName"];
    let inputMovieName = movieName.value;

    fetch("./MoviesFromJSON.json")
        .then(response => response.json())
        .then(myMovies => loadMovies(myMovies, inputMovieName))
        .catch(err => console.log("error:" + err));
}

function loadMovies(myMovies, inputMovieName) {
    var mainContainer = document.getElementById("goodmovies");
    
    mainContainer.innerHTML = ""; 

    for (var i = 0; i < myMovies.movies.length; i++) {
        if (myMovies.movies[i].title === inputMovieName) {
            let div = document.createElement("div");
            
            div.innerHTML = `
                <h3>${myMovies.movies[i].title}</h3> <br>
                <p>Year: ${myMovies.movies[i].year}</p> <br><br>
                <img src="${myMovies.movies[i].url}" alt="${myMovies.movies[i].title} poster" width="200">
            `;
            
            mainContainer.appendChild(div);
        }
    }
}