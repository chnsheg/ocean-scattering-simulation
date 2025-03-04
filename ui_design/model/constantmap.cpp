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

// void CaculateScatteredPhotons(double energe, double Alpha_water,
//                           double Beta_particulate, double Beta_m,
//                           double lambda, double n, double z, double H,
//                           double energy_ratio, double *N_Brillouin,
//                           double *N_Mie, double *N_Rayleigh)

QVector<QString> physicalConstantVector = QVector<QString>(
    {"SpeedOfLight", "PlanckConstant", "GravitationalConstant"});
QVector<QString> laserConstantVector = QVector<QString>({"LaserWidth",
                                                         "waveLength",
                                                         "Intensity",
                                                         "frequencyRange",
                                                         "laserEnergy",
                                                         "frequencyResolution"});
QVector<QString> spectrumConstantVector = QVector<QString>(
    {"Temperature", "Salinity", "BetaParticulate", "BetaM", "WaterRefractiveIndex", "EnergyRatio"});

QVector<QString> underWaterSpectrumConstantVector = QVector<QString>(
    {"M", "AlphaWater", "detect_eff", "optical_eff", "radius", "N_dark", "Depth",
     "TelescopeHeight", "noise_beta", "sample_interval", "vertical_resolution"});

QVector<QString> fizeauConstantVector = QVector<QString>(
    {"Fizeau_lambda", "Fizeau_plate", "Fizeau_angle", "Fizeau_index", "Fizeau_albedo"});
QVector<QString> pmtConstantVector = QVector<QString>(
    {"channel_number", "channel_width", "channel_space", "poissrnd_lambda"});
QVector<QString> runtimeCurveDataVector = QVector<QString>({"Laser",                          // 0
                                                            "BrillouinSpectrum",              // 1
                                                            "MieSpectrum",                    // 2
                                                            "RayleighSpectrum",               // 3
                                                            "BrillouinSpectrumLaserEffected", // 4
                                                            "RayleighSpectrumLaserEffected",  // 5
                                                            "MieSpectrumLaserEffected",       // 6
                                                            "TotalSpectrum",                  // 7
                                                            "UnderWaterBrillouinSpectrum",    // 8
                                                            "UnderWaterRayleighSpectrum",     // 9
                                                            "UnderWaterMieSpectrum",          // 10
                                                            "UnderWaterSpectrum",             // 11
                                                            "FizeauInstrument",               // 12
                                                            "SpectrumAfterFizeau",            // 13                          // 14
                                                            "SpectrumAfterPMT"});             // 14

QVector<QString> runtimeResultDataVector = QVector<QString>({"BrillouinLine", // 用来保存运行时产生的数据
                                                             "BrillouinShift",
                                                             "NBrillouin",
                                                             "NMie",
                                                             "NRayleigh",
                                                             "SNR",
                                                             "PhotonNum",
                                                             "retrievalBrillouinShift",
                                                             "retrievalBrillouinLine",
                                                             "retrievalRayleighLine"
                                                             "retrievalTemperature",
                                                             "retrievalSalinity"});

QVector<QString> runtimeImageDataVector = QVector<QString>({"LaserImage",
                                                            "SpectrumImage",
                                                            "UnderWaterTotalImage",
                                                            "FizeauImage",
                                                            "PMTImage"});

QVector<QString> retrievalConstantVector = QVector<QString>({"initialBrillouinShift ",
                                                             "initialBrillouinLine ",
                                                             "initialRayleighLine",
                                                             "upperBrillouinShift ",
                                                             "upperBrillouinLine ",
                                                             "upperRayleighLine",
                                                             "lowerBrillouinShift ",
                                                             "lowerBrillouinLine ",
                                                             "lowerRayleighLine",
                                                             "depthNum",
                                                             "depthLower",
                                                             "depthUpper",
                                                             "Tolerance"});

ConstantGroup physicalConstantGroup(&physicalConstantVector);
ConstantGroup laserConstantGroup(&laserConstantVector);
ConstantGroup fizeauConstantGroup(&fizeauConstantVector);
ConstantGroup pmtConstantGroup(&pmtConstantVector);
ConstantGroup spectrumConstantGroup(&spectrumConstantVector);
ConstantGroup underWaterSpectrumConstantGroup(&underWaterSpectrumConstantVector);
ConstantGroup runtimeDataGroup(&runtimeCurveDataVector);
ConstantGroup runtimeResultGroup(&runtimeResultDataVector);
ConstantGroup runtimeImageGroup(&runtimeImageDataVector);
ConstantGroup retrievalConstantGroup(&retrievalConstantVector);

ConstantMap::ConstantMap()
{
    this->m_constantMap = new QMap<int, ConstantGroup *>();
    this->m_constantMap->insert(0, &laserConstantGroup);
    this->m_constantMap->insert(1, &spectrumConstantGroup);
    this->m_constantMap->insert(2, &fizeauConstantGroup);
    this->m_constantMap->insert(3, &pmtConstantGroup);
    this->m_constantMap->insert(4, &physicalConstantGroup);
    this->m_constantMap->insert(5, &runtimeDataGroup);
    this->m_constantMap->insert(6, &runtimeResultGroup);
    this->m_constantMap->insert(7, &underWaterSpectrumConstantGroup);
    this->m_constantMap->insert(8, &runtimeImageGroup);
    this->m_constantMap->insert(9, &retrievalConstantGroup);
}

ConstantMap::~ConstantMap() {}

QString ConstantMap::getConstantName(int groupIndex, int constantIndex)
{
    ConstantGroup *constant = m_constantMap->value(groupIndex); // 返回指定键对应的值
    return constant->getConstantName(constantIndex);
}
