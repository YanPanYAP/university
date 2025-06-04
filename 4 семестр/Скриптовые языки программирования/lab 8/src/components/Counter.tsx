import { increment, decrement, reset } from "../redux/actions";
import { useAppDispatch, useAppSelector } from "../redux/hooks";
import Button from "./Button";
import styles from "./Counter.module.css";

const Counter = () => {
  const dispatch = useAppDispatch();
  const value = useAppSelector((state) => state.value);

  return (
    <div className={styles.counter}>
      <h1 className={styles.title}>Counter: {value}</h1>
      <div className={styles.buttons}>
        <Button label="+" onClick={() => dispatch(increment())} />
        <Button label="–" onClick={() => dispatch(decrement())} />
        <Button label="Reset" onClick={() => dispatch(reset())} />
      </div>
    </div>
  );
};

export default Counter;
