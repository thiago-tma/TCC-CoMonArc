/* Implementação faz uso da biblioteca desenvolvida pela AdaFruit */
#include <Magnetometer.h>
#include <Adafruit_QMC5883P.h>
#include <Logger/include/log_api.h>

Adafruit_QMC5883P qmc;

/* Values acquired from calibration for QMC5883 1 */
//static int16_t Xmin = -9990;
//static int16_t Xmax = -5970;
//static int16_t Ymin = -5040; 
//static int16_t Ymax = -180;

/* Values acquired from calibration for QMC5883 2 */
static int16_t Xmin = -3413;
static int16_t Xmax = 6592;
static int16_t Ymin = -8963; 
static int16_t Ymax = 1252;

static int16_t Xoffset = 0;
static int16_t Yoffset = 0;

static float savedHeading = 0;

void Magnetometer_Create()
{

    if (!qmc.begin())
    {
        while(1);
    }
    
    /* Calibration offset */
    Xoffset = (Xmax + Xmin) / 2;
    Yoffset = (Ymax + Ymin) / 2;

    /* Recommended configuration for calibration */
    qmc.setRange(QMC5883P_RANGE_2G);
    qmc.setOSR(QMC5883P_OSR_4);
    qmc.setDSR(QMC5883P_DSR_2);
    qmc.setODR(QMC5883P_ODR_50HZ);
    qmc.setMode(QMC5883P_MODE_CONTINUOUS);
}

void Magnetometer_Destroy()
{
    qmc.softReset();
    savedHeading = 0;
}

void Magnetometer_NewRead()
{
    int16_t x, y, z;

    /* Read raw magnetic field data */
    qmc.getRawMagnetic(&x, &y, &z);

    /* Apply hard-iron calibration correction */
    x -= Xoffset;
    y -= Yoffset;
    
    /* Heading computation */
    float heading = atan2((float)y, (float)x) * 180.0 / PI;
    if (heading < 0) heading += 360.0;

    savedHeading = (int16_t)heading;
    log_magnetometer_data_reading(heading);
}

void Magnetometer_GetHeading(int16_t * heading)
{
    *heading = savedHeading;
}