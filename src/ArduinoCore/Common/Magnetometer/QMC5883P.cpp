/* Implementação faz uso da biblioteca desenvolvida pela AdaFruit */
#include <Magnetometer.h>
#include <Adafruit_QMC5883P.h>
#include <Logger/include/log_api.h>

#define MAGNETOMETER_INITIALIZATION_MAX_ATTEMPTS 10

static bool initialized = false;

static Adafruit_QMC5883P qmc;

/* Values acquired from calibration for QMC5883 1 */
//static int16_t Xmin = -9990;
//static int16_t Xmax = -5970;
//static int16_t Ymin = -5040; 
//static int16_t Ymax = -180;

/* Values acquired from calibration for QMC5883 2 */
static int16_t Xmin = -3225;
static int16_t Xmax = -623;
static int16_t Ymin = -3465; 
static int16_t Ymax =  975;

static float filterAlpha = 0.8;

static int16_t Xoffset = 0;
static int16_t Yoffset = 0;
static float Xfiltered = 0;
static float Yfiltered = 0;

static int16_t savedHeading_int16 = 0;

Magnetometer_Error_t Magnetometer_Create()
{
    if (initialized) return MAGNETOMETER_ERROR_ALREADY_INITIALIZED;

    uint8_t tries = 0;
    while (!qmc.begin() && tries < MAGNETOMETER_INITIALIZATION_MAX_ATTEMPTS)
    {
        tries++;
    }
    if (tries >= MAGNETOMETER_INITIALIZATION_MAX_ATTEMPTS) return MAGNETOMETER_ERROR_INITIALIZATION_FAILED;
    
    /* Calibration offset */
    Xoffset = (Xmax + Xmin) / 2;
    Yoffset = (Ymax + Ymin) / 2;

    /* Recommended configuration for calibration */
    qmc.setRange(QMC5883P_RANGE_2G);
    qmc.setOSR(QMC5883P_OSR_4);
    qmc.setDSR(QMC5883P_DSR_2);
    qmc.setODR(QMC5883P_ODR_50HZ);
    qmc.setMode(QMC5883P_MODE_CONTINUOUS);

    initialized = true;

    return MAGNETOMETER_OK;
}

Magnetometer_Error_t Magnetometer_Destroy()
{
    if (!initialized) return MAGNETOMETER_ERROR_NOT_INITIALIZED;

    qmc.softReset();
    savedHeading_int16 = 0;
    Xfiltered = 0;
    Yfiltered = 0;

    return MAGNETOMETER_OK;
}

Magnetometer_Error_t Magnetometer_NewRead()
{
    if (!initialized) return MAGNETOMETER_ERROR_NOT_INITIALIZED;

    int16_t x, y, z;

    /* Read raw magnetic field data */
    if (!qmc.getRawMagnetic(&x, &y, &z)) return MAGNETOMETER_ERROR_NEWREAD_FAILED;

    /* Apply hard-iron calibration correction */
    x -= Xoffset;
    y -= Yoffset;

    Xfiltered = Xfiltered*filterAlpha + (1-filterAlpha)*x;
    Yfiltered = Yfiltered*filterAlpha + (1-filterAlpha)*y;

    /* Heading computation */
    float heading = atan2((float)Yfiltered, (float)Xfiltered) * 180.0 / PI;

    if (heading < 0) heading += 360.0;
    savedHeading_int16 = (int16_t)heading;

    log_magnetometer_data_reading(savedHeading_int16);

    return MAGNETOMETER_OK;
}

Magnetometer_Error_t Magnetometer_GetHeading(int16_t * heading)
{
    if (!initialized) return MAGNETOMETER_ERROR_NOT_INITIALIZED;

    *heading = savedHeading_int16;
    return MAGNETOMETER_OK;
}