import React from "react";
import Posts from "./features/posts/Posts";
import "./App.css";

const App: React.FC = () => {
  return (
    <div className="app-container">
      <Posts />
    </div>
  );
};

export default App;
