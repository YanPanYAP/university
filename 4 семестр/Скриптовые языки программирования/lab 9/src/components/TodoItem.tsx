import React, { useEffect, useState } from "react";
import { useDispatch } from "react-redux";
import type { Todo } from "../redux/types";
import styles from "./TodoItem.module.css";

interface Props {
  todo: Todo;
}

const TodoItem: React.FC<Props> = ({ todo }) => {
  const dispatch = useDispatch();
  const [editTitle, setEditTitle] = useState(todo.title);
  const [editDescription, setEditDescription] = useState(todo.description);
  const [isEditingLocal, setIsEditingLocal] = useState(todo.isEditing);

  useEffect(() => {
    setIsEditingLocal(todo.isEditing);
  }, [todo.isEditing]);

  let itemClass = styles.item;
  if (todo.completed) itemClass += ` ${styles.completed}`;
  else if (isEditingLocal) itemClass += ` ${styles.editing}`;

  const handleEditToggle = () => {
    if (isEditingLocal) {
      dispatch({
        type: "EDIT_TODO",
        payload: {
          id: todo.id,
          title: editTitle,
          description: editDescription,
        },
      });
    } else {
      setEditTitle(todo.title);
      setEditDescription(todo.description);
    }

    dispatch({ type: "SET_EDITING", payload: todo.id });
    setIsEditingLocal(!isEditingLocal);
  };

  return (
    <div className={itemClass}>
      <div className={styles.content}>
        {isEditingLocal ? (
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
          {isEditingLocal ? "✓" : "E"}
        </button>
        <button
          className={`${styles.circleButton} ${styles.delete}`}
          onClick={() => dispatch({ type: "DELETE_TODO", payload: todo.id })}
        >
          D
        </button>
        <button
          className={`${styles.circleButton} ${
            todo.completed ? styles.toggleDone : styles.toggle
          }`}
          onClick={() => dispatch({ type: "TOGGLE_TODO", payload: todo.id })}
        ></button>
      </div>
    </div>
  );
};

export default TodoItem;
