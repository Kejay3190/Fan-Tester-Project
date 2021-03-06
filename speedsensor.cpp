#include "speedsensor.h"
#include "cbw.h"

SpeedSensor::SpeedSensor(QObject *parent, const int &fanBladeCount, const int &maxSamples) :
    QObject(parent), bladeCount(fanBladeCount), sampleLimit(maxSamples)
{
    time = new QTime(QTime::currentTime());
    rpmSamples = new QList<double>;
}

SpeedSensor::~SpeedSensor()
{
    delete time;
    delete rpmSamples;
}

void SpeedSensor::setBladeCount(const int &newBladeCount)
{
    if (bladeCount != newBladeCount) {
        bladeCount = newBladeCount;
    }
}

void SpeedSensor::calculateRpm()
{
    cbCIn32(0, 0, &sensorCount); //read the count from the board and store it in the given variable
    rpm = (static_cast<double>(sensorCount) / bladeCount) / (time->restart() / 60000.0);
    if (rpmSamples->count() < sampleLimit) { //only store up to the number of samples set by the user
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
    clearBoardCount();
    emit rpmChanged(rpm, aveRpm);
}

void SpeedSensor::clearSamples()
{
    rpmSamples->clear();
}

void SpeedSensor::clearBoardCount()
{
    cbCClear(0, 0);
}

void SpeedSensor::setSampleLimit(const int &newSampleLimit)
{
    if (sampleLimit != newSampleLimit) {
        sampleLimit = newSampleLimit;
    }
}
