import React, { useState } from "react";
import { useDispatch } from "react-redux";
import { addTodo } from "../redux/todoSlice";
import type { Todo } from "../redux/types";
import styles from "./TodoForm.module.css";

const TodoForm: React.FC = () => {
  const dispatch = useDispatch();
  const [title, setTitle] = useState("");
  const [description, setDescription] = useState("");

  const handleAdd = () => {
    if (!title.trim()) return;
    const newTodo: Todo = {
      id: Date.now(),
      title,
      description,
      completed: false,
    };
    dispatch(addTodo(newTodo));
    setTitle("");
    setDescription("");
  };

  return (
    <div className={styles.formWrapper}>
      <div className={styles.form}>
        <input
          className={styles.input}
          placeholder="Название"
          value={title}
          onChange={(e) => setTitle(e.target.value)}
        />
        <textarea
          className={styles.input}
          placeholder="Описание"
          value={description}
          onChange={(e) => setDescription(e.target.value)}
          rows={3}
        />
        <button className={styles.button} onClick={handleAdd}>
          Добавить задачу
        </button>
      </div>
    </div>
  );
};

export default TodoForm;
