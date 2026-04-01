import React, {useState, useEffect} from 'react';

function Counter() {
    const [count, setCount] = useState(0);
    const [message, setMessage] = useState(0);
    
    return(
    <div>
    <h1>Counter: {count}</h1>
    <button 
    onClick={() => {
        setCount((count) => count + 1);
        console.log(count);
    }}
    >
    Add
    </button>
    <button 
    onClick={() => {
        setCount((count) => count - 1);
        console.log(count);
    }}
    >
    Sub
    </button>
    <button 
    onClick={() => {
        setCount((count) => count * 2);
        console.log(count);
    }}
    >
    Mult
    </button>
    <h3>{message}</h3>
    <input onChange={(e) => setMessage(e.target.value)} />
    <button onClick={(e) => alert(message)}>Show</button>
    </div>
    );
}
export default Counter;