#pragma once

#include <cstdint>
#include <QMainWindow>
#include <QUdpSocket>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow {
    Q_OBJECT
public:
    ServerWindow(QWidget *parent = nullptr);

    ~ServerWindow();

public slots:
    // Слот проверяет, соответствует ли введенные IP-адрес и порт текущим значениям.
    // Если они идентичны - ничего не делает. Если они отличаются - разблокирует кнопку
    // "Обновить", выводит соответствующее вспомогательное сообщение
    void slotIpOrPortChanged(const QString&);

    // Слот проверяет корректность ввода IP-адреса и порта.
    // Если все верно - обновляет их, стирает вспомогательное сообщение и блокирует кнопку.
    // Иначе - выводит сообщение об ошибке
    void slotIpAndPortUpdateAsked();

    // Слот направляет в объект класса Server данные для передачи, текущие IP-адрес и порт
    void slotTransmitData();

    // Слот проверяет корректность введенных в поля данных.
    // Если все верно - обновляет data_, иначе - выделяет
    // некорректно заполненное поле цветом
    void slotCheckAndUpdateData(const QString&);

private:
    Ui::ServerWindow *ui_;
    UdpServer* server_;

    QHostAddress current_ip_; // Текущий IP-адрес клиента
    uint16_t current_port_;   // Текущий порт клиента

    DataVars data_; // Введенные данные

    // Метод связывает необходимые слоты и сигналы
    void SetSignalsAndSlots();

    // Метод вводит значения IP-адреса и порта по умолчанию в соответствующие
    // текстовые поля, создает необходимые для этих полей валидаторы
    void InitIpAndPort();

    // Метод вводит значения по умолчанию (0) в поля данных, создает
    // необходимые для этих полей валидаторы
    void InitData();

    // Метод считывает и возвращает значение поля R
    uint16_t GetRValue() const;
};
