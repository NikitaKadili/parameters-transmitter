#pragma once

#include <cstdint>
#include <QtWidgets>
#include <QUdpSocket>

// Структура представляет набор данных
struct DataVars {
    uint8_t x = 0;        // Координата X
    int8_t y = 0;         // Координата Y
    uint8_t velocity = 0; // Скорость
    uint8_t mode = 0;     // Режим работы
    uint8_t state = 0;    // Состояние
    double acc = 0;       // Ускорение
    uint8_t power = 0;    // Мощность
    uint16_t r = 0;       // Резерв
};
// Структура представляет пакет упакованных данных структуры DataVars
struct DataPackage {
    uint16_t data1 = 0;
    uint16_t data2 = 0;
    uint16_t data3 = 0;
    uint16_t data4 = 0;
};

// Класс представляет Udp сервер для отправки данных с формы
class UdpServer : public QObject {
    Q_OBJECT
public:
    UdpServer();

    // Метод принимает структуру данных и адрес для отправки, групирует и отправляет адресату
    void TransmitData(const DataVars&, const QHostAddress&, uint16_t);

private:
    QUdpSocket* socket_; // Указатель на сокет

    // Метод возвращает упакованные данные
    DataPackage GetPackedData(const DataVars&) const;
};
