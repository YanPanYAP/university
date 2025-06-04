import React from "react";
import { useSelector } from "react-redux";
import type { RootState } from "./redux/store";
import TodoForm from "./components/TodoForm";
import TodoItem from "./components/TodoItem";
import styles from "./App.module.css";

const App: React.FC = () => {
  const todos = useSelector((state: RootState) => state.todos.todos);

  return (
    <div className={styles.container}>
      <h1 className={styles.title}>ToDoList</h1>
      <TodoForm />
      {todos.map((todo) => (
        <TodoItem key={todo.id} todo={todo} />
      ))}
    </div>
  );
};

export default App;
