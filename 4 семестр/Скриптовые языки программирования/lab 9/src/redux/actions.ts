import type { Todo } from "./types";

export const addTodo = (todo: Todo) => ({
  type: "ADD_TODO" as const,
  payload: todo,
});

export const deleteTodo = (id: number) => ({
  type: "DELETE_TODO" as const,
  payload: id,
});

export const toggleTodo = (id: number) => ({
  type: "TOGGLE_TODO" as const,
  payload: id,
});

export const setEditing = (id: number) => ({
  type: "SET_EDITING" as const,
  payload: id,
});

export const editTodo = (id: number, title: string, description: string) => ({
  type: "EDIT_TODO" as const,
  payload: { id, title, description },
});
