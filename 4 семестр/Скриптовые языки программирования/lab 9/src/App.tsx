import React from "react";
import { useSelector } from "react-redux";
import type { TodoState } from "./redux/types";
import TodoItem from "./components/TodoItem";
import TodoForm from "./components/TodoForm";
import styles from "./App.module.css";

const App: React.FC = () => {
  const todos = useSelector((state: TodoState) => state.todos);

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
