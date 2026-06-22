const express = require('express');
const session = require('express-session');
const path = require('path');
const bodyParser = require('body-parser');
const users = require('./users.json');

const app = express();
const PORT = 3000;


app.use(session({ secret: 'secret', resave: false, saveUninitialized: false }));
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));



function formsAuth(req, res, next) {
  if (req.session.user) return next();
  res.redirect('/login');
}



app.get('/login', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'login.html'));
});


app.post('/login', (req, res) => {
  const { username, password } = req.body;
  const user = users.find(u => u.username === username && u.password === password);
  if (user) {
    req.session.user = user;
    res.send(`<p>Выход выполнен!</p><a href="/">Back to interface</a>`);
  } else {
    res.send(`<p>Некорректное имя или паоль</p><a href="/login">Войти снова</a>`);
  }
});


app.get('/logout', (req, res) => {
  req.session.destroy(() => {
    res.send(`<p>Logged out successfully!</p><a href="/">Назад</a>`);
  });
});


app.get('/resource', formsAuth, (req, res) => {
  res.send(`<p>Ресурс</p><a href="/">Назад</a>`);
});


app.use((req, res) => {
  res.status(404).send('404 Not Found');
});


app.listen(PORT, () => {
  console.log(`21-03 server running at http://localhost:${PORT}`);
});