#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <QObject>
#include <QString>
#include <QDebug>
#include <QQmlContext>

#include "issues_viewer_presenter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    IssuesViewerPresenter presenter;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("presenter", &presenter);

    QQuickStyle::setStyle("Material");
    const QUrl url(QStringLiteral("qrc:/issues_viewer/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
