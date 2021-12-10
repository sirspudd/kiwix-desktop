#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include "settingsview.h"

class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int kiwixServerPort READ getKiwixServerPort NOTIFY portChanged)
    Q_PROPERTY(qreal zoomFactor MEMBER m_zoomFactor WRITE setZoomFactor NOTIFY zoomChanged)
    Q_PROPERTY(QString downloadDir MEMBER m_downloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)

public:
    explicit SettingsManager(QObject *parent = nullptr);
    virtual ~SettingsManager() {};

    SettingsView* getView();
    void setSettings(const QString &key, const QVariant &value);
    void deleteSettings(const QString &key);
    bool settingsExists(const QString &key);
    QVariant getSettings(const QString &key);
    qreal getZoomFactorByZimId(const QString &id);

public slots:
    void setKiwixServerPort(int port);
    int getKiwixServerPort() { return m_kiwixServerPort; };
    void setZoomFactor(qreal zoomFactor);
    qreal getZoomFactor() { return m_zoomFactor; };
    bool setDownloadDir(QString downloadDir);
    QString getDownloadDir() { return m_downloadDir; }
    bool setMonitorDir(QString monitorDir);
    QString getMonitorDir() { return m_monitorDir; }
private:
    void initSettings();

signals:
    void portChanged(int port);
    void zoomChanged(qreal zoomFactor);
    void downloadDirChanged(QString downloadDir);
    void monitorDirChanged(QString monitorDir);

private:
    QSettings m_settings;
    SettingsView *m_view;
    int m_kiwixServerPort;
    qreal m_zoomFactor;
    QString m_downloadDir;
    QString m_monitorDir;
};

#endif // SETTINGSMANAGER_H
