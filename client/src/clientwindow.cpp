#include "clientwindow.h"
#include "./ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(new Ui::ClientWindow)
    , client_(new UdpClient)
{
    ui_->setupUi(this);
    ui_->port_spbox->setValue(DEFAULT_PORT);
    ui_->portwarning_lbl->setText("");

    connect(client_, SIGNAL(signalDataRecieved(const DataVars&)),
            SLOT(slotUpdateData(const DataVars&)));
    connect(ui_->changeport_button, SIGNAL(pressed()),
            SLOT(slotUpdatePort()));
    connect(ui_->port_spbox, SIGNAL(valueChanged(int)),
            SLOT(slotPortChanged(int)));
}

ClientWindow::~ClientWindow() {
    delete client_;
    delete ui_;
}

void ClientWindow::slotUpdateData(const DataVars& data) {
    if (cache_ && cache_.value() == data) {
        return;
    }
    cache_ = data;

    ui_->x_edit->setText(QString::number(data.x));
    ui_->y_edit->setText(QString::number(data.y));
    ui_->velocity_edit->setText(QString::number(data.velocity));
    ui_->mode_edit->setText(QString::number(data.mode));
    ui_->state_edit->setText(QString::number(data.state));
    ui_->acc_edit->setText(QString::number(data.acc));
    ui_->power_edit->setText(QString::number(data.power));

#define SET_RBIT(num) {                     \
    if ((data.r & 1<<num) > 0) {            \
        ui_->bit##num##_edit->setText("1"); \
    }                                       \
    else {                                  \
        ui_->bit##num##_edit->setText("0"); \
    }                                       \
}

    SET_RBIT(0);  SET_RBIT(1);  SET_RBIT(2);  SET_RBIT(3);
    SET_RBIT(4);  SET_RBIT(5);  SET_RBIT(6);  SET_RBIT(7);
    SET_RBIT(8);  SET_RBIT(9);  SET_RBIT(10); SET_RBIT(11);
    SET_RBIT(12); SET_RBIT(13); SET_RBIT(14); SET_RBIT(15);

#undef SET_RBIT
}

void ClientWindow::slotUpdatePort() {
    port_ = ui_->port_spbox->value();
    client_->SetPort(ui_->port_spbox->value());

    ui_->portwarning_lbl->setText("");
    ui_->changeport_button->setEnabled(false);
}

void ClientWindow::slotPortChanged(int new_port) {
    bool is_new = port_ != new_port;

    ui_->portwarning_lbl->setText(is_new
                    ? "Порт был изменен. Нажмите \"Изменить\", чтобы изменения вступили в силу"
                    : "");
    ui_->changeport_button->setEnabled(is_new);
}
