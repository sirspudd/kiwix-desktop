#include "settingsview.h"
#include "ui_settings.h"
#include "kiwixapp.h"
#include <kiwix/tools.h>
#include <QMessageBox>
#include <QFileDialog>
SettingsView::SettingsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    QFile file(QString::fromUtf8(":/css/_settingsManager.css"));
    file.open(QFile::ReadOnly);
    QString styleSheet = QString(file.readAll());
    ui->widget->setStyleSheet(styleSheet);
    connect(ui->serverPortSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsView::setKiwixServerPort);
    connect(ui->zoomPercentSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsView::setZoom);
    connect(ui->browseButton, &QPushButton::clicked, this, &SettingsView::browseDownloadDir);
    connect(ui->resetButton, &QPushButton::clicked, this, &SettingsView::resetDownloadDir);
    connect(ui->monitorBrowse, &QPushButton::clicked, this, &SettingsView::browseMonitorDir);
    connect(ui->monitorReset, &QPushButton::clicked, this, &SettingsView::resetMonitorDir);
    connect(KiwixApp::instance()->getSettingsManager(), &SettingsManager::downloadDirChanged, this, &SettingsView::onDownloadDirChanged);
    connect(KiwixApp::instance()->getSettingsManager(), &SettingsManager::monitorDirChanged, this, &SettingsView::onMonitorDirChanged);
    connect(KiwixApp::instance()->getSettingsManager(), &SettingsManager::zoomChanged, this, &SettingsView::onZoomChanged);
    connect(KiwixApp::instance()->getSettingsManager(), &SettingsManager::portChanged, this, &SettingsView::onServerPortChanged);
    ui->settingsLabel->setText(gt("settings"));
    ui->serverPortLabel->setText(gt("port-for-local-kiwix-server-setting"));
    ui->zoomPercentLabel->setText(gt("zoom-level-setting"));
    ui->downloadDirLabel->setText(gt("download-directory-setting"));
    ui->monitorDirLabel->setText(gt("monitor-directory-setting"));
    ui->resetButton->setText(gt("reset"));
    ui->browseButton->setText(gt("browse"));
    ui->monitorReset->setText(gt("reset"));
    ui->monitorBrowse->setText(gt("browse"));
}
void SettingsView::init(int port, int zoomPercent, const QString &downloadDir, const QString &monitorDir)
{
    ui->serverPortSpinBox->setValue(port);
    ui->zoomPercentSpinBox->setValue(zoomPercent);
    ui->downloadDirPath->setText(downloadDir);
    ui->monitorDirPath->setText(monitorDir);
}
bool SettingsView::confirmDirDialog(const QString& dir, QString messageText, QString messageTitle)
{
    auto text = messageText;
    text = text.replace("{{DIRECTORY}}", dir);
    QMessageBox msgBox(
        QMessageBox::Question, //Icon
        messageTitle, //Title
        text, //Text
        QMessageBox::Ok | QMessageBox::Cancel //Buttons
    );
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
    return (ret == QMessageBox::Ok);
}

void SettingsView::resetDownloadDir()
{
    auto dir = QString::fromStdString(kiwix::getDataDirectory());
    const auto &downloadDir = KiwixApp::instance()->getSettingsManager()->getDownloadDir();
    if (dir == downloadDir) {
        return;
    }
    if (confirmDirDialog(dir, gt("download-dir-dialog-msg"), gt("download-dir-dialog-title"))) {
        KiwixApp::instance()->getSettingsManager()->setDownloadDir(dir);
    }
}

void SettingsView::browseDownloadDir()
{
    const auto &downloadDir = KiwixApp::instance()->getSettingsManager()->getDownloadDir();
    QString dir = QFileDialog::getExistingDirectory(KiwixApp::instance()->getMainWindow(),
                                                    gt("browse-directory"),
                                                    downloadDir,
                                                    QFileDialog::ShowDirsOnly);
    if (dir == downloadDir || dir.isEmpty()) {
        return;
    }

    if (confirmDirDialog(dir, gt("download-dir-dialog-msg"), gt("download-dir-dialog-title"))) {
        KiwixApp::instance()->getSettingsManager()->setDownloadDir(dir);
    }
}

void SettingsView::browseMonitorDir()
{
    const auto &monitorDir = KiwixApp::instance()->getSettingsManager()->getMonitorDir();
    QString previousDir;
    if(monitorDir == "") {
        previousDir = KiwixApp::instance()->getSettingsManager()->getDownloadDir();
    } else {
        previousDir = monitorDir;
    }
    QString dir = QFileDialog::getExistingDirectory(KiwixApp::instance()->getMainWindow(),
                                                    gt("browse-directory"),
                                                    previousDir,
                                                    QFileDialog::ShowDirsOnly);
    if (dir == monitorDir || dir.isEmpty()) {
        return;
    }
    std::vector<std::string> paths;
    std::string path = dir.toStdString() + "/library.xml";
    paths.push_back(path);
    if (KiwixApp::instance()->getLibrary()->reloadLibrary(paths) && confirmDirDialog(dir, gt("monitor-dir-dialog-msg"), gt("monitor-dir-dialog-title"))) {
        KiwixApp::instance()->getSettingsManager()->setMonitorDir(dir);
    } else {
        QMessageBox msgBox;
        msgBox.setText(gt("monitor-directory-invalid"));
        msgBox.exec();
    }
}

void SettingsView::resetMonitorDir()
{
    auto dir = QString();
    const auto &monitorDir = KiwixApp::instance()->getSettingsManager()->getMonitorDir();
    if (dir == monitorDir) {
        return;
    }
    if (confirmDirDialog(dir, gt("monitor-reset-dir-dialog-msg"), gt("monitor-reset-dir-dialog-title"))) {
        KiwixApp::instance()->getSettingsManager()->setMonitorDir(dir);
    }
}

void SettingsView::setZoom(int zoomPercent)
{
    qreal zoomFactor = (qreal) zoomPercent/100;
    KiwixApp::instance()->getSettingsManager()->setZoomFactor(zoomFactor);
}

void SettingsView::setKiwixServerPort(int port)
{
    KiwixApp::instance()->getSettingsManager()->setKiwixServerPort(port);
}

void SettingsView::onDownloadDirChanged(const QString &dir)
{
    ui->downloadDirPath->setText(dir);
}

void SettingsView::onMonitorDirChanged(const QString &dir)
{
    ui->monitorDirPath->setText(dir);
}

void SettingsView::onZoomChanged(qreal zoomFactor)
{
    qreal zoomPercent = zoomFactor * 100;
    ui->zoomPercentSpinBox->setValue(zoomPercent);
}

void SettingsView::onServerPortChanged(int port)
{
    ui->serverPortSpinBox->setValue(port);
}
