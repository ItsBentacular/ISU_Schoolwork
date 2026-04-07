import React, { useState } from "react";
import Sidebar from "./components/Sidebar";
import ProductsComponent from "./components/Products";
import { Products } from "./data/fashion";
import { Categories } from "./data/categories";

const App = () => {
  const [active, setActive] = useState("all");
  const [query, setQuery] = useState("");

  const filteredProducts = Products.filter((p) => {
    const matchesCategory = active === "all" || p.category === active;
    const matchesSearch = p.title.toLowerCase().includes(query.toLowerCase());
    
    return matchesCategory && matchesSearch;
  });

  return (
    <div className="flex min-h-screen">
      <Sidebar 
        categories={Categories} 
        active={active} 
        setActive={setActive}  
        query={query} 
        setQuery={setQuery} 
      />
      
      <ProductsComponent products={filteredProducts} />
    </div>
  );
};

export default App;