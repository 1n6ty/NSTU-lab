CREATE TABLE IF NOT EXISTS users
(
    idu SERIAL PRIMARY KEY,
    loginu VARCHAR(30) UNIQUE,
    pass VARCHAR(30),
    lastname VARCHAR(30),
    nameu VARCHAR(30),
    patronymic VARCHAR(30),
    dateofregistration TIMESTAMP,
    datelastconnect TIMESTAMP,
    statusu VARCHAR(30)
);

CREATE TABLE IF NOT EXISTS bots
(
    idu SERIAL PRIMARY KEY,
    ownerid INT,
    nameu VARCHAR(30),
    descriptionu TEXT,
    FOREIGN KEY (ownerid) REFERENCES users(idu) ON DELETE CASCADE,
    dateofregistration TIMESTAMP,
    statusu VARCHAR(30)
);

INSERT INTO users (loginu, pass, nameu, statusu) VALUES ('admin', '123', 'Superuser', 'admin');