#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
namespace Ui {
class Settings;
}
class SettingsView : public QWidget
{
    Q_OBJECT
public:
    SettingsView(QWidget *parent = nullptr);
    ~SettingsView(){};
    void init(int port, int zoomPercent, const QString &downloadDir, const QString &monitorDir);
public Q_SLOTS:
    void resetDownloadDir();
    void browseDownloadDir();
    void browseMonitorDir();
    void clearMonitorDir();
    void setZoom(int zoomPercent);
    void onDownloadDirChanged(const QString &dir);
    void onMonitorDirChanged(const QString &dir);
    void onZoomChanged(qreal zoomFactor);
    void onServerPortChanged(int port);
    void setKiwixServerPort(int port);
private:
    bool confirmDirDialog(const QString& dir, QString messageText, QString messageTitle);

    Ui::Settings *ui;
};

#endif // SETTINGSVIEW_H
