#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <httpserver.h>

QSettings *getSettings(QString &configPath)
{
    QSettings *settings = new QSettings(configPath, QSettings::IniFormat);

    settings->beginGroup("server");

    if (!settings->contains("port")) {
        settings->setValue("port", 80);
    }

    if (!settings->contains("document_root")) {
        settings->setValue("document_root", QCoreApplication::applicationDirPath());
    }

    settings->endGroup();

    settings->beginGroup("cache");

    if (!settings->contains("max_cached_file_size")) {
        settings->setValue("max_cached_file_size", 100);
    }

    if (!settings->contains("max_total_cache_size")) {
        settings->setValue("max_total_cache_size", 20000);
    }

    settings->endGroup();

    return settings;
}

void parseCommandLineArguments(QCoreApplication &a, QString &configPath)
{
    QCommandLineParser parser;
    QCommandLineOption configOption(QStringList() << "c" << "config", "Path to config file", "config");

    parser.setApplicationDescription("Http web server");
    parser.addHelpOption();
    parser.addOption(configOption);

    parser.process(a);

    configPath = (parser.isSet(configOption)) ? parser.value(configOption) : "settings.ini";
}

int main(int argc, char *argv[])
{
    QString configPath;
    QCoreApplication a(argc, argv);

    parseCommandLineArguments(a, configPath);

    QSettings *settings = getSettings(configPath);
    HttpServer httpServer(settings);

    if (!httpServer.start()) {
        std::cout << "Server start failed" << std::endl;
    }

    return a.exec();
}
