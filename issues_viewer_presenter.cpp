#include "issues_viewer_presenter.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

IssuesViewerPresenter::IssuesViewerPresenter(QObject *parent)
    : QObject(parent), _manager(std::make_unique<QNetworkAccessManager>(this)) {
    connect(_manager.get(), &QNetworkAccessManager::finished, this, &IssuesViewerPresenter::_on_request_finished);
}

bool IssuesViewerPresenter::is_load_enabled() const { return _is_load_enabled; }

bool IssuesViewerPresenter::is_content_available() const { return _is_content_available; }

bool IssuesViewerPresenter::is_previous_page_available() const { return !_previous_page_path.isEmpty(); }

bool IssuesViewerPresenter::is_next_page_available() const { return !_next_page_path.isEmpty(); }

const QStringList &IssuesViewerPresenter::issues() const { return _issues; }

QString IssuesViewerPresenter::pagination_text() const {
    if (_total_pages) {
        return QString("%1 / %2").arg(_current_page + 1).arg(_total_pages + 1);
    }
    return QString();
}

void IssuesViewerPresenter::on_path_changed(const QString &text) {
    const auto allowed = !text.trimmed().isEmpty();
    if (_is_load_enabled != allowed) {
        _is_load_enabled = allowed;
        emit load_enabled_changed();
    }
    if (_is_content_available) {
        _is_content_available = false;
        _current_page = 0;
        _total_pages = 0;
        emit content_available_changed();
    }
}

void IssuesViewerPresenter::on_load_issues(const QString &url) {
    if (url.isEmpty()) {
        return;
    }

    _issues.clear();
    _previous_page_path.clear();
    _next_page_path.clear();

    QNetworkRequest request(url.trimmed());
    _manager->get(request);
}

void IssuesViewerPresenter::on_load_previous_page() {
    _current_page -= 1;
    on_load_issues(QString(_previous_page_path));
}

void IssuesViewerPresenter::on_load_next_page() {
    _current_page += 1;
    on_load_issues(QString(_next_page_path));
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
    emit issues_changed();
    emit previous_page_available_changed();
    emit next_page_available_changed();
    emit pagination_text_changed();
}

void IssuesViewerPresenter::_parse_navigation_links(const QString &headers) {
    if (headers.isEmpty())
        return;

    QRegularExpression prev_mask("<(.+)>; rel=\"prev\"");
    QRegularExpression next_mask("<(.+)>; rel=\"next\"");

    auto match = prev_mask.match(headers);
    if (match.hasMatch()) {
        _previous_page_path = match.captured(1);
    }

    match = next_mask.match(headers);
    if (match.hasMatch()) {
        _next_page_path = match.captured(1);

        if (_current_page + 1 > _total_pages) {
            _total_pages = _current_page + 1;
        }
    }
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
