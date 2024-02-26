#include "constantmap.h"

ConstantGroup::ConstantGroup(QVector<QString> *_constants)
{
    this->constants = _constants;
}

ConstantGroup::ConstantGroup(const ConstantGroup &)
{
    // This is a private copy constructor, so it can't be called.
}

ConstantGroup::ConstantGroup()
{
}

QVector<QString> physicalConstantVector = QVector<QString>({"SpeedOfLight", "PlanckConstant", "GravitationalConstant"});
QVector<QString> laserConstantVector = QVector<QString>({"LaserWidth", "waveLength", "Intensity", "frequencyRange", "C"});
QVector<QString> spectrumConstantVector = QVector<QString>({"Temperature", "Salinity", "BrillouinLine", "BrillouinShift"});
QVector<QString> fizeauConstantVector = QVector<QString>({"Wavelength1", "Power1", "PulseDuration1", "PulseEnergy1"});
QVector<QString> pmtConstantVector = QVector<QString>({"QuantumEfficiency", "DarkCountRate", "Gain", "NoiseEquivalentPower"});
QVector<QString> runtimeDataVector = QVector<QString>({"Laser", "BrillouinSpectrum", "MieSpectrum", "RayleighSpectrum", "UnderWaterSpectrum", "FizeauInstrument", "SpectrumAfterFizeau", "PMT", "SpectrumAfterPMT"});

ConstantGroup physicalConstantGroup(&physicalConstantVector);
ConstantGroup laserConstantGroup(&laserConstantVector);
ConstantGroup fizeauConstantGroup(&fizeauConstantVector);
ConstantGroup pmtConstantGroup(&pmtConstantVector);
ConstantGroup spectrumConstantGroup(&spectrumConstantVector);
ConstantGroup runtimeDataGroup(&runtimeDataVector);

ConstantMap::ConstantMap()
{
    this->m_constantMap = new QMap<int, ConstantGroup *>();
    this->m_constantMap->insert(0, &laserConstantGroup);
    this->m_constantMap->insert(1, &spectrumConstantGroup);
    this->m_constantMap->insert(2, &fizeauConstantGroup);
    this->m_constantMap->insert(3, &pmtConstantGroup);
    this->m_constantMap->insert(4, &physicalConstantGroup);
    this->m_constantMap->insert(5, &runtimeDataGroup);
}

ConstantMap::~ConstantMap() {}

QString ConstantMap::getConstantName(int groupIndex, int constantIndex)
{
    ConstantGroup *constant = m_constantMap->value(groupIndex); // 返回指定键对应的值
    return constant->getConstantName(constantIndex);
}