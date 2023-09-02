#include <QApplication>
#include "serverwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ServerWindow w;
    w.show();

    return app.exec();
}
