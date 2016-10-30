#include <QApplication>

#include "MainWidget.h"
#include "src/setting.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    if(argc == 2) {
        getConfigueParam(argv[1]);
    }

    else {
        getConfigueParam();
    }

    MainWidget w;
    w.show();
    return app.exec();
}
