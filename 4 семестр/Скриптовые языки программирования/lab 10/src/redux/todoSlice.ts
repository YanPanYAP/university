import { createSlice, type PayloadAction } from "@reduxjs/toolkit";
import type { Todo } from "./types";

interface TodoState {
  todos: Todo[];
}

const initialState: TodoState = {
  todos: [],
};

const todoSlice = createSlice({
  name: "todos",
  initialState,
  reducers: {
    addTodo: (state, action: PayloadAction<Todo>) => {
      state.todos.push(action.payload);
    },
    deleteTodo: (state, action: PayloadAction<number>) => {
      state.todos = state.todos.filter((todo) => todo.id !== action.payload);
    },
    toggleTodo: (state, action: PayloadAction<number>) => {
      const todo = state.todos.find((t) => t.id === action.payload);
      if (todo) todo.completed = !todo.completed;
    },
    setEditing: (state, action: PayloadAction<number>) => {
      state.todos = state.todos.map((todo) =>
        todo.id === action.payload
          ? { ...todo, isEditing: true }
          : { ...todo, isEditing: false }
      );
    },
    editTodo: (
      state,
      action: PayloadAction<{
        id: number;
        title: string;
        description: string;
      }>
    ) => {
      const { id, title, description } = action.payload;
      const todo = state.todos.find((t) => t.id === id);
      if (todo) {
        todo.title = title;
        todo.description = description;
        todo.isEditing = false;
      }
    },
  },
});

export const { addTodo, deleteTodo, toggleTodo, setEditing, editTodo } =
  todoSlice.actions;

export default todoSlice.reducer;
