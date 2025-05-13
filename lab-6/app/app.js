const { Pool } = require("pg");
const express = require('express');

const pool = new Pool({
    host: 'localhost',
    database: 'testdb',
    user: 'postgres',
    password: 'postgres',
    port: 5432
});

const app = express();
const PORT = 80;

app.set('view engine', 'ejs');
app.use(express.urlencoded({extended: true}));
app.use(express.json());

// Главная страница
app.get("/", (req, res) => {
    pool.query("SELECT * FROM users WHERE loginu = $1 and pass = $2", 
    [req.query.login || '', req.query.password || ''], (err, user_data) => {
        
        pool.query("UPDATE users SET datelastconnect = $1 WHERE idu = $2", 
        [new Date().toISOString().slice(0, 19).replace('T', ' '), user_data.rows[0]?.idu || 0], () => {});
        
        pool.query("SELECT bots.idu AS botid, bots.ownerid, bots.nameu, bots.descriptionu, users.loginu AS ownerlogin, bots.dateofregistration, bots.statusu FROM bots LEFT JOIN users ON users.idu = bots.ownerid", 
        [], (err, bots) => {
            pool.query("SELECT * FROM users", [], (err, users) => {
                res.render('lists', {
                    users: users.rows,
                    bots: bots.rows
                });
            });
        });
    });
});

// Регистрация
app.get("/register", (req, res) => {
    res.render("register");
});

app.post("/register", (req, res) => {
    const current_date = new Date().toISOString().slice(0, 19).replace('T', ' ');
    pool.query(
        "INSERT INTO users (loginu, pass, lastname, nameu, patronymic, dateofregistration, datelastconnect, statusu) VALUES ($1, $2, $3, $4, $5, $6, $7, $8)", 
        [req.body.loginu, req.body.passwordu, req.body.nameu, req.body.lastname, req.body.patronymic, current_date, current_date, 'user'], 
        () => res.redirect("/")
    );
});

// Удаление пользователя
app.delete("/deleteuser", (req, res) => {
    pool.query("DELETE FROM users WHERE idu = $1", [req.body.id], () => res.sendStatus(200));
});

// Редактирование пользователя
app.get("/edituser", (req, res) => {
    pool.query("SELECT * FROM users WHERE idu = $1", [req.query.edit_id], (err, data) => {
        res.render('edituser', data.rows[0]);
    });
});

app.post("/edituser", (req, res) => {
    pool.query(
        "UPDATE users SET loginu = $2, pass = $3, nameu = $4, lastname = $5, patronymic = $6 WHERE idu = $1", 
        [req.query.edit_id, req.body.login, req.body.password, req.body.name, req.body.lastname, req.body.patronymic], 
        () => res.redirect("/")
    );
});

// Добавление бота
app.get("/addbot", (req, res) => {
    res.render("addbot");
});

app.post("/addbot", (req, res) => {
    pool.query(
        "INSERT INTO bots (ownerid, nameu, descriptionu, dateofregistration, statusu) VALUES ($1, $2, $3, $4, $5)", 
        [1, req.body.name, req.body.description, new Date().toISOString().slice(0, 19).replace('T', ' '), req.body.status], 
        () => res.redirect("/")
    );
});

// Удаление бота
app.delete("/deletebot", (req, res) => {
    pool.query("DELETE FROM bots WHERE idu = $1", [req.body.id], () => res.sendStatus(200));
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});