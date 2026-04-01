import { useState } from 'react'
import './App.css'
import Sidebar from './components/Sidebar'

const App = () => {
  const [active,setActive] = useState("all");
  return (
    <div className="flex min-h-screen">
      <Sidebar
        categories={categories}
        active={active}
        setActive={setActive}
      />
    </div>
  );
};

export default App
