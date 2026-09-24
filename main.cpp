#include <QtWidgets/qapplication.h>
#include <QtWidgets/qpushbutton.h>

int main(int argc, char** argv) {
    QApplication myApp(argc, argv);

    QPushButton myButton("Hello World");
    myButton.show();

    return myApp.exec();
}