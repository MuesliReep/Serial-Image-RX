#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Output output;
    output.initialise();

    Datastream data(&output);
    data.initialise();

    return a.exec();
}
