import React, { useState, createContext, useContext, useEffect } from "react";
import {
  BrowserRouter as Router,
  Routes,
  Route,
  Link,
  useNavigate,
  Navigate,
  Outlet,
} from "react-router-dom";
import "./index.css";

const UserContext = createContext();

const UserProvider = ({ children }) => {
  const [user, setUser] = useState(null);

  useEffect(() => {
    const storedUser = localStorage.getItem("user");
    if (storedUser) {
      setUser(JSON.parse(storedUser));
    }
  }, []);

  const login = (userData) => {
    setUser(userData);
    localStorage.setItem("user", JSON.stringify(userData));
  };

  const logout = () => {
    setUser(null);
    localStorage.removeItem("user");
  };

  return (
    <UserContext.Provider value={{ user, login, logout }}>
      {children}
    </UserContext.Provider>
  );
};

const useUser = () => useContext(UserContext);

const ProtectedRoute = ({ children }) => {
  const { user } = useUser();
  return user ? children : <Navigate to="/sign-in" replace />;
};

const InputField = ({
  label,
  type = "text",
  value,
  onChange,
  error,
  showToggle,
  togglePassword,
}) => (
  <div className="input-group">
    <input
      type={type}
      placeholder={label}
      value={value}
      onChange={onChange}
      className={error ? "error" : ""}
    />
    {error && <div className="error-msg">{error}</div>}
    {showToggle && (
      <button className="toggle-btn" onClick={togglePassword} type="button">
        {type === "password" ? "👁" : "🙈"}
      </button>
    )}
  </div>
);

const validateEmail = (email) => {
  const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  if (!email) return "Email обязателен";
  if (/\s/.test(email)) return "Email не должен содержать пробелы";
  if (!emailRegex.test(email)) return "Некорректный email";
  return "";
};

const validatePassword = (password) => {
  if (!password) return "Пароль обязателен";
  if (password.length < 8) return "Минимум 8 символов";
  if (!/[A-Z]/.test(password)) return "Требуется заглавная буква";
  if (!/[a-z]/.test(password)) return "Требуется строчная буква";
  if (!/\d/.test(password)) return "Требуется цифра";
  if (/\s/.test(password)) return "Пароль не должен содержать пробелы";
  return "";
};

const validateName = (name) => {
  if (!name) return "Имя обязательно";
  if (!/^[A-Za-zА-Яа-яЁё\s]+$/.test(name)) return "Только буквы и пробелы";
  if (name.length < 2) return "Минимум 2 символа";
  if (name.length > 50) return "Максимум 50 символов";
  return "";
};

const AuthWrapper = ({ title, children }) => (
  <div className="auth-container">
    <h2>{title}</h2>
    {children}
  </div>
);

function SocialReg() {
  return (
    <div className="social-buttons">
      <button className="social-btn social-fb">f</button>
      <button className="social-btn social-vk">вк</button>
      <button className="social-btn social-google">G</button>
    </div>
  );
}

const Dashboard = () => {
  const { user, logout } = useUser();
  return (
    <div className="auth-container">
      <h2>Добро пожаловать, {user?.name}!</h2>
      <p>Email: {user?.email}</p>
      <button onClick={logout}>Выйти</button>
    </div>
  );
};

const SignUp = () => {
  const [name, setName] = useState("");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [confirm, setConfirm] = useState("");
  const [showPassword, setShowPassword] = useState(false);
  const [errors, setErrors] = useState({});
  const { login } = useUser();
  const navigate = useNavigate();

  const handleRegister = () => {
    const newErrors = {
      name: validateName(name),
      email: validateEmail(email),
      password: validatePassword(password),
      confirm: confirm !== password ? "Пароли не совпадают" : "",
    };

    setErrors(newErrors);

    if (Object.values(newErrors).every((e) => !e)) {
      const newUser = {
        id: Date.now().toString(),
        name,
        email,
        password,
      };

      const storedUsers = JSON.parse(localStorage.getItem("users") || "[]");

      if (storedUsers.some((u) => u.email === email)) {
        setErrors({ email: "Email уже зарегистрирован" });
        return;
      }

      const updatedUsers = [...storedUsers, newUser];
      localStorage.setItem("users", JSON.stringify(updatedUsers));

      login(newUser);
      navigate("/dashboard");
    }
  };

  return (
    <AuthWrapper title="Регистрация">
      <SocialReg />
      <InputField
        label="Имя"
        value={name}
        onChange={(e) => setName(e.target.value)}
        error={errors.name}
      />
      <InputField
        label="Email"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
        error={errors.email}
      />
      <InputField
        label="Пароль"
        type={showPassword ? "text" : "password"}
        value={password}
        onChange={(e) => setPassword(e.target.value)}
        error={errors.password}
        showToggle
        togglePassword={() => setShowPassword(!showPassword)}
      />
      <InputField
        label="Подтверждение пароля"
        type={showPassword ? "text" : "password"}
        value={confirm}
        onChange={(e) => setConfirm(e.target.value)}
        error={errors.confirm}
      />
      <button onClick={handleRegister}>Зарегистрироваться</button>
      <p>
        <Link to="/sign-in">Войти</Link>
      </p>
    </AuthWrapper>
  );
};

