import React, { useState } from "react";
import { useDispatch } from "react-redux";
import type { Todo } from "../redux/types";
import styles from "./TodoForm.module.css";

const TodoForm: React.FC = () => {
  const dispatch = useDispatch();
  const [title, setTitle] = useState("");
  const [description, setDescription] = useState("");

  const addTodo = () => {
    if (!title.trim()) return;
    const newTodo: Todo = {
      id: Date.now(),
      title,
      description,
      completed: false,
    };
    dispatch({ type: "ADD_TODO", payload: newTodo });
    setTitle("");
    setDescription("");
  };

  return (
    <div className={styles.formWrapper}>
      <div className={styles.form}>
        <input
          className={styles.input}
          placeholder="название"
          value={title}
          onChange={(e) => setTitle(e.target.value)}
        />
        <input
          className={styles.input}
          placeholder="описание"
          value={description}
          onChange={(e) => setDescription(e.target.value)}
        />
        <button className={styles.button} onClick={addTodo}>
          добавить задачу
        </button>
      </div>
    </div>
  );
};

export default TodoForm;
