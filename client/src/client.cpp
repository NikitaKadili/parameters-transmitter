#include "client.h"

#include <tuple>

UdpClient::UdpClient()
    : socket_(new QUdpSocket(this))
{
    socket_->bind(DEFAULT_PORT);
    connect(socket_, SIGNAL(readyRead()), SLOT(slotProcessData()));
}

void UdpClient::SetPort(uint16_t port) {
    socket_->abort();
    socket_->bind(port);
}

void UdpClient::slotProcessData() {
    QByteArray data;

    do {
        data.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(data.data(), data.size());
    } while (socket_->hasPendingDatagrams());

    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15);

    DataPackage recieved_package;
    in >> recieved_package.data1 >> recieved_package.data2
            >> recieved_package.data3 >> recieved_package.data4;

    emit signalDataRecieved(GetUnpackedData(recieved_package));
}

DataVars UdpClient::GetUnpackedData(const DataPackage& package) const {
    // Для десериализации данных накладываем битовые маски,
    // сдвигаем биты вправо при необходимости
    DataVars data = {
        static_cast<uint8_t>(package.data1 & 63),                  // Координата X
        static_cast<int8_t>(package.data1 >> 8),                   // Координата Y
        static_cast<uint8_t>(package.data2 & 255),                 // Скорость
        static_cast<uint8_t>((package.data2 & 768) >> 8),          // Режим работы
        static_cast<uint8_t>(package.data2 >> 12),                 // Состояние
        (static_cast<double>(package.data3 & 255) - 127.0) / 10.0, // Ускорение
        static_cast<uint8_t>(package.data3 >> 8),                  // Мощность
        package.data4                                              // Резерв
    };

    return data;
}

bool DataVars::operator==(const DataVars& other) {
    return std::tie(x, y, velocity, mode, state, acc, power, r)
            == std::tie(other.x, other.y, other.velocity, other.mode,
                        other.state, other.acc, other.power, other.r);
}
