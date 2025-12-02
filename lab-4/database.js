const sqlite3 = require('sqlite3').verbose();
const { open } = require('sqlite');

// Инициализация базы данных
async function initDb() {
    const db = await open({
        filename: process.env.DB_FILENAME || 'tasks.db',
        driver: sqlite3.Database
    });
    
    await db.exec(`
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            task_text TEXT NOT NULL,
            is_completed BOOLEAN DEFAULT FALSE,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    `);
    
    return db;
}

// Функции для работы с базой данных
async function addTask(db, userId, taskText) {
    const result = await db.run(
        'INSERT INTO tasks (user_id, task_text) VALUES (?, ?)',
        [userId, taskText]
    );
    return result.lastID;
}

async function getTasks(db, userId, showCompleted = false) {
    if (showCompleted) {
        return await db.all(
            'SELECT id, task_text, is_completed FROM tasks WHERE user_id = ? ORDER BY id',
            [userId]
        );
    } else {
        return await db.all(
            'SELECT id, task_text FROM tasks WHERE user_id = ? AND is_completed = FALSE ORDER BY id',
            [userId]
        );
    }
}

async function getCompletedTasks(db, userId) {
    return await db.all(
        'SELECT id, task_text FROM tasks WHERE user_id = ? AND is_completed = TRUE ORDER BY id',
        [userId]
    );
}

async function getAllTasksCount(db, userId) {
    const result = await db.get(
        'SELECT COUNT(*) as count FROM tasks WHERE user_id = ?',
        [userId]
    );
    return result.count;
}

async function getActiveTasksCount(db, userId) {
    const result = await db.get(
        'SELECT COUNT(*) as count FROM tasks WHERE user_id = ? AND is_completed = FALSE',
        [userId]
    );
    return result.count;
}

async function markTaskDone(db, userId, taskId) {
    const result = await db.run(
        'UPDATE tasks SET is_completed = TRUE WHERE id = ? AND user_id = ?',
        [taskId, userId]
    );
    return result.changes > 0;
}

async function deleteTask(db, userId, taskId) {
    const result = await db.run(
        'DELETE FROM tasks WHERE id = ? AND user_id = ?',
        [taskId, userId]
    );
    return result.changes > 0;
}

async function clearAllTasks(db, userId) {
    const result = await db.run(
        'DELETE FROM tasks WHERE user_id = ?',
        [userId]
    );
    return result.changes;
}

async function taskExists(db, userId, taskId) {
    const result = await db.get(
        'SELECT id FROM tasks WHERE id = ? AND user_id = ?',
        [taskId, userId]
    );
    return result !== undefined;
}

module.exports = {
    initDb,
    addTask,
    getTasks,
    getCompletedTasks,
    getAllTasksCount,
    getActiveTasksCount,
    markTaskDone,
    deleteTask,
    clearAllTasks,
    taskExists
};