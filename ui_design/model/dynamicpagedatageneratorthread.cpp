#include "dynamicpagedatageneratorthread.h"
#include "base/singleton.h"
#include "model/spectrumdatagenerator.h"
#include "model/underwaterspectrumdatagenerator.h"
#include "model/frequenceDataGenerator.h"

// DynamicPageDataGeneratorThread::DynamicPageDataGeneratorThread() {}

#include "retrievalthread.h"

int DynamicPageDataGeneratorThread::objectCount = 0;

DynamicPageDataGeneratorThread::DynamicPageDataGeneratorThread(int index)
    : _index(index)
{
    objectCount++;
    // PageDataGenerator *pageDataGenerator = Singleton<PageDataGenerator>::getInstance(nullptr);
    // connect(this, &TaskRunner::taskCompleted, pageDataGenerator, &PageDataGenerator::handleTaskCompletedSlot);
    qDebug() << "RetrievalThread is created, objectCount:" << objectCount;
}

void DynamicPageDataGeneratorThread::run()
{
    QString taskName = "DynamicPageDataGeneratorThread" + QString::number(objectCount);
    QVariantList *args = new QVariantList(); //(status, xDataVector, yDataVector, i, QString("SNR随深度变化"), legendList);
    qDebug() << "DynamicPageDataGeneratorThread is running in thread:" << QThread::currentThreadId();
    QVector<double> *RF;
    QVector<QVector<double> *> *xDataVector;
    QVector<QVector<double> *> *yDataVector;
    xDataVector = new QVector<QVector<double> *>;
    yDataVector = new QVector<QVector<double> *>;
    QVector<QVector<double> *> *laserLineWidthEffectData;
    QStringList legendList;
    QStringList labelList;
    PageDataGenerator *pageDataGenerator = Singleton<PageDataGenerator>::getInstance(nullptr);

    switch (_index)
    {
    case 0:
        for (int i = 0; i < 1; i++) // 总共有几个页面
        {
            if (i == 0)
            {
                laserLineWidthEffectData = SpectrumDataGenerator::generateLaserLineWidthEffectData();
                if (laserLineWidthEffectData == nullptr || laserLineWidthEffectData->size() != 4)
                {
                    args->append(0); // status=0表示数据生成失败
                    emit taskCompleted(taskName, args);
                    return;
                }
                //  FrequenceDataGenerator::generateRelativeFrequenceData();
                RF = FrequenceDataGenerator::generateRelativeFrequenceData();
                xDataVector->append(RF);
                yDataVector->append(laserLineWidthEffectData->at(0));
                yDataVector->append(laserLineWidthEffectData->at(1));
                yDataVector->append(laserLineWidthEffectData->at(2));
                yDataVector->append(laserLineWidthEffectData->at(3));
                legendList.append("布里渊散射谱");
                legendList.append("瑞利散射谱");
                legendList.append("米散射谱");
                legendList.append("整体散射谱");
                // emit dynamicDataGenerated(xDataVector, yDataVector, i, QString("受激光拓宽的散射谱"), legendList); // i 表示第几面的曲线
                args->append(1); // status=1表示单个数据生成成功
                // args->append(QVariant::fromValue(xDataVector));
                // args->append(QVariant::fromValue(yDataVector));
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*xDataVector))); // 深拷贝xDataVector
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*yDataVector))); // 深拷贝yDataVector

                args->append(i);
                args->append(QString("受激光拓宽的散射谱"));
                args->append(legendList);
                labelList.append("frequency/Hz");
                labelList.append("Intensity");
                args->append(labelList);
                emit taskCompleted(taskName, new QVariantList(*args));
            }
        }
        // 清空args
        args->clear();
        legendList.clear();
        labelList.clear();
        args->append(2); // status=2表示所有数据生成成功
        emit taskCompleted(taskName, new QVariantList(*args));
        // delete RF;
        break;
    case 1:
        for (int i = 0; i < 3; i++) // 总共有几个页面
        {
            if (i == 0)
            {
                laserLineWidthEffectData = UnderWaterSpectrumDataGenerator::generateSNRDepthByMData();
                // 取到laserLineWidthEffectData最后一个元素，以它的大小为循环次数
                xDataVector->append(laserLineWidthEffectData->at(0));
                for (int j = 0; j < laserLineWidthEffectData->last()->size(); j++)
                {
                    yDataVector->append(laserLineWidthEffectData->at(j + 1));
                    legendList.append("M = " + QString::number(laserLineWidthEffectData->last()->at(j)));
                }

                legendList.append("SNR = 15.42");
                legendList.append("SNR = 8.77");
                // emit dynamicDataGenerated(xDataVector, yDataVector, i, QString("SNR随深度变化"), legendList); // i 表示第几面的曲线
                // 查找其他曲线与y=15.42dB的交点
                // for (int j = 0; j < yDataVector->size() - 1; j++)
                // {
                //     for (int k = 0; k < xDataVector->at(0)->size(); k++)
                //     {
                //         if (yDataVector->at(j)->at(k) - 15.42 < 0.1 && yDataVector->at(j)->at(k) - 15.42 > -0.1)
                //         {
                //             // line_x->replace(k, xDataVector->at(0)->at(k));
                //             qDebug() << "SNR = 15.42:" << xDataVector->at(0)->at(k);
                //         }
                //     }
                // }
                // y = 15.42dB画一条线
                QVector<double> *line_y = new QVector<double>(xDataVector->at(0)->size(), 15.42);
                yDataVector->append(line_y);
                // y = 8.77dB画一条线
                QVector<double> *line_y2 = new QVector<double>(xDataVector->at(0)->size(), 8.77);
                yDataVector->append(line_y2);

                args->append(1); // status=1表示单个数据生成成功
                // args->append(QVariant::fromValue(xDataVector));
                // args->append(QVariant::fromValue(yDataVector));
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*xDataVector))); // 深拷贝xDataVector
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*yDataVector))); // 深拷贝yDataVector

                args->append(i);
                args->append(QString("SNR随深度变化"));
                args->append(legendList);
                labelList.append("depth/m");
                labelList.append("SNR/dB");
                args->append(labelList);
                // emit taskCompleted(taskName, new QVariantList(*args));
                pageDataGenerator->handleTaskCompletedSlot(taskName, args);
            }
            else if (i == 1)
            {
                // delete xDataVector->at(0);
                // delete yDataVector->at(0);
                xDataVector = new QVector<QVector<double> *>;
                yDataVector = new QVector<QVector<double> *>;
                laserLineWidthEffectData = UnderWaterSpectrumDataGenerator::generateNsMByDepthData();
                // 取到laserLineWidthEffectData最后一个元素，以它的大小为循环次数
                xDataVector->append(laserLineWidthEffectData->at(0));
                // 先清空legendList
                legendList.clear();
                labelList.clear();
                for (int j = 0; j < laserLineWidthEffectData->last()->size(); j++)
                {
                    // qDebug() << "laserLineWidthEffectData->last()->at(j):" << laserLineWidthEffectData->last()->at(j);
                    // qDebug() << "laserLineWidthEffectData->at(0)->size():" << laserLineWidthEffectData->at(0)->size();
                    yDataVector->append(laserLineWidthEffectData->at(j + 1));
                    legendList.append("M = " + QString::number(laserLineWidthEffectData->last()->at(j)));
                }
                legendList.append("Ns = 3089.58");
                // emit dynamicDataGenerated(xDataVector, yDataVector, i, QString("Ns/M随深度变化"), legendList); // i 表示第几面的曲线

                // for (int j = 0; j < yDataVector->size() - 1; j++)
                // {
                //     for (int k = 0; k < xDataVector->at(0)->size(); k++)
                //     {
                //         if (yDataVector->at(j)->at(k) == 3089.58)
                //         {
                //             // line_x->replace(k, xDataVector->at(0)->at(k));
                //             qDebug() << "Ns = 3089.58:" << xDataVector->at(0)->at(k);
                //         }
                //     }
                // }

                QVector<double> *line_y = new QVector<double>(xDataVector->at(0)->size(), 3089.58);
                yDataVector->append(line_y);

                args->clear();
                args->append(1);                                                                 // status=1表示单个数据生成成功
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*xDataVector))); // 深拷贝xDataVector
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*yDataVector))); // 深拷贝yDataVector
                args->append(i);
                args->append(QString("Ns/M随深度变化"));
                args->append(legendList);

                labelList.append("depth/m");
                labelList.append("Ns");
                args->append(labelList);
                // emit taskCompleted(taskName, new QVariantList(*args));
                pageDataGenerator->handleTaskCompletedSlot(taskName, args);
            }
            else if (i == 2)
            {
                // delete xDataVector->at(0);
                // delete yDataVector->at(0);

                xDataVector = new QVector<QVector<double> *>;
                yDataVector = new QVector<QVector<double> *>;
                laserLineWidthEffectData = UnderWaterSpectrumDataGenerator::generateSNRDepthByAlphaData();
                // 取到laserLineWidthEffectData最后一个元素，以它的大小为循环次数
                xDataVector->append(laserLineWidthEffectData->at(0));
                // 先清空legendList
                legendList.clear();
                labelList.clear();
                for (int j = 0; j < laserLineWidthEffectData->last()->size(); j++)
                {
                    yDataVector->append(laserLineWidthEffectData->at(j + 1));
                    legendList.append("Alpha = " + QString::number(laserLineWidthEffectData->last()->at(j)));
                }

                legendList.append("SNR = 15.42");
                legendList.append("SNR = 8.77");

                QVector<double> *line_y = new QVector<double>(xDataVector->at(0)->size(), 15.42);
                yDataVector->append(line_y);
                QVector<double> *line_y2 = new QVector<double>(xDataVector->at(0)->size(), 8.77);
                yDataVector->append(line_y2);

                // emit dynamicDataGenerated(xDataVector, yDataVector, i, QString("不同alpha下SNR随深度变化"), legendList); // i 表示第几面的曲线
                args->clear();
                args->append(1);                                                                 // status=1表示单个数据生成成功
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*xDataVector))); // 深拷贝xDataVector
                args->append(QVariant::fromValue(new QVector<QVector<double> *>(*yDataVector))); // 深拷贝yDataVector
                args->append(i);
                args->append(QString("不同alpha下SNR随深度变化"));
                args->append(legendList);
                labelList.append("depth/m");
                labelList.append("SNR/dB");
                args->append(labelList);
                // emit taskCompleted(taskName, new QVariantList(*args));
                pageDataGenerator->handleTaskCompletedSlot(taskName, args);
            }
        }
        // 清空args
        args->clear();
        args->append(2); // status=2表示所有数据生成成功
        // emit taskCompleted(taskName, new QVariantList(*args));
        pageDataGenerator->handleTaskCompletedSlot(taskName, args);

        delete xDataVector;
        delete yDataVector;
        delete laserLineWidthEffectData;
        delete args;

        break;
    }
}
