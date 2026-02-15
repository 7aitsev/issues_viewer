#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

class QString;

class IssuesViewerPresenter : public QObject {
    Q_OBJECT

    // clang-format off
    Q_PROPERTY(bool   load_enabled
               READ   is_load_enabled
               NOTIFY load_enabled_changed)
    Q_PROPERTY(bool   content_available
               READ   is_content_available
               NOTIFY content_available_changed)
    Q_PROPERTY(bool   previous_page_available
               READ   is_previous_page_available
               NOTIFY previous_page_available_changed)
    Q_PROPERTY(bool   next_page_available
               READ   is_next_page_available
               NOTIFY next_page_available_changed)
    Q_PROPERTY(QStringList issues
               READ        issues
               NOTIFY      issues_changed)
    Q_PROPERTY(QString pagination_text
               READ    pagination_text
               NOTIFY  pagination_text_changed)
    // clang-format on

public:
    explicit IssuesViewerPresenter(QObject *parent = nullptr);

    bool is_load_enabled() const;
    bool is_content_available() const;
    bool is_previous_page_available() const;
    bool is_next_page_available() const;
    const QStringList &issues() const;
    QString pagination_text() const;

    Q_INVOKABLE void on_path_changed(const QString &text);
    Q_INVOKABLE void on_load_issues(const QString &url);
    Q_INVOKABLE void on_load_previous_page();
    Q_INVOKABLE void on_load_next_page();

signals:
    void load_enabled_changed();
    void content_available_changed();
    void previous_page_available_changed();
    void next_page_available_changed();
    void issues_changed();
    void pagination_text_changed();

private:
    void _on_request_finished(QNetworkReply *reply);
    void _parse_navigation_links(const QString &headers);
    void _parse_issue_titles(const QByteArray &data);

private:
    std::unique_ptr<QNetworkAccessManager> _manager;

    QStringList _issues;
    QString _previous_page_path;
    QString _next_page_path;

    uint16_t _current_page = 0;
    uint16_t _total_pages = 0;
    bool _is_load_enabled = false;
    bool _is_content_available = false;
};
