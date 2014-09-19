#include <QCoreApplication>
#include <QCommandLineParser>
#include <httpserver.h>

void parseCommandLineArguments(QCoreApplication &a, int &port, QString &documentRoot)
{
    QCommandLineParser parser;
    QCommandLineOption portOption(QStringList() << "p" << "port", "Set listening port to <port>", "port");
    QCommandLineOption documentRootOption(QStringList() << "r" << "document-root", "Set document root to <document-root>", "document-root");

    parser.setApplicationDescription("Http web server");
    parser.addHelpOption();
    parser.addOption(portOption);
    parser.addOption(documentRootOption);

    parser.process(a);

    port = (parser.isSet(portOption)) ? parser.value(portOption).toInt() : 80;
    documentRoot = (parser.isSet(documentRootOption)) ? parser.value(documentRootOption) : "";
}

int main(int argc, char *argv[])
{
    int port;
    QString documentRoot;
    QCoreApplication a(argc, argv);

    parseCommandLineArguments(a, port, documentRoot);

    HttpServer httpServer(port, documentRoot);

    return a.exec();
}
