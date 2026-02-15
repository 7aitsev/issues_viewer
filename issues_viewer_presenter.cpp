#include "issues_viewer_presenter.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>

IssuesViewerPresenter::IssuesViewerPresenter(QObject *parent)
    : QObject(parent), _manager(std::make_unique<QNetworkAccessManager>(this))
{
    connect(_manager.get(), &QNetworkAccessManager::finished,
            this, &IssuesViewerPresenter::_on_request_finished);
}

bool IssuesViewerPresenter::is_load_enabled() const {
    return _is_load_enabled;
}

bool IssuesViewerPresenter::is_content_available() const {
    return _is_content_available;
}

const QStringList& IssuesViewerPresenter::issues() const {
    return _issues;
}

void IssuesViewerPresenter::on_path_changed(const QString &text) {
    const auto allowed = !text.trimmed().isEmpty();
    if (_is_load_enabled != allowed) {
        _is_load_enabled = allowed;
        emit load_enabled_changed();
    }
}

void IssuesViewerPresenter::on_load_issues(const QString &url) {
    _issues.clear();

    QNetworkRequest request(url.trimmed());
    _manager->get(request);
}

void IssuesViewerPresenter::_on_request_finished(QNetworkReply *reply) {
    if (!reply) {
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        auto data = reply->readAll();
        qDebug() << "Data received:" << data.left(64) << "...";

        _parse_navigation_links(reply->rawHeader("Link"));
        _parse_issue_titles(data);
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();

    const auto is_content_available = !_issues.empty();
    if (_is_content_available != is_content_available) {
        _is_content_available = is_content_available;
        emit content_available_changed();
    }
}

void IssuesViewerPresenter::_parse_navigation_links(const QString &headers) {
    if (headers.isEmpty()) return;
}

void IssuesViewerPresenter::_parse_issue_titles(const QByteArray &data) {
    QJsonParseError error;
    const auto doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON Error:" << error.errorString();
        return;
    }

    if (!doc.isArray()) {
        qDebug() << "JSON has wrong format: expected array";
        return;
    }

    const auto issues = doc.array();
    for (const QJsonValue &item : issues) {
        if (!item.isObject()) {
            continue;
        }

        QJsonObject issue = item.toObject();
        if (issue.contains("title") && issue["title"].isString()) {
            QString title = issue["title"].toString();
            _issues.append(title);
        }
    }
}
