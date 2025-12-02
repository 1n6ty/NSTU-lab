const dbOperations = require('./database');
const keyboards = require('./keyboard');

// Состояния пользователей для обработки ввода
const userStates = {};

// Функция для отправки приветственного сообщения с задачами
async function sendWelcomeWithTasks(bot, db, chatId, userId) {
    const activeTasks = await dbOperations.getTasks(db, userId);
    const activeCount = await dbOperations.getActiveTasksCount(db, userId);
    const totalCount = await dbOperations.getAllTasksCount(db, userId);

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

    await bot.sendMessage(chatId, welcomeText, keyboards.createMainKeyboard());
}

// Функция для обработки списка задач
async function listTasksHandler(bot, db, chatId, userId) {
    try {
        const tasks = await dbOperations.getTasks(db, userId);

        if (!tasks || tasks.length === 0) {
            await bot.sendMessage(
                chatId,
                "📝 Список активных задач пуст\n\nИспользуйте кнопку '📝 Добавить задачу' для создания новой задачи",
                keyboards.createMainKeyboard()
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
async function completedTasksHandler(bot, db, chatId, userId) {
    try {
        const tasks = await dbOperations.getCompletedTasks(db, userId);

        if (!tasks || tasks.length === 0) {
            await bot.sendMessage(
                chatId,
                "✅ У вас нет выполненных задач\n\nВсе выполненные задачи будут отображаться здесь",
                keyboards.createMainKeyboard()
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
async function allTasksHandler(bot, db, chatId, userId) {
    try {
        const tasks = await dbOperations.getTasks(db, userId, true);

        if (!tasks || tasks.length === 0) {
            await bot.sendMessage(
                chatId,
                "📝 Список задач пуст\n\nИспользуйте кнопку '📝 Добавить задачу' для создания новой задачи",
                keyboards.createMainKeyboard()
            );
            return;
        }

        let tasksText = "🔄 Все ваши задачи:\n\n";
        tasks.forEach(task => {
            const status = task.is_completed ? "✅" : "⏳";
            tasksText += `${status} ${task.id}: ${task.task_text}\n`;
        });

        const activeCount = await dbOperations.getActiveTasksCount(db, userId);
        const totalCount = await dbOperations.getAllTasksCount(db, userId);
        const completedCount = totalCount - activeCount;

        tasksText += `\n📊 Статистика: ${activeCount} активных, ${completedCount} выполненных, всего ${totalCount}`;
        tasksText += "\n\nИспользуйте кнопки '✅ Выполнить задачу' или '🗑️ Удалить задачу' для работы с задачами";
        await bot.sendMessage(chatId, tasksText);
    } catch (error) {
        await bot.sendMessage(chatId, `❌ Ошибка при получении списка задач: ${error.message}`);
    }
}

// Обработчик текстовых сообщений
async function handleMessage(bot, db, msg) {
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
            await bot.sendMessage(chatId, "❌ Действие отменено", keyboards.createMainKeyboard());
            return;
        }

        switch (state) {
            case 'waiting_for_task':
                if (!text.trim()) {
                    await bot.sendMessage(chatId, "❌ Текст задачи не может быть пустым. Попробуйте еще раз:");
                    return;
                }
                
                try {
                    const taskId = await dbOperations.addTask(db, userId, text.trim());
                    delete userStates[userId];
                    await bot.sendMessage(
                        chatId,
                        `✅ Задача добавлена (ID: ${taskId})\n\n📝 Текст: ${text.trim()}`,
                        keyboards.createMainKeyboard()
                    );
                } catch (error) {
                    delete userStates[userId];
                    await bot.sendMessage(
                        chatId,
                        `❌ Ошибка при добавлении задачи: ${error.message}`,
                        keyboards.createMainKeyboard()
                    );
                }
                break;

            case 'waiting_for_done_id':
                if (text === '📋 Список задач') {
                    await listTasksHandler(bot, db, chatId, userId);
                    return;
                }
                
                try {
                    const taskId = parseInt(text.trim());
                    if (await dbOperations.markTaskDone(db, userId, taskId)) {
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            `✅ Задача ${taskId} отмечена как выполненная`,
                            keyboards.createMainKeyboard()
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
                    await listTasksHandler(bot, db, chatId, userId);
                    return;
                }
                
                try {
                    const taskId = parseInt(text.trim());
                    if (await dbOperations.deleteTask(db, userId, taskId)) {
                        delete userStates[userId];
                        await bot.sendMessage(
                            chatId,
                            `🗑️ Задача ${taskId} удалена`,
                            keyboards.createMainKeyboard()
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
                    const deletedCount = await dbOperations.clearAllTasks(db, userId);
                    delete userStates[userId];
                    await bot.sendMessage(
                        chatId,
                        `🧹 Удалено ${deletedCount} задач!\n\nТеперь список задач пуст.`,
                        keyboards.createMainKeyboard()
                    );
                } else if (text === '❌ Нет, отменить') {
                    delete userStates[userId];
                    await bot.sendMessage(
                        chatId,
                        "❌ Очистка задач отменена",
                        keyboards.createMainKeyboard()
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
            await bot.sendMessage(chatId, helpText, keyboards.createMainKeyboard());
            break;

        case '📝 Добавить задачу':
            userStates[userId] = 'waiting_for_task';
            await bot.sendMessage(
                chatId,
                "📝 Введите текст новой задачи:",
                keyboards.createCancelKeyboard()
            );
            break;

        case '✅ Выполнить задачу':
            userStates[userId] = 'waiting_for_done_id';
            await bot.sendMessage(
                chatId,
                "✅ Введите ID задачи для отметки как выполненной:\n\nСначала посмотрите ID в списке задач 📋",
                keyboards.createTaskActionsKeyboard()
            );
            break;

        case '🗑️ Удалить задачу':
            userStates[userId] = 'waiting_for_delete_id';
            await bot.sendMessage(
                chatId,
                "🗑️ Введите ID задачи для удаления:\n\nСначала посмотрите ID в списке задач 📋",
                keyboards.createTaskActionsKeyboard()
            );
            break;

        case '🧹 Очистить всё':
            const totalTasks = await dbOperations.getAllTasksCount(db, userId);
            if (totalTasks === 0) {
                await bot.sendMessage(
                    chatId,
                    "📝 У вас нет задач для очистки",
                    keyboards.createMainKeyboard()
                );
                return;
            }
            userStates[userId] = 'waiting_for_clear_confirmation';
            await bot.sendMessage(
                chatId,
                `⚠️ Вы уверены, что хотите удалить ВСЕ задачи?\n\nВсего задач: ${totalTasks}\nЭто действие нельзя отменить!`,
                keyboards.createConfirmKeyboard()
            );
            break;

        case '📋 Список задач':
            await listTasksHandler(bot, db, chatId, userId);
            break;

        case '✅ Выполненные':
            await completedTasksHandler(bot, db, chatId, userId);
            break;

        case '🔄 Все задачи':
            await allTasksHandler(bot, db, chatId, userId);
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
                await bot.sendMessage(chatId, helpText, keyboards.createMainKeyboard());
            }
            break;
    }
}

module.exports = {
    sendWelcomeWithTasks,
    handleMessage,
    listTasksHandler,
    completedTasksHandler,
    allTasksHandler,
    userStates
};