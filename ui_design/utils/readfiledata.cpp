#include "utils/readfiledata.h"

ReadFileData::ReadFileData() {}

void ReadFileData::readCSV(QVector<double> *data, QString filename)
{
    QFile inFile(":/" + filename);
    QStringList lines; /*行数据*/
    if (inFile.open(QIODevice::ReadOnly)) {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd()) {
            lines.push_back(stream_text.readLine());
        }
        for (int j = 0; j < lines.size(); j++) {
            QString line = lines.at(j);
            QStringList split = line.split(","); /*列数据*/
            for (int col = 0; col < split.size(); col++) {
                data->push_back(split.at(col).toDouble());
                // qDebug() << split.at(col).toDouble() << "\t";
            }
        }
        inFile.close();
    } else {
        qDebug() << "open file failed";
    }
}
