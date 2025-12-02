const TelegramBot = require('node-telegram-bot-api');
const sqlite3 = require('sqlite3').verbose();
const { open } = require('sqlite');

// Инициализация бота
const token = "8277406815:AAHF4q76xiyqtrxiuz-FlQJYNNA94l9QJCs";
const bot = new TelegramBot(token, { polling: true });

// Состояния пользователей для обработки ввода
const userStates = {};

// Инициализация базы данных
async function initDb() {
    const db = await open({
        filename: 'tasks.db',
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

// Главная клавиатура
function createMainKeyboard() {
    return {
        reply_markup: {
            keyboard: [
                ['📝 Добавить задачу', '📋 Список задач'],
                ['✅ Выполненные', '🔄 Все задачи'],
                ['✅ Выполнить задачу', '🗑️ Удалить задачу'],
                ['🧹 Очистить всё', '❓ Помощь']
            ],
            resize_keyboard: true
        }
    };
}

// Клавиатура для отмены
function createCancelKeyboard() {
    return {
        reply_markup: {
            keyboard: [['❌ Отмена']],
            resize_keyboard: true
        }
    };
}

// Клавиатура подтверждения
function createConfirmKeyboard() {
    return {
        reply_markup: {
            keyboard: [
                ['✅ Да, очистить всё', '❌ Нет, отменить']
            ],
            resize_keyboard: true
        }
    };
}

// Клавиатура действий с задачами
function createTaskActionsKeyboard() {
    return {
        reply_markup: {
            keyboard: [
                ['📋 Список задач', '❌ Отмена']
            ],
            resize_keyboard: true
        }
    };
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

// Функция для отправки приветственного сообщения с задачами
async function sendWelcomeWithTasks(db, chatId, userId) {
    const activeTasks = await getTasks(db, userId);
    const activeCount = await getActiveTasksCount(db, userId);
    const totalCount = await getAllTasksCount(db, userId);

    let welcomeText = '';

    if (activeCount === 0) {
        if (totalCount === 0) {
            // Нет задач вообще
            welcomeText = `
🤖 Добро пожаловать в Персональный Менеджер Задач!

📝 У вас пока нет задач. Начните с добавления первой задачи!

Доступные команды:
📝 Добавить задачу - создать новую задачу
📋 Список задач - показать активные задачи
✅ Выполненные - показать выполненные задачи
🔄 Все задачи - показать все задачи
✅ Выполнить задачу - отметить задачу как выполненную
🗑️ Удалить задачу - удалить задачу
🧹 Очистить всё - удалить все задачи
❓ Помощь - показать справку
`;
        } else {
            // Есть только выполненные задачи
            const completedCount = totalCount;
            welcomeText = `
🤖 Добро пожаловать в Персональный Менеджер Задач!

🎉 Отличная работа! У вас нет активных задач - все ${completedCount} задач выполнены!

Доступные команды:
📝 Добавить задачу - создать новую задачу
✅ Выполненные - показать выполненные задачи
🔄 Все задачи - показать все задачи
🧹 Очистить всё - удалить все задачи
❓ Помощь - показать справку
`;
        }
    } else {
        // Есть активные задачи
        welcomeText = `
🤖 Добро пожаловать в Персональный Менеджер Задач!

📋 У вас ${activeCount} активных задач:

`;
        activeTasks.forEach(task => {
            welcomeText += `🆔 ${task.id}: ${task.task_text}\n`;
        });

        welcomeText += `

Доступные команды:
📝 Добавить задачу - создать новую задачу
📋 Список задач - показать активные задачи
✅ Выполненные - показать выполненные задачи
🔄 Все задачи - показать все задачи
✅ Выполнить задачу - отметить задачу как выполненную
🗑️ Удалить задачу - удалить задачу
🧹 Очистить всё - удалить все задачи
❓ Помощь - показать справку
`;
    }

    await bot.sendMessage(chatId, welcomeText, createMainKeyboard());
}

// Основная функция
async function main() {
    const db = await initDb();
    console.log('Бот запущен...');

    // Обработчик команды /start
    bot.onText(/\/start/, async (msg) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        await sendWelcomeWithTasks(db, chatId, userId);
    });

    // Обработчик команды /help
    bot.onText(/\/help/, async (msg) => {
        const chatId = msg.chat.id;
        const helpText = `
📖 **Справка по командам:**

**Основные действия (кнопки):**
📝 Добавить задачу - создать новую задачу
📋 Список задач - показать активные задачи  
✅ Выполненные - показать выполненные задачи
🔄 Все задачи - показать все задачи
✅ Выполнить задачу - отметить задачу как выполненную
🗑️ Удалить задачу - удалить задачу
🧹 Очистить всё - удалить ВСЕ задачи

**Команды для работы с задачами:**
/done ID - отметить задачу как выполненную
/delete ID - удалить задачу

**Примеры:**
/done 1 - отметить задачу №1 как выполненную
/delete 3 - удалить задачу №3

Просто нажмите на нужную кнопку внизу экрана!
`;
        await bot.sendMessage(chatId, helpText, createMainKeyboard());
    });

    // Обработчик команды /done
    bot.onText(/\/done (.+)/, async (msg, match) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        const taskId = match[1].trim();

        try {
            if (await markTaskDone(db, userId, parseInt(taskId))) {
                await bot.sendMessage(chatId, `✅ Задача ${taskId} отмечена как выполненная`, createMainKeyboard());
            } else {
                await bot.sendMessage(chatId, `❌ Задача с ID ${taskId} не найдена`);
            }
        } catch (error) {
            await bot.sendMessage(chatId, '❌ ID задачи должен быть числом\n\nПример: /done 1');
        }
    });

    // Обработчик команды /delete
    bot.onText(/\/delete (.+)/, async (msg, match) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        const taskId = match[1].trim();

        try {
            if (await deleteTask(db, userId, parseInt(taskId))) {
                await bot.sendMessage(chatId, `🗑️ Задача ${taskId} удалена`, createMainKeyboard());
            } else {
                await bot.sendMessage(chatId, `❌ Задача с ID ${taskId} не найдена`);
            }
        } catch (error) {
            await bot.sendMessage(chatId, '❌ ID задачи должен быть числом\n\nПример: /delete 1');
        }
    });

    // Обработчик текстовых сообщений (кнопок)
    bot.on('message', async (msg) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        const text = msg.text;

        // Пропускаем команды, которые обрабатываются отдельно
        if (text.startsWith('/')) {
            return;
        }

        // Обработка состояний пользователя
        if (userStates[userId]) {
            const state = userStates[userId];
            
            if (text === '❌ Отмена') {
                delete userStates[userId];
                await bot.sendMessage(chatId, "❌ Действие отменено", createMainKeyboard());
                return;
            }

            switch (state) {
                case 'waiting_for_task':
                    if (!text.trim()) {
                        await bot.sendMessage(chatId, "❌ Текст задачи не может быть пустым. Попробуйте еще раз:");
                        return;
                    }
                    
                    try {
                        const taskId = await addTask(db, userId, text.trim());
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            `✅ Задача добавлена (ID: ${taskId})\n\n📝 Текст: ${text.trim()}`,
                            createMainKeyboard()
                        );
                    } catch (error) {
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            `❌ Ошибка при добавлении задачи: ${error.message}`,
                            createMainKeyboard()
                        );
                    }
                    break;

                case 'waiting_for_done_id':
                    if (text === '📋 Список задач') {
                        await listTasksHandler(chatId, userId);
                        return;
                    }
                    
                    try {
                        const taskId = parseInt(text.trim());
                        if (await markTaskDone(db, userId, taskId)) {
                            delete userStates[userId];
                            await bot.sendMessage(
                                chatId,
                                `✅ Задача ${taskId} отмечена как выполненная`,
                                createMainKeyboard()
                            );
                        } else {
                            await bot.sendMessage(
                                chatId,
                                `❌ Задача с ID ${taskId} не найдена. Попробуйте еще раз:`
                            );
                        }
                    } catch (error) {
                        await bot.sendMessage(
                            chatId,
                            "❌ ID задачи должен быть числом. Попробуйте еще раз:"
                        );
                    }
                    break;

                case 'waiting_for_delete_id':
                    if (text === '📋 Список задач') {
                        await listTasksHandler(chatId, userId);
                        return;
                    }
                    
                    try {
                        const taskId = parseInt(text.trim());
                        if (await deleteTask(db, userId, taskId)) {
                            delete userStates[userId];
                            await bot.sendMessage(
                                chatId,
                                `🗑️ Задача ${taskId} удалена`,
                                createMainKeyboard()
                            );
                        } else {
                            await bot.sendMessage(
                                chatId,
                                `❌ Задача с ID ${taskId} не найдена. Попробуйте еще раз:`
                            );
                        }
                    } catch (error) {
                        await bot.sendMessage(
                            chatId,
                            "❌ ID задачи должен быть числом. Попробуйте еще раз:"
                        );
                    }
                    break;

                case 'waiting_for_clear_confirmation':
                    if (text === '✅ Да, очистить всё') {
                        const deletedCount = await clearAllTasks(db, userId);
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            `🧹 Удалено ${deletedCount} задач!\n\nТеперь список задач пуст.`,
                            createMainKeyboard()
                        );
                    } else if (text === '❌ Нет, отменить') {
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            "❌ Очистка задач отменена",
                            createMainKeyboard()
                        );
                    }
                    break;
            }
            return;
        }

        // Обработка кнопок
        switch (text) {
            case '❓ Помощь':
                const helpText = `
📖 **Справка по командам:**

**Основные действия (кнопки):**
📝 Добавить задачу - создать новую задачу
📋 Список задач - показать активные задачи  
✅ Выполненные - показать выполненные задачи
🔄 Все задачи - показать все задачи
✅ Выполнить задачу - отметить задачу как выполненную
🗑️ Удалить задачу - удалить задачу
🧹 Очистить всё - удалить ВСЕ задачи

**Команды для работы с задачами:**
/done ID - отметить задачу как выполненную
/delete ID - удалить задачу

**Примеры:**
/done 1 - отметить задачу №1 как выполненную
/delete 3 - удалить задачу №3

Просто нажмите на нужную кнопку внизу экрана!
`;
                await bot.sendMessage(chatId, helpText, createMainKeyboard());
                break;

            case '📝 Добавить задачу':
                userStates[userId] = 'waiting_for_task';
                await bot.sendMessage(
                    chatId,
                    "📝 Введите текст новой задачи:",
                    createCancelKeyboard()
                );
                break;

            case '✅ Выполнить задачу':
                userStates[userId] = 'waiting_for_done_id';
                await bot.sendMessage(
                    chatId,
                    "✅ Введите ID задачи для отметки как выполненной:\n\nСначала посмотрите ID в списке задач 📋",
                    createTaskActionsKeyboard()
                );
                break;

            case '🗑️ Удалить задачу':
                userStates[userId] = 'waiting_for_delete_id';
                await bot.sendMessage(
                    chatId,
                    "🗑️ Введите ID задачи для удаления:\n\nСначала посмотрите ID в списке задач 📋",
                    createTaskActionsKeyboard()
                );
                break;

            case '🧹 Очистить всё':
                const totalTasks = await getAllTasksCount(db, userId);
                if (totalTasks === 0) {
                    await bot.sendMessage(
                        chatId,
                        "📝 У вас нет задач для очистки",
                        createMainKeyboard()
                    );
                    return;
                }
                userStates[userId] = 'waiting_for_clear_confirmation';
                await bot.sendMessage(
                    chatId,
                    `⚠️ Вы уверены, что хотите удалить ВСЕ задачи?\n\nВсего задач: ${totalTasks}\nЭто действие нельзя отменить!`,
                    createConfirmKeyboard()
                );
                break;

            case '📋 Список задач':
                await listTasksHandler(chatId, userId);
                break;

            case '✅ Выполненные':
                await completedTasksHandler(chatId, userId);
                break;

            case '🔄 Все задачи':
                await allTasksHandler(chatId, userId);
                break;

            default:
                if (!userStates[userId]) {
                    const helpText = `
Не понимаю команду 😕

Используйте кнопки ниже или команды:
📝 Добавить задачу - создать новую задачу
📋 Список задач - активные задачи
✅ Выполненные - выполненные задачи
🔄 Все задачи - все задачи
✅ Выполнить задачу - отметить задачу как выполненную
🗑️ Удалить задачу - удалить задачу
🧹 Очистить всё - удалить ВСЕ задачи
❓ Помощь - справка

Или команды:
/done ID - отметить выполненной
/delete ID - удалить задачу
`;
                    await bot.sendMessage(chatId, helpText, createMainKeyboard());
                }
                break;
        }
    });

    // Функция для обработки списка задач
    async function listTasksHandler(chatId, userId) {
        try {
            const tasks = await getTasks(db, userId);

            if (!tasks || tasks.length === 0) {
                await bot.sendMessage(
                    chatId,
                    "📝 Список активных задач пуст\n\nИспользуйте кнопку '📝 Добавить задачу' для создания новой задачи",
                    createMainKeyboard()
                );
                return;
            }

            let tasksText = "📋 Ваши активные задачи:\n\n";
            tasks.forEach(task => {
                tasksText += `🆔 ${task.id}: ${task.task_text}\n`;
            });

            tasksText += "\nИспользуйте кнопки '✅ Выполнить задачу' или '🗑️ Удалить задачу' для работы с задачами";
            await bot.sendMessage(chatId, tasksText);
        } catch (error) {
            await bot.sendMessage(chatId, `❌ Ошибка при получении списка задач: ${error.message}`);
        }
    }

    // Функция для обработки выполненных задач
    async function completedTasksHandler(chatId, userId) {
        try {
            const tasks = await getCompletedTasks(db, userId);

            if (!tasks || tasks.length === 0) {
                await bot.sendMessage(
                    chatId,
                    "✅ У вас нет выполненных задач\n\nВсе выполненные задачи будут отображаться здесь",
                    createMainKeyboard()
                );
                return;
            }

            let tasksText = "✅ Выполненные задачи:\n\n";
            tasks.forEach(task => {
                tasksText += `🆔 ${task.id}: ${task.task_text}\n`;
            });

            await bot.sendMessage(chatId, tasksText);
        } catch (error) {
            await bot.sendMessage(chatId, `❌ Ошибка при получении списка задач: ${error.message}`);
        }
    }

    // Функция для обработки всех задач
    async function allTasksHandler(chatId, userId) {
        try {
            const tasks = await getTasks(db, userId, true);

            if (!tasks || tasks.length === 0) {
                await bot.sendMessage(
                    chatId,
                    "📝 Список задач пуст\n\nИспользуйте кнопку '📝 Добавить задачу' для создания новой задачи",
                    createMainKeyboard()
                );
                return;
            }

            let tasksText = "🔄 Все ваши задачи:\n\n";
            tasks.forEach(task => {
                const status = task.is_completed ? "✅" : "⏳";
                tasksText += `${status} ${task.id}: ${task.task_text}\n`;
            });

            const activeCount = await getActiveTasksCount(db, userId);
            const totalCount = await getAllTasksCount(db, userId);
            const completedCount = totalCount - activeCount;

            tasksText += `\n📊 Статистика: ${activeCount} активных, ${completedCount} выполненных, всего ${totalCount}`;
            tasksText += "\n\nИспользуйте кнопки '✅ Выполнить задачу' или '🗑️ Удалить задачу' для работы с задачами";
            await bot.sendMessage(chatId, tasksText);
        } catch (error) {
            await bot.sendMessage(chatId, `❌ Ошибка при получении списка задач: ${error.message}`);
        }
    }

    // Обработка ошибок
    bot.on('polling_error', (error) => {
        console.error('Polling error:', error);
    });

    bot.on('webhook_error', (error) => {
        console.error('Webhook error:', error);
    });

    // Обработка завершения работы
    process.on('SIGINT', async () => {
        console.log('Завершение работы...');
        await db.close();
        process.exit(0);
    });
}

// Запуск приложения
main().catch(console.error);