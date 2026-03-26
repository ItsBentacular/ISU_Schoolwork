import asset1 from "./assets/react.svg"

function Header() {
    return(
        <>
        <header>
            <h1 style={{color: "blue", 
                border:"2px solid red", 
                backgroundColor:"tan", 
                padding:"25px"}}>
                    My First React App
            </h1>
                
            <nav>
                <ul style={{display: "flex", gap: "20px", listStyle:"none"}}>
                    <li>
                    <a href="">Home</a>
                    </li>    
                    <li>
                    <a href="">About</a>
                    </li>    
                    <li>
                    <a href="">Contact</a>
                    </li>    
                    <li>
                    <a href="">Quiz</a>
                    </li>    
                </ul>     
            </nav>
            <h2>
                <img src={asset1} alt="react logo" />
            </h2>
        </header>
        </>
    );
}

export default Header;