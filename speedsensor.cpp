#include "speedsensor.h"
#include "cbw.h"

SpeedSensor::SpeedSensor(QObject *parent, const int &fanBladeCount, const int &maxSamples) :
    QObject(parent), bladeCount(fanBladeCount), sampleLimit(maxSamples)
{
    time = new QTime(QTime::currentTime());
    rpmSamples = new QList<double>;
    clearBoardCount();
}

SpeedSensor::~SpeedSensor()
{
    delete time;
    delete rpmSamples;
}

void SpeedSensor::setBladeCount(int newBladeCount)
{
    if (bladeCount != newBladeCount) {
        bladeCount = newBladeCount;
    }
}

void SpeedSensor::clearBoardCount()
{
    cbCClear(0, 0); //clear the count on the daq board
}

void SpeedSensor::calculateRpm()
{
    cbCIn32(0, 0, &sensorCount); //read the count from the board and store it in the given variable
    rpm = (double (sensorCount) / double (bladeCount)) / (double (time->restart()) / double (60000));
    if (rpmSamples->count() < sampleLimit) {
        rpmSamples->append(rpm);
    } else {
        rpmSamples->removeFirst();
        rpmSamples->append(rpm);
    }
    double sum = 0;
    for (const double &sample : *rpmSamples) { //sum all of the rpm samples
        sum += sample;
    }
    aveRpm = sum / rpmSamples->count(); //average the rpm samples
    cbCClear(0, 0); //clear the count on the board
    emit rpmChanged(rpm, aveRpm);
}

void SpeedSensor::setSampleLimit(int newSampleLimit)
{
    if (sampleLimit != newSampleLimit) {
        sampleLimit = newSampleLimit;
    }
}
