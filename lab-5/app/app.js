const { Pool } = require("pg");
const bodyParser = require('body-parser');
const express = require('express');

require('dotenv').config();

const pool = new Pool({
    host: process.env.DB_HOST,
    database: process.env.DB_NAME,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    port: 5432
});

const app = express();
const PORT = 80;

app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');
app.use(bodyParser.urlencoded({extended:true}));
app.use(bodyParser.json());

app.get("/", (req, res) => {
    if(req.query.login !== undefined && req.query.password !== undefined){
        pool.query(
            "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
            [req.query.login, req.query.password], 
            (err, user_data) => {
                if(err) return console.error(err);
                if(user_data.rowCount == 0){
                    res.render("index");
                    return;
                }
                pool.query("UPDATE users SET datelastconnect = $1 WHERE idu = $2", [new Date().toISOString().slice(0, 19).replace('T', ' '), user_data.rows[0].idu], (err, data) => {if(err) return console.error(err);});
                let render_options = {};
                pool.query(
                    "SELECT bots.idu AS botid, bots.ownerid, bots.nameu, bots.descriptionu, users.loginu AS ownerlogin, bots.dateofregistration, bots.statusu FROM bots LEFT JOIN users ON users.idu = bots.ownerid;", 
                    [],
                    (err, data) => {
                        if(err) return console.error(err);
                        let bot_render_options = data.rows;
                        if(user_data.rowCount > 0 && user_data.rows[0].statusu == 'admin'){
                            for(let r = 0; r < data.rowCount; r++) bot_render_options[r].editable = true;
                        } else {
                            for(let r = 0; r < data.rowCount; r++) if(bot_render_options[r].ownerid == user_data.rows[0].idu) bot_render_options[r].editable = true;
                        }
                        render_options.bots = bot_render_options;
                        if(render_options.users !== undefined && render_options.bots !== undefined) res.render('lists', render_options);
                    }
                );
                pool.query(
                    "SELECT * FROM users;", 
                    [],
                    (err, data) => {
                        if(err) return console.error(err);
                        let user_render_options = data.rows;
                        if(user_data.rowCount > 0 && user_data.rows[0].statusu == 'admin'){
                            for(let r = 0; r < data.rowCount; r++) user_render_options[r].editable = true;
                        } else {
                            for(let r = 0; r < data.rowCount; r++) if(user_render_options[r].loginu == user_data.rows[0].loginu) user_render_options[r].editable = true;
                        }
                        render_options.users = user_render_options;
                        if(render_options.users !== undefined && render_options.bots !== undefined) res.render('lists', render_options);
                    }
                );
            }
        );
    } else {
        res.render("index");
    }
});

app.get("/register/", (req, res) => {
    res.render("register");
});
app.post("/register/", (req, res) => {
    if(!req.body) return res.sendStatus(400);

    let current_date = new Date().toISOString().slice(0, 19).replace('T', ' ');
    const statusu = "user";
    pool.query(
        "INSERT INTO users (loginu, pass, lastname, nameu, patronymic, dateofregistration, datelastconnect, statusu) VALUES ($1, $2, $3, $4, $5, $6, $7, $8);", 
        [req.body.loginu, req.body.passwordu, req.body.nameu, req.body.lastname, req.body.patronymic, current_date, current_date, statusu], 
        (err, data) => {
            if(err){
                res.sendStatus(403);
            } else res.redirect(`/?login=${req.body.loginu}&password=${req.body.passwordu}`);
        }
    );
});

app.delete("/deleteuser/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }
            
            if(user_data.rows[0].statusu == 'admin' || user_data.rows[0].idu == req.body.id){
                pool.query(
                    "DELETE FROM users WHERE idu = $1;", 
                    [req.body.id], 
                    (err, data) => {
                        if(err) return console.error(err);
                    }
                );
            }

            res.sendStatus(200);
        }
    );
});

app.get("/edituser/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }

            if(user_data.rows[0].statusu == 'admin' || user_data.rows[0].idu == req.query.edit_id){
                pool.query(
                    "SELECT * FROM users WHERE idu = $1", 
                    [req.query.edit_id], 
                    (err, user_edit_data) => {
                        if(err) return console.error(err);
                        if(user_edit_data.rowCount == 0){
                            res.sendStatus(404);
                            return;
                        }
                        
                        res.render('edituser', user_edit_data.rows[0]);
                    }
                )
            } else {
                res.sendStatus(403);
            }
        }
    );
});
app.post("/edituser/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }

            if(user_data.rows[0].statusu == 'admin' || user_data.rows[0].idu == req.query.edit_id){
                pool.query(
                    "UPDATE users SET loginu = $2, pass = $3, nameu = $4, lastname = $5, patronymic = $6 WHERE idu = $1", 
                    [req.query.edit_id, req.body.login, req.body.password, req.body.name, req.body.lastname, req.body.patronymic], 
                    (err, data) => {
                        if(err) return console.error(err);
                        
                        res.redirect(`/?login=${req.query.login}&password=${req.query.password}`);
                    }
                )
            } else {
                res.sendStatus(403);
            }
        }
    );
});

app.get("/addbot/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }
            
            res.render("addbot");
        }
    );
});
app.post("/addbot/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }
            
            pool.query(
                "INSERT INTO bots (ownerid, nameu, descriptionu, dateofregistration, statusu) VALUES ($1, $2, $3, $4, $5);", 
                [user_data.rows[0].idu, req.body.name, req.body.description, new Date().toISOString().slice(0, 19).replace('T', ' '), req.body.status],
                (err, data) => {
                    if(err) return console.error(err);
                    else res.redirect(`/?login=${req.query.login}&password=${req.query.password}`);
                }
            );
        }
    );
});

app.delete("/deletebot/", (req, res) => {
    pool.query(
        "SELECT * FROM users WHERE loginu = $1 and pass = $2;", 
        [req.query.login, req.query.password], 
        (err, user_data) => {
            if(err) return console.error(err);
            if(user_data.rowCount == 0){
                res.sendStatus(403);
                return;
            }
            
            if(user_data.rows[0].statusu == 'admin'){
                pool.query(
                    "DELETE FROM bots WHERE idu = $1;", 
                    [req.body.id], 
                    (err, data) => {
                        if(err) return console.error(err);
                        res.sendStatus(200);
                    }
                );
            } else {
                pool.query(
                    "SELECT ownerid FROM bots WHERE idu = $1;", 
                    [req.body.id], 
                    (err, data) => {
                        if(err) return console.error(err);
                        if(data.rowCount > 0 && data.rows[0].ownerid == user_data.rows[0].idu){
                            pool.query(
                                "DELETE FROM bots WHERE idu = $1;", 
                                [req.body.id], 
                                (err, data) => {
                                    if(err) return console.error(err);
                                    res.sendStatus(200);
                                }
                            );
                        } else {
                            res.sendStatus(404);
                            return;
                        }
                    }
                );
            }
        }
    );
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});