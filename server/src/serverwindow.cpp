#include "serverwindow.h"
#include "./ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::ServerWindow)
    , server_(new UdpServer)
    , current_ip_("127.0.0.1")
    , current_port_(2424)
{
    ui_->setupUi(this);

    InitData();
    InitIpAndPort();

    SetSignalsAndSlots();

    // Устанавливаем таймер для периодической отправки данных клиенту
    QTimer* timer = new QTimer(this);
    timer->setInterval(500);
    timer->start();
    connect(timer, SIGNAL(timeout()), SLOT(slotTransmitData()));
}

ServerWindow::~ServerWindow() {
    delete server_;
    delete ui_;
}

void ServerWindow::slotIpOrPortChanged(const QString&) {
    quint32 new_ip = (ui_->ip1_ledit->text().toInt()<<24)
            + (ui_->ip2_ledit->text().toInt()<<16)
            + (ui_->ip3_ledit->text().toInt()<<8)
            + ui_->ip4_ledit->text().toInt();

    // Если IP-адрес и порт не поменялись - скрываем текст подсказки,
    // блокируем кнопку "Обновить"
    if (current_ip_.toIPv4Address() == new_ip
            && current_port_ == ui_->port_ledit->text().toInt())
    {
        ui_->warning_lbl->clear();
        ui_->update_button->setEnabled(false);

        return;
    }

    ui_->warning_lbl->setText("IP-адрес и/или порт клиента были изменены. "
                                   "Нажмите \"Обновить\", чтобы обновить данные клиента");
    ui_->update_button->setEnabled(true);
}

void ServerWindow::slotIpAndPortUpdateAsked() {
    int new_a = ui_->ip1_ledit->text().toInt();
    int new_b = ui_->ip2_ledit->text().toInt();
    int new_c = ui_->ip3_ledit->text().toInt();
    int new_d = ui_->ip4_ledit->text().toInt();
    int new_port = ui_->port_ledit->text().toInt();

    // Если введены некорректные данные - выводим соответствующее сообщение, выходим из слота
    if (new_a > 255 || new_b > 255 || new_c > 255 || new_d > 255
            || new_port > 65535)
    {
        ui_->warning_lbl->setText("Допущена ошибка при вводе IP-адреса или порта. Проверьте данные");

        return;
    }

    // Задаем новые текущие значения
    current_ip_.setAddress((new_a<<24) + (new_b<<16) + (new_c<<8) + new_d);
    current_port_ = static_cast<uint16_t>(new_port);

    // Очищаем предупреждающую строку, блокируем копку "Обновить"
    ui_->warning_lbl->clear();
    ui_->update_button->setEnabled(false);
}

void ServerWindow::slotTransmitData() {
    data_.r = GetRValue(); // Перед отправкой обновляем значение переменной R
    server_->TransmitData(data_, current_ip_, current_port_);
}

void ServerWindow::slotCheckAndUpdateData(const QString&) {
    uint val_uint;
    int val_int;
    double val_d;

    QString warning_style = "border: 1px solid red;"
            "padding-top: 2px;"
            "padding-bottom: 2px;"
            "border-radius: 2px";

#define CHECK_UINT_VAR(v, max) {                     \
    val_uint = ui_->v##_edit->text().toUInt();       \
    if (val_uint > max) {                            \
        ui_->v##_edit->setStyleSheet(warning_style); \
    }                                                \
    else {                                           \
        ui_->v##_edit->setStyleSheet("");            \
        data_.v = val_uint;                          \
    }                                                \
}

    // Проверка значения X
    CHECK_UINT_VAR(x, 63);

    // Проверка значения Y
    val_int = ui_->y_edit->text().toInt();
    if (val_int < -32 || val_int > 31) {
        ui_->y_edit->setStyleSheet(warning_style);
    }
    else {
        ui_->y_edit->setStyleSheet("");
        data_.y = val_int;
    }

    // Проверка значения скорости
    CHECK_UINT_VAR(velocity, 255);
    // Проверка значения режима
    CHECK_UINT_VAR(mode, 3);
    // Проверка значения состояния
    CHECK_UINT_VAR(state, 3);

    // Проверка значения ускорения
    val_d = ui_->acc_edit->text().toDouble();
    if (val_d < -12.7 || val_d > 12.8) {
        ui_->acc_edit->setStyleSheet(warning_style);
    }
    else {
        ui_->acc_edit->setStyleSheet("");
        data_.acc = val_d;
    }

    // Проверка значения мощности
    CHECK_UINT_VAR(power, 130);

#undef CHECK_UINT_VAR
}

void ServerWindow::SetSignalsAndSlots() {
    // Соединяем слоты, необходимые для обработки сигналов текстовых полей IP-адреса и порта
    connect(ui_->ip1_ledit, SIGNAL(textChanged(const QString&)),
            SLOT(slotIpOrPortChanged(const QString&)));
    connect(ui_->ip2_ledit, SIGNAL(textChanged(const QString&)),
            SLOT(slotIpOrPortChanged(const QString&)));
    connect(ui_->ip3_ledit, SIGNAL(textChanged(const QString&)),
            SLOT(slotIpOrPortChanged(const QString&)));
    connect(ui_->ip4_ledit, SIGNAL(textChanged(const QString&)),
            SLOT(slotIpOrPortChanged(const QString&)));
    connect(ui_->port_ledit, SIGNAL(textChanged(const QString&)),
            SLOT(slotIpOrPortChanged(const QString&)));

    // Соединяем слот, обрабатывающий запрос на обновление данных IP-адреса и порта
    connect(ui_->update_button, SIGNAL(clicked()),
            SLOT(slotIpAndPortUpdateAsked()));

    // Соединяем слот проверки введенных данных и актуализации поля data_
    // с событиями изменения данных в этих полях
    connect(ui_->x_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->y_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->velocity_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->mode_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->state_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->acc_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
    connect(ui_->power_edit, SIGNAL(textChanged(const QString&)),
            SLOT(slotCheckAndUpdateData(const QString&)));
}

void ServerWindow::InitIpAndPort() {
    // Устанавливаем в полях ввода значения по умолчанию
    ui_->ip1_ledit->setText("127");
    ui_->ip2_ledit->setText("0");
    ui_->ip3_ledit->setText("0");
    ui_->ip4_ledit->setText("1");
    ui_->port_ledit->setText("2424");

    // Задаем валидатор для полей ввода IP-адреса
    QValidator* ip_validator = new QIntValidator(0, 255, this);
    ui_->ip1_ledit->setValidator(ip_validator);
    ui_->ip2_ledit->setValidator(ip_validator);
    ui_->ip3_ledit->setValidator(ip_validator);
    ui_->ip4_ledit->setValidator(ip_validator);
    // Задаем валидатор для поля ввода порта
    QValidator* port_validator = new QIntValidator(0, 65535, this);
    ui_->port_ledit->setValidator(port_validator);
}

void ServerWindow::InitData() {
    // Устанавливаем в полях ввода значения по умолчанию
    ui_->x_edit->setText("0");
    ui_->y_edit->setText("0");
    ui_->velocity_edit->setText("0");
    ui_->mode_edit->setText("0");
    ui_->state_edit->setText("0");
    ui_->acc_edit->setText("0");
    ui_->power_edit->setText("0");

    // Задаем валидаторы для каждого из полей
    QValidator* x_validator = new QIntValidator(0, 63, this);
    ui_->x_edit->setValidator(x_validator);

    QValidator* y_validator = new QIntValidator(-32, 31, this);
    ui_->y_edit->setValidator(y_validator);

    QValidator* velocity_validator = new QIntValidator(0, 255, this);
    ui_->velocity_edit->setValidator(velocity_validator);

    QValidator* mode_and_state_validator = new QIntValidator(0, 3, this);
    ui_->mode_edit->setValidator(mode_and_state_validator);
    ui_->state_edit->setValidator(mode_and_state_validator);

    QValidator* acc_validator = new QDoubleValidator(-12.7, 12.8, 1, this);
    acc_validator->setLocale(QLocale::English); // Для восприятия валидатором точек
    ui_->acc_edit->setValidator(acc_validator);

    QValidator* power_validator = new QIntValidator(0, 130, this);
    ui_->power_edit->setValidator(power_validator);
}

uint16_t ServerWindow::GetRValue() const {
    uint16_t r = 0;

#define SPBOX_VALUE(id) ui_->bit ## id ## _spbx->value()

    // Устанавливаем значение каждого бита переменной r
    // в соответствии со значением его счетчика
    if (SPBOX_VALUE(0) == 1) { r |= 1; }
    if (SPBOX_VALUE(1) == 1) { r |= 1<<1; }
    if (SPBOX_VALUE(2) == 1) { r |= 1<<2; }
    if (SPBOX_VALUE(3) == 1) { r |= 1<<3; }
    if (SPBOX_VALUE(4) == 1) { r |= 1<<4; }
    if (SPBOX_VALUE(5) == 1) { r |= 1<<5; }
    if (SPBOX_VALUE(6) == 1) { r |= 1<<6; }
    if (SPBOX_VALUE(7) == 1) { r |= 1<<7; }
    if (SPBOX_VALUE(8) == 1) { r |= 1<<8; }
    if (SPBOX_VALUE(9) == 1) { r |= 1<<9; }
    if (SPBOX_VALUE(10) == 1) { r |= 1<<10; }
    if (SPBOX_VALUE(11) == 1) { r |= 1<<11; }
    if (SPBOX_VALUE(12) == 1) { r |= 1<<12; }
    if (SPBOX_VALUE(13) == 1) { r |= 1<<13; }
    if (SPBOX_VALUE(14) == 1) { r |= 1<<14; }
    if (SPBOX_VALUE(15) == 1) { r |= 1<<15; }

#undef SPBOX_VALUE

    return r;
}
