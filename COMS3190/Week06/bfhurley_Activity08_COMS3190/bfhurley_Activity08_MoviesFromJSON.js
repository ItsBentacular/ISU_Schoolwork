fetch("./MoviesFromJSON.json")
  .then(response => response.json())
  .then(myMovies => loadMovies(myMovies))
  .catch(err => console.log("error:" + err));

function loadMovies(myMovies) {
  var mainContainer = document.getElementById("goodmovies");
  
  console.log("Successfully loaded movie data:", myMovies);

  for (let movie of myMovies.movies) {
    console.log("Adding movie: " + movie.title);
    
    let div = document.createElement("div");
    
    div.innerHTML = `
      <h3>${movie.title}</h3>
      <p>Year: ${movie.year} </p>
      <img src="${movie.url}" alt="${movie.title} poster" width="200">
      <hr>
    `;
    
    mainContainer.appendChild(div);
  }
}