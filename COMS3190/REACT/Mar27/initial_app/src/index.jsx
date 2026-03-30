import React from  "react";
import ReactDOM from "react-dom/client";

const rootElement = document.getElementById("root");
const root = ReactDOM.createRoot(rootElement);


const myElement1 = (<h1>I love JSX!</h1>)

const myElement2 = (
    <ul>
        <li>Apple</li>
        <li>Pear</li>
        <li>Peach</li>
    </ul>
);

function MyElement3() {
    const name = "Jake";


    return <h3>my name is {name}, and my friend's name is {name}</h3>
}
// root.render needs a html div <> to render multiple components at once
root.render(
    <>
    {myElement1}
    {myElement2}
    {MyElement3()}
    </>,
);