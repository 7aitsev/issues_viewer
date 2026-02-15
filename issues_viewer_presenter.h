#pragma once

#include <QObject>
#include <QString>

#include <vector>

class QString;

class IssuesViewerPresenter : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool   load_enabled
               READ   is_load_enabled
               NOTIFY load_enabled_changed)
    Q_PROPERTY(bool   content_available
               READ   is_content_available
               NOTIFY content_available_changed)

public:
    explicit IssuesViewerPresenter(QObject *parent = nullptr);

    bool is_load_enabled() const;
    bool is_content_available() const;

    Q_INVOKABLE void on_path_changed(const QString &text);
    Q_INVOKABLE void on_load_issues(const QString &url);

signals:
    void load_enabled_changed();
    void content_available_changed();

private:
    static std::vector<QString> _request_issues(const QString &url);

private:
    std::vector<QString> _issues;

    bool _is_load_enabled = false;
    bool _is_content_available = false;
};