const SignIn = () => {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [showPassword, setShowPassword] = useState(false);
  const [error, setError] = useState("");
  const { login } = useUser();
  const navigate = useNavigate();

  const handleLogin = () => {
    const storedUsers = JSON.parse(localStorage.getItem("users") || "[]");
    const foundUser = storedUsers.find(
      (u) => u.email === email && u.password === password
    );

    if (foundUser) {
      login(foundUser);
      navigate("/dashboard");
    } else {
      setError("Неверный email или пароль");
    }
  };

  return (
    <AuthWrapper title="Вход">
      <SocialReg />
      <InputField
        label="Email"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
        error={error}
      />
      <InputField
        label="Пароль"
        type={showPassword ? "text" : "password"}
        value={password}
        onChange={(e) => setPassword(e.target.value)}
        error={error}
        showToggle
        togglePassword={() => setShowPassword(!showPassword)}
      />
      <button onClick={handleLogin}>Войти</button>
      <p>
        <Link to="/sign-up">Зарегистрироваться</Link>
      </p>
      <p>
        <Link to="/reset-password">Забыли пароль?</Link>
      </p>
    </AuthWrapper>
  );
};

const ResetPassword = () => {
  const [email, setEmail] = useState("");
  const [success, setSuccess] = useState(false);
  const [error, setError] = useState("");

  const handleReset = () => {
    const emailErr = validateEmail(email);
    if (emailErr) {
      setError(emailErr);
      setSuccess(false);
    } else {
      const storedUsers = JSON.parse(localStorage.getItem("users") || "[]");
      const userIndex = storedUsers.findIndex(
        (u) => u.email.toLowerCase() === email.trim().toLowerCase()
      );

      if (userIndex !== -1) {
        storedUsers[userIndex].password = "NewPass123";
        localStorage.setItem("users", JSON.stringify(storedUsers));

        const currentUser = JSON.parse(localStorage.getItem("user"));
        if (
          currentUser &&
          currentUser.email.toLowerCase() === email.trim().toLowerCase()
        ) {
          currentUser.password = "NewPass123";
          localStorage.setItem("user", JSON.stringify(currentUser));
        }

        setSuccess(true);
        setError("");
      } else {
        setError("Пользователь с таким email не найден");
        setSuccess(false);
      }
    }
  };

  return (
    <AuthWrapper title="Восстановление пароля">
      <InputField
        label="Email"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
        error={error}
      />
      <button onClick={handleReset}>Сбросить пароль</button>
      {success && <p className="success">Новый пароль: NewPass123</p>}
      <p>
        <Link to="/sign-in">Назад ко входу</Link>
      </p>
    </AuthWrapper>
  );
};

const App = () => (
  <UserProvider>
    <Router>
      <Routes>
        <Route path="/" element={<Navigate to="/sign-in" />} />
        <Route path="/sign-up" element={<SignUp />} />
        <Route path="/sign-in/*" element={<SignIn />}>
          <Route path="sign-up" element={<SignUp />} /> {}
        </Route>
        <Route path="/reset-password" element={<ResetPassword />} />
        <Route
          path="/dashboard"
          element={
            <ProtectedRoute>
              <Dashboard />
            </ProtectedRoute>
          }
        />
      </Routes>
    </Router>
  </UserProvider>
);

export default App;
