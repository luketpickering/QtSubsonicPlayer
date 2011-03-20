#include <QApplication>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("QtSubsonicPlayer");

    MainWindow *mainWindow = new MainWindow;
    mainWindow->show();
    return app.exec();
}
