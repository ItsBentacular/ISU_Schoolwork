import React, { useState } from "react";
import ProductsComponent from "./components/Products";
import { Products } from "./data/fashion";
import { Categories } from "./data/categories";
import Sidebar from "./components/Sidebar";

const App = () => {
  const [active, setActive] = useState("all");
  const filteredProducts =
    active === "all" ? Products : Products.filter((p) => p.category === active);
  return (
    <div className="flex min-h-screen">
      {/* Sidebar */}
      <Sidebar categories={Categories} active={active} setActive={setActive} />
      {/* Products Section */}
      <ProductsComponent products={filteredProducts} />
    </div>
  );
};
export default App;