import { legacy_createStore as createStore } from "redux";
import { counterReducer } from "./reducer";
import { CounterState, CounterActionTypes } from "./types";
import { Reducer } from "redux";

const reducer: Reducer<CounterState, CounterActionTypes> = counterReducer;
export const store = createStore(reducer);
export type RootState = ReturnType<typeof store.getState>;
export type AppDispatch = typeof store.dispatch;
