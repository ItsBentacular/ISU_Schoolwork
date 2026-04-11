import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import MyApp from "./MyRoutingApplication.jsx";

createRoot(document.getElementById("root")).render(
  <StrictMode>
    <MyApp />
  </StrictMode>,
);
