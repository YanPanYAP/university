export interface Todo {
  id: number;
  title: string;
  description: string;
  completed: boolean;
  isEditing?: boolean;
}

export interface TodoState {
  todos: Todo[];
}

export interface AddTodoAction {
  type: "ADD_TODO";
  payload: Todo;
}

export interface DeleteTodoAction {
  type: "DELETE_TODO";
  payload: number;
}

export interface ToggleTodoAction {
  type: "TOGGLE_TODO";
  payload: number;
}

export interface SetEditingTodoAction {
  type: "SET_EDITING";
  payload: number;
}

export interface EditTodoAction {
  type: "EDIT_TODO";
  payload: {
    id: number;
    title: string;
    description: string;
  };
}

export type TodoAction =
  | AddTodoAction
  | DeleteTodoAction
  | ToggleTodoAction
  | SetEditingTodoAction
  | EditTodoAction;
