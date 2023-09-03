#include "server.h"

UdpServer::UdpServer()
    : socket_(new QUdpSocket(this))
{ /* do nothing */ }

void UdpServer::TransmitData(const DataVars& to_send, const QHostAddress& address, uint16_t port) {
    QByteArray data;

    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    DataPackage package = GetPackedData(to_send);
    out << package.data1 << package.data2 << package.data3 << package.data4;

    socket_->writeDatagram(data, address, port);
}

DataPackage UdpServer::GetPackedData(const DataVars& to_send) const {
    // Упаковываем данные в слова в соответствии с заявленными позициями
    uint16_t data1 = (to_send.y<<8) + to_send.x;
    uint16_t data2 = (to_send.state<<12) + (to_send.mode<<8) + to_send.velocity;
    uint16_t data3 = (to_send.power<<8) + static_cast<uint8_t>(to_send.acc * 10.0 + 127);

    return { data1, data2, data3, to_send.r };
}

