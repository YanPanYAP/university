const express = require('express');
const fs = require('fs');
const path = require('path');
const { engine } = require('express-handlebars');

const app = express();
const PORT = process.env.PORT || 3000;
const DB = path.join(__dirname, 'phonebook.json');

app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

app.engine('handlebars', engine({
    defaultLayout: 'main',
    helpers: {
        cancel: function () {
            return '<a href="/">Отказаться</a>';
        }
    }
}));

app.set('view engine', 'handlebars');

function readDB() {
    return JSON.parse(fs.readFileSync(DB));
}

function writeDB(data) {
    fs.writeFileSync(DB, JSON.stringify(data, null, 2));
}

app.get('/', (req, res) => {
    const data = readDB();
    res.render('index', { entries: data.entries });
});

app.get('/Add', (req, res) => {
    const data = readDB();
    res.render('add', { entries: data.entries });
});

app.post('/Add', (req, res) => {
    const { name, phone } = req.body;

    const nameRegex = /^[A-Za-zА-Яа-яЁё-]+$/;
    const phoneRegex = /^(\+375|80)(29|44|25)\d{7}$/;

    if (!nameRegex.test(name)) {
        const data = readDB();
        return res.render('add', {
            entries: data.entries,
            error: 'Имя может содержать только буквы и дефис',
            old: { name, phone }
        });
    }

    if (!phoneRegex.test(phone)) {
        const data = readDB();
        return res.render('add', {
            entries: data.entries,
            error: 'Телефон должен быть формата +375(29|44|25)XXXXXXX или 80(29|44|25)XXXXXXX',
            old: { name, phone }
        });
    }

    const data = readDB();
    const newId = data.entries.length ? data.entries[data.entries.length - 1].id + 1 : 1;

    data.entries.push({ id: newId, name, phone });

    writeDB(data);
    res.redirect('/');
});

app.get('/Update', (req, res) => {
    const id = parseInt(req.query.id);
    const data = readDB();
    const selected = data.entries.find(e => e.id === id);

    res.render('update', {
        entries: data.entries,
        selected
    });
});

app.post('/Update', (req, res) => {
    const { id, name, phone } = req.body;

    const nameRegex = /^[A-Za-zА-Яа-яЁё-]+$/;
    const phoneRegex = /^(\+375|80)(29|44|25)\d{7}$/;

    const data = readDB();
    const selected = data.entries.find(e => e.id == id);

    if (!nameRegex.test(name)) {
        return res.render('update', {
            entries: data.entries,
            selected,
            error: 'Имя может содержать только буквы и дефис'
        });
    }

    if (!phoneRegex.test(phone)) {
        return res.render('update', {
            entries: data.entries,
            selected,
            error: 'Телефон должен быть формата +375(29|44|25)XXXXXXX или 80(29|44|25)XXXXXXX'
        });
    }

    selected.name = name;
    selected.phone = phone;

    writeDB(data);
    res.redirect('/');
});

app.post('/Delete', (req, res) => {
    const id = parseInt(req.body.id);
    const data = readDB();

    data.entries = data.entries.filter(e => e.id !== id);

    writeDB(data);
    res.redirect('/');
});

app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});