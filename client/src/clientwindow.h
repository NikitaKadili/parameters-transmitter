#pragma once

#include <cstdint>
#include <optional>
#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow {
    Q_OBJECT
public:
    ClientWindow(QWidget* parent = nullptr);

    ~ClientWindow();

public slots:
    // Слот вызывается для обновления выводимых данных
    void slotUpdateData(const DataVars&);

    // Слот обновляет порт клиента
    void slotUpdatePort();

    // Слот проверяет соответствие нового значения порта текущему.
    // Если значения различаются - разблокирует кнопку "Изменить"
    // и выводит предупредительное сообщение
    void slotPortChanged(int);

private:
    Ui::ClientWindow *ui_;
    UdpClient* client_;

    // Кэш хранит текущие значения переменных для оптимизации
    mutable std::optional<DataVars> cache_;
    uint16_t port_ = DEFAULT_PORT;
};
