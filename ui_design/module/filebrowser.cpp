#include "filebrowser.h"
#include <qdebug.h>

FileBrowser::FileBrowser(QWidget *parent)
    : QWidget(parent)
{
}

QStringList FileBrowser::showFileDialog(QFileDialog::AcceptMode mode,
                                        const QString &defaultPath,
                                        const QString &filter,
                                        QString &defaultSuffix,
                                        bool allowMultipleFiles)
{
    QFileDialog dialog;
    dialog.setAcceptMode(mode); // 设置对话框模式, 保存或者打开,QFileDialog::AcceptSave或者QFileDialog::AcceptOpen

    // 设置文件模式, 可以选择多个文件,QFileDialog::ExistingFiles 或者 QFileDialog::ExistingFile
    if (allowMultipleFiles)
    {
        dialog.setFileMode(QFileDialog::ExistingFiles);
    }
    else
    {
        dialog.setFileMode(QFileDialog::ExistingFile);
    }

    dialog.setDirectory(defaultPath);       // 设置默认路径, 也可以设置为当前路径
    dialog.setNameFilter(filter);           // 设置文件过滤器, 只显示指定类型的文件
    dialog.setDefaultSuffix(defaultSuffix); // 设置默认后缀名, 保存文件时默认的后缀名

    QStringList filePaths;

    if (mode == QFileDialog::AcceptSave)
    {
        QString selectedFilePath = dialog.getSaveFileName(this, tr("Save File"), defaultPath, filter, &defaultSuffix);
        if (!selectedFilePath.isEmpty())
        {
            // qDebug() << "selectedFilePath: " << selectedFilePath;
            filePaths.append(selectedFilePath);
            return filePaths;
        }
    }
    else if (mode == QFileDialog::AcceptOpen)
    {
        QStringList selectedFilesPath = dialog.getOpenFileNames(this, tr("Open File"), defaultPath, filter);
        if (!selectedFilesPath.isEmpty())
        {
            return selectedFilesPath;
        }
    }

    return QStringList(); // 返回空列表表示用户取消操作
}

QStringList FileBrowser::saveJsonFilesDialog(const QString &defaultPath, bool allowMultipleFiles)
{
    QStringList filePaths = showFileDialog(QFileDialog::AcceptSave,
                                           defaultPath,
                                           "JSON Files (*.json)",
                                           QString("json"),
                                           allowMultipleFiles);

    if (!filePaths.isEmpty())
    {
        // 发送保存完成的信号
        emit filesSaved(filePaths);
    }

    return filePaths;
}

QStringList FileBrowser::openJsonFilesDialog(const QString &defaultPath, bool allowMultipleFiles)
{
    QStringList filePaths = showFileDialog(QFileDialog::AcceptOpen,
                                           defaultPath,
                                           "JSON Files (*.json)",
                                           QString("json"),
                                           allowMultipleFiles);

    if (!filePaths.isEmpty())
    {
        // 发送导入完成的信号
        emit filesImported(filePaths); // 发送导入完成的信号，收到后可以进行遍历处理
    }

    return filePaths;
}

QStringList FileBrowser::saveCsvFilesDialog(const QString &defaultPath, bool allowMultipleFiles)
{
    QStringList filePaths = showFileDialog(QFileDialog::AcceptSave,
                                           defaultPath,
                                           "CSV Files (*.csv)",
                                           QString("csv"),
                                           allowMultipleFiles);

    if (!filePaths.isEmpty())
    {
        // 先清空文件
        QFile file(filePaths[0]);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file.close();
        }
        else // 文件打开失败
        {
            qWarning() << "Failed to open file for writing:" << filePaths[0];
            return QStringList();
        }
        // 发送保存完成的信号
        emit filesSaved(filePaths);
    }

    return filePaths;
}

QStringList FileBrowser::openCsvFilesDialog(const QString &defaultPath, bool allowMultipleFiles)
{
    QStringList filePaths = showFileDialog(QFileDialog::AcceptOpen,
                                           defaultPath,
                                           "CSV Files (*.csv)",
                                           QString("csv"),
                                           allowMultipleFiles);

    if (!filePaths.isEmpty())
    {
        // 发送导入完成的信号
        emit filesImported(filePaths);
    }

    return filePaths;
}

void FileBrowser::handleFileDialogAccepted(const QStringList &filePaths)
{
    // 处理文件对话框的槽函数
    if (!filePaths.isEmpty())
    {
        emit filesImported(filePaths);
    }
}