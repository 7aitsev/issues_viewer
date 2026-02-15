#include "issues_viewer_presenter.h"

#include <QDebug>

#include <future>
#include <thread>

IssuesViewerPresenter::IssuesViewerPresenter(QObject *parent)
    : QObject(parent)
{
}

bool IssuesViewerPresenter::is_load_enabled() const {
    return _is_load_enabled;
}

bool IssuesViewerPresenter::is_content_available() const {
    return _is_content_available;
}

void IssuesViewerPresenter::on_path_changed(const QString &text) {
    const auto allowed = !text.trimmed().isEmpty();
    if (_is_load_enabled != allowed) {
        _is_load_enabled = allowed;
        emit load_enabled_changed();
    }
}

void IssuesViewerPresenter::on_load_issues(const QString &url) {
    qDebug() << "Presenter is processing URL:" << url;

    auto future = std::async(std::launch::deferred, _request_issues, std::ref(url));

    try {
        _issues = std::move(future.get());
    } catch (...) {
    }

    _is_content_available = !_is_content_available;
    emit content_available_changed();
}

std::vector<QString> IssuesViewerPresenter::_request_issues(const QString &url) {

}
