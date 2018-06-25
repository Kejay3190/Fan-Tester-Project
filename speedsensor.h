#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QList>

class SpeedSensor : public QObject
{
    Q_OBJECT

public:
    explicit SpeedSensor(QObject *parent, const int &fanBladeCount, const int &maxSamples);
    ~SpeedSensor();

private:
    unsigned long sensorCount = 0;
    double rpm = 0;
    double aveRpm = 0;
    int bladeCount = 0;
    int sampleLimit = 0;
    QTime *time;
    QList<double> *rpmSamples;

    void clearBoardCount();

signals:
    void rpmChanged(const double &rpm, const double &aveRpm);

public slots:
    void calculateRpm();
    void setSampleLimit(int newSampleLimit);
    void setBladeCount(int newBladeCount);
};

#endif // SPEEDSENSOR_H
