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

module.exports = {
    createMainKeyboard,
    createCancelKeyboard,
    createConfirmKeyboard,
    createTaskActionsKeyboard
};