
#include <QApplication>

#include "Output.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Output output;
    output.show();
    return app.exec();
}
