require('dotenv').config();
const TelegramBot = require('node-telegram-bot-api');
const dbModule = require('./database');
const handlers = require('./handlers');
const keyboards = require('./keyboard');

// Инициализация бота
const token = process.env.BOT_TOKEN;
if (!token) {
    console.error('BOT_TOKEN не найден в .env файле!');
    process.exit(1);
}

const bot = new TelegramBot(token, { polling: true });

// Основная функция
async function main() {
    const db = await dbModule.initDb();
    console.log('Бот запущен...');
    console.log('База данных инициализирована');

    // Обработчик команды /start
    bot.onText(/\/start/, async (msg) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        await handlers.sendWelcomeWithTasks(bot, db, chatId, userId);
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
        await bot.sendMessage(chatId, helpText, keyboards.createMainKeyboard());
    });

    // Обработчик команды /done
    bot.onText(/\/done (.+)/, async (msg, match) => {
        const chatId = msg.chat.id;
        const userId = msg.from.id;
        const taskId = match[1].trim();

        try {
            if (await dbModule.markTaskDone(db, userId, parseInt(taskId))) {
                await bot.sendMessage(chatId, `✅ Задача ${taskId} отмечена как выполненная`, keyboards.createMainKeyboard());
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
            if (await dbModule.deleteTask(db, userId, parseInt(taskId))) {
                await bot.sendMessage(chatId, `🗑️ Задача ${taskId} удалена`, keyboards.createMainKeyboard());
            } else {
                await bot.sendMessage(chatId, `❌ Задача с ID ${taskId} не найдена`);
            }
        } catch (error) {
            await bot.sendMessage(chatId, '❌ ID задачи должен быть числом\n\nПример: /delete 1');
        }
    });

    // Обработчик текстовых сообщений
    bot.on('message', async (msg) => {
        await handlers.handleMessage(bot, db, msg);
    });

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

    process.on('SIGTERM', async () => {
        console.log('Получен сигнал завершения...');
        await db.close();
        process.exit(0);
    });
}

// Запуск приложения
main().catch(console.error);