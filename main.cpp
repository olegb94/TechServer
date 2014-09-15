#include <QCoreApplication>
#include <httpserver.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServer server;

    return a.exec();
}
