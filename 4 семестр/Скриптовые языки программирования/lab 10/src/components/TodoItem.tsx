import React, { useState } from "react";
import { useDispatch } from "react-redux";
import type { Todo } from "../redux/types";
import {
  deleteTodo,
  editTodo,
  setEditing,
  toggleTodo,
} from "../redux/todoSlice";
import styles from "./TodoItem.module.css";

interface Props {
  todo: Todo;
}

const TodoItem: React.FC<Props> = ({ todo }) => {
  const dispatch = useDispatch();
  const [editTitle, setEditTitle] = useState(todo.title);
  const [editDescription, setEditDescription] = useState(todo.description);
  const isEditing = todo.isEditing;

  const handleEditToggle = () => {
    if (isEditing) {
      dispatch(
        editTodo({
          id: todo.id,
          title: editTitle,
          description: editDescription,
        })
      );
    } else {
      setEditTitle(todo.title);
      setEditDescription(todo.description);
      dispatch(setEditing(todo.id));
    }
  };

  return (
    <div
      className={`${styles.item} ${todo.completed ? styles.completed : ""} ${
        isEditing ? styles.editing : ""
      }`}
    >
      <div className={styles.content}>
        {isEditing ? (
          <>
            <input
              className={styles.input}
              value={editTitle}
              onChange={(e) => setEditTitle(e.target.value)}
            />
            <textarea
              className={styles.input}
              value={editDescription}
              onChange={(e) => setEditDescription(e.target.value)}
              rows={3}
            />
          </>
        ) : (
          <>
            <div
              className={`${styles.title} ${
                todo.completed ? styles.strike : ""
              }`}
            >
              {todo.title}
            </div>
            <div
              className={`${styles.fullDescription} ${
                todo.completed ? styles.strike : ""
              }`}
            >
              {todo.description}
            </div>
          </>
        )}
      </div>
      <div className={styles.actions}>
        <button
          className={`${styles.circleButton} ${styles.edit}`}
          onClick={handleEditToggle}
        >
          {isEditing ? "✓" : "E"}
        </button>
        <button
          className={`${styles.circleButton} ${styles.delete}`}
          onClick={() => dispatch(deleteTodo(todo.id))}
        >
          D
        </button>
        <button
          className={`${styles.circleButton} ${
            todo.completed ? styles.toggleDone : styles.toggle
          }`}
          onClick={() => dispatch(toggleTodo(todo.id))}
        ></button>
      </div>
    </div>
  );
};

export default TodoItem;
