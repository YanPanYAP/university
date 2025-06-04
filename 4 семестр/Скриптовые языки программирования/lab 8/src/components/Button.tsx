import styles from "./Button.module.css";

interface ButtonProps {
  onClick: () => void;
  label: string;
}

const Button = ({ onClick, label }: ButtonProps) => (
  <button className={styles.button} onClick={onClick}>
    {label}
  </button>
);

export default Button;
