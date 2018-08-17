#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QList>

class SpeedSensor : public QObject
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit SpeedSensor(QObject *parent, const int &fanBladeCount, const int &maxSamples);
    ~SpeedSensor();
    void clearSamples();
    void clearBoardCount();

private:
    unsigned long sensorCount = 0; //how many blades were seen by the sensor
    double rpm = 0;
    double aveRpm = 0;
    int bladeCount = 0; //the number of blades on the fan being tested
    int sampleLimit = 0; //the max number of samples to average
    QTime *time;
    QList<double> *rpmSamples;

signals:
    void rpmChanged(const double &aveRpm, const double &rpm);

public slots:
    void calculateRpm();
    void setSampleLimit(const int &newSampleLimit);
    void setBladeCount(const int &newBladeCount);
};

#endif // SPEEDSENSOR_H
