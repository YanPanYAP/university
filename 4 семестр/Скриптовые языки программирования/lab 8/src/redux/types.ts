export const INCREMENT = "INCREMENT";
export const DECREMENT = "DECREMENT";
export const RESET = "RESET";

export interface CounterState {
  value: number;
}

interface IncrementAction {
  type: typeof INCREMENT;
}

interface DecrementAction {
  type: typeof DECREMENT;
}

interface ResetAction {
  type: typeof RESET;
}

export type CounterActionTypes =
  | IncrementAction
  | DecrementAction
  | ResetAction;
