const express = require('express');
const passport = require('passport');
const { BasicStrategy } = require('passport-http');
const session = require('express-session');
const path = require('path');
const users = require('./users.json');

const app = express();
const PORT = 3000;


passport.use(new BasicStrategy((username, password, done) => {
  const user = users.find(u => u.username === username && u.password === password);
  if (user) return done(null, user);
  return done(null, false);
}));

passport.serializeUser((user, done) => done(null, user.username));

passport.deserializeUser((username, done) => {
  const user = users.find(u => u.username === username);
  done(null, user);
});


app.use(session({ secret: 'secret', resave: false, saveUninitialized: false }));
app.use(passport.initialize());
app.use(passport.session());
app.use(express.static(path.join(__dirname, 'public')));


function ensureAuthenticated(req, res, next) {
  if (req.isAuthenticated()) return next();
  passport.authenticate('basic', { session: true })(req, res, next);
}


app.get('/resource', ensureAuthenticated, (req, res) => {
  res.send(`<p>Ресурс</p><a href="/">Back to interface</a>`);
});


app.get('/logout', (req, res, next) => {
  req.logout(err => {
    if (err) return next(err);
    req.session.destroy(() => {
      res.send(`<p>Выход выполнен (необходимо закрыть браузер)</p><a href="/">Back to interface</a>`);
    });
  });
});


app.use((req, res) => {
  res.status(404).send('404 Not Found');
});


app.listen(PORT, () => {
  console.log(`21-01 server running at http://localhost:${PORT}`);
});