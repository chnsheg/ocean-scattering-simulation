#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QObject>
#include <QFileDialog>
#include <QStringList>

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = nullptr);
    // 保存 JSON 文件
    QStringList saveJsonFilesDialog(const QString &defaultPath = QDir::currentPath(), bool allowMultipleFiles = false); // 默认为当前路径

    // 导入 JSON 文件
    QStringList openJsonFilesDialog(const QString &defaultPath = QDir::currentPath(), bool allowMultipleFiles = false);

    // 保存 CSV 文件
    QStringList saveCsvFilesDialog(const QString &defaultPath = QDir::currentPath(), bool allowMultipleFiles = false);

    // 导入 CSV 文件
    QStringList openCsvFilesDialog(const QString &defaultPath = QDir::currentPath(), bool allowMultipleFiles = false);

signals:
    // 保存文件完成的信号
    void filesSaved(const QStringList &filePaths);

    // 导入文件完成的信号
    void filesImported(const QStringList &filePaths);

private slots:
    // 处理文件对话框的槽函数
    void handleFileDialogAccepted(const QStringList &filePaths);

private:
    // 显示文件对话框的函数
    QStringList showFileDialog(QFileDialog::AcceptMode mode,
                               const QString &defaultPath,
                               const QString &filter,
                               QString &defaultSuffix,
                               bool allowMultipleFiles = false);
};

#endif // FILEBROWSER_H
