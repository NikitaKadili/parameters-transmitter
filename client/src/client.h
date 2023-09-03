#pragma once

#include <cstdint>
#include <QtWidgets>
#include <QUdpSocket>

const uint16_t DEFAULT_PORT = 2424;

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

    bool operator==(const DataVars& other);
};
// Структура представляет пакет упакованных данных структуры DataVars
struct DataPackage {
    uint16_t data1 = 0;
    uint16_t data2 = 0;
    uint16_t data3 = 0;
    uint16_t data4 = 0;
};

// Класс представляет Udp клиент для получения данных
class UdpClient : public QObject {
    Q_OBJECT
public:
    UdpClient();

    // Метод задает порт клиента
    void SetPort(uint16_t port);

private slots:
    // Слот вызывается для получения и расшифровки данных
    void slotProcessData();

signals:
    // Сигнал отправляет полученные и распакованные данные в объекте типа DataVars
    void signalDataRecieved(const DataVars&);

private:
    QUdpSocket* socket_;   // Указатель на udp-сокет

    // Метод возвращает распакованные данные
    DataVars GetUnpackedData(const DataPackage&) const;
};
