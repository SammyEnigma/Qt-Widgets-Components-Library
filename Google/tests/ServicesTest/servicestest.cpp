#ifdef TEST_GOOGLE_SERVICES

#include <QtTest>

#include <QStringList>
#include <QEventLoop>
#include <QDebug>

#include "googledetectlanguage.h"
#include "googlespellchecker.h"
#include "googletranslator.h"
#include "googlegeocoding.h"
#include "googleweather.h"
#include "googlesuggest.h"
#include "googleauth.h"

#include "servicestest.h"

#define GOOGLE_USERNAME         "USER@gmail.com"
#define GOOGLE_PASSWORD         "PASSWORD"

ServicesTest::ServicesTest (QObject *parent)
    : QObject(parent)
{
}

ServicesTest::~ServicesTest() {
}

void ServicesTest::testAuth (void) {
    QEventLoop l;
    THGoogleAuth gAuth(GOOGLE_USERNAME);
    connect(&gAuth, SIGNAL(authenticated()), &l, SLOT(quit()));
    gAuth.login(THGoogleAuth::Contacts, GOOGLE_PASSWORD);
    l.exec();

    if (gAuth.error() == THGoogleAuth::NoError) {
        qDebug() << "SID" << gAuth.sid();
        qDebug() << "LSID" << gAuth.lsid();
        qDebug() << "AUTH" << gAuth.auth();
    } else {
        qDebug() << gAuth.errorString();
    }

    QVERIFY(1);     // Do some test...
}

void ServicesTest::testWeather (void) {
    QEventLoop l;
    THGoogleWeather gWeather;
    connect(&gWeather, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gWeather.forecast("San Mateo, CA");
    l.exec();

    qDebug() << gWeather.city() << "Forecast";
    qDebug() << "Current:" << gWeather.currentCondition();
    qDebug() << gWeather.currentHumidity();
    qDebug() << gWeather.currentWind();
    qDebug() << gWeather.currentTemperature(THGoogleWeather::Celsius);

    for (int i = THGoogleWeather::Sunday; i <= THGoogleWeather::Saturday; ++i) {
        THGoogleWeather::DayOfWeek day = (THGoogleWeather::DayOfWeek)i;
        if (!gWeather.hasForecast(day))
            continue;

        qDebug() << gWeather.dayOfWeek(day) << gWeather.forecastCondition(day);
        qDebug() << " - Low Temperature" << gWeather.forecastLowTemperature(day, THGoogleWeather::Celsius);
        qDebug() << " - High Temperature" << gWeather.forecastHighTemperature(day, THGoogleWeather::Celsius);
    }
}

void ServicesTest::testGeoCoding (void) {
    QEventLoop l;
    THGoogleGeocoding gGeoCoder;
    connect(&gGeoCoder, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gGeoCoder.request("Asola");
    l.exec();

    foreach (THGoogleGeoPlacemark placemark, gGeoCoder.placemarks()) {
        qDebug() << placemark.east() 
                 << placemark.west() 
                 << placemark.north() 
                 << placemark.south() 
                 << placemark.address() 
                 << placemark.coordinates() 
                 << placemark.countryName() 
                 << placemark.countryCode() 
                 << placemark.localityName() 
                 << placemark.postalCodeNumber() 
                 << placemark.thoroughfareName() 
                 << placemark.administrativeAreaName()
                 << placemark.subAdministrativeAreaName();
    }

    QVERIFY(1);     // Do some test...
}

void ServicesTest::testTranslator (void) {
    QEventLoop l;

    THGoogleTranslator gTranslator;
    connect(&gTranslator, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gTranslator.setDestinationLanguage("en");
    gTranslator.translate("Ciao");

    l.exec();

    qDebug() << gTranslator.query();
    qDebug() << gTranslator.translatedText();
    qDebug() << gTranslator.detectedSourceLanguage();

    QVERIFY(1);     // Do some test...
}

void ServicesTest::testSuggestions (void) {
    QEventLoop l;

    THGoogleSuggest gSuggest;
    connect(&gSuggest, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gSuggest.suggest("Qt Softw");

    l.exec();

    qDebug() << gSuggest.suggestions();

    QVERIFY(1);     // Do some test...
}

void ServicesTest::testSpellChecker (void) {
    QEventLoop l;

    THGoogleSpellChecker gSpellChecker;
    connect(&gSpellChecker, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gSpellChecker.spellCheck("Gogl Spll");

    l.exec();

    foreach (QString original, gSpellChecker.keys())
        qDebug() << original << gSpellChecker.suggestions(original);

    QVERIFY(1);     // Do some test...
}

void ServicesTest::testLanguageDetection (void) {
    QEventLoop l;

    THGoogleDetectLanguage gDetectLang;
    connect(&gDetectLang, SIGNAL(finished(bool)), &l, SLOT(quit()));
    gDetectLang.detectLanguage("Ciao");

    l.exec();

    qDebug() << "Query" << gDetectLang.query();
    qDebug() << "Language" << gDetectLang.language();
    qDebug() << "Is Reliable" << gDetectLang.isReliable();
    qDebug() << "Confidence" << gDetectLang.confidence();

    QVERIFY(1);     // Do some test...
}

QTEST_MAIN(ServicesTest)

#endif /* TEST_GOOGLE_SERVICES */

