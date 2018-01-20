#if defined(ARDUINO)
  SYSTEM_MODE(MANUAL);
#endif

// EEPROM management
#include "eeprom.h"

// NTP time
#include <SparkTime.h>

// MQ-135 (GAS)
#include "mq135.h"

// BMP-180 (pressure/temp)
#include <Wire.h>
#include "i2c.h"
#include "i2c_BMP280.h"

// BH-1750 (light)
#include "BH1750.h"

// DHT22 (humidity/temp)
#include "PietteTech_DHT.h"

// e-Ink display
#include "unicorn_helmet_ink.h"

// HC-SR04 (ultasonic)
#include "hc_sr04.h"

// utility functions
#include "utility.h"

#define DHTTYPE DHT22               // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN D15                  // Digital pin for DHT22
#define PIN_MQ135 A6                // Anlog pin for GAS sensor
#define PIN_TRCT A0                 // Anlog pin for IR sensor
#define PIN_BATTERY_MEASURE A2      // Pin to read buttery current
#define PIN_BATTERY_ON A3           // Pin to turn on battery measurement
#define PIN_SENSOR_ON A4            // Pin to turn on sensros
#define HAS_RED_COLOR
#define IR_PROBE_DELAY 5
#define UDP_PORT 8888

//declarations
void sendResult();
void checkEnvironment();
int get_snow(int n = 40);
void connectWiFi();
void get_pressure(SingleResult& result);
bool get_dht_status();
void start_dht_acquire();
void get_humidity(SingleResult& result, SingleResult& prev_result, bool& performed);
void draw_bar_chart(GxEPD_Class& ink, int x, int y, float* data, int data_size);
void draw_pressure_chart(SingleResult& result);
void updateInk(SingleResult& result);
void get_battery_level(SingleResult& result);
void dht_wrapper(); // must be declared before the lib initialization

static const uint8_t DC    = D6;
static const uint8_t RST   = D7;
IPAddress UDP_ADDR(139, 59, 177, 138);

UDP Udp;
BMP280 bmp;
MQ135 mq = MQ135(PIN_MQ135);
UDP UDPClient;
SparkTime rtc;
BH1750 bh;
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t b
GxIO_Class io(SPI1, D5, DC);
GxEPD_Class display(io, RST, D3);
Ultrasonic ultrasonic(A1, 12);

SingleResult current, previous;
int led2 = D7;
bool bmp_online = false;
bool mq_online = false;
bool bh_online = false;
bool dht_online = false;
bool ink_online = false;
unsigned long currentTime;
unsigned long lastTime = 0UL;
bool bDHTstarted;
bool con = false;


//Timer timer(30000, checkEnvironment);

void setup(){
    pinMode(PIN_TRCT, INPUT);
    pinMode(PIN_BATTERY_ON, OUTPUT);
    digitalWrite(PIN_BATTERY_ON, LOW);
    pinMode(PIN_SENSOR_ON, OUTPUT);
    digitalWrite(PIN_SENSOR_ON, LOW);
    pinMode(PIN_BATTERY_MEASURE, INPUT);

    Serial.begin(115200);
    Wire.begin();
    display.init();
    rtc.begin(&UDPClient, "north-america.pool.ntp.org");
    rtc.setTimeZone(1); // gmt offset

    if (bmp.initialize()) {
        Particle.publish("DEBUG", "BMP280 sensor found...");
        bmp_online = true;
    } else {
        Particle.publish("WARN", "Could not find a valid BMP280 sensor, check wiring!");
    }
    if (mq.getRZero() > 1) {
        Particle.publish(String(mq.getRZero()), "MQ135 sensor found...");
        mq_online = true;
    } else {
        Particle.publish("WARN", "Could not find a valid MQ135 sensor, check wiring!");
    }
    if (bh.begin(BH1750::ONE_TIME_HIGH_RES_MODE)){
        Particle.publish("DEBUG", "BH1750 sensor found...");
        bh_online = true;
    }
    Particle.publish(String(analogRead(DHTPIN)), "DHT22 sensor found...");
    if (get_dht_status()){
        Particle.publish(String(analogRead(DHTPIN)), "DHT22 sensor found...");
        dht_online = true;
    }

    //timer.start();
    Particle.publish("DEBUG", "started!");

}

void loop(){
    delay(10000);
    if (!con) {
        connectWiFi();
        con = true;
    }
    //int ad_value;
    //ad_value = analogRead(A0);
    //Particle.publish("ad_value", String(ad_value));

    //float temp = 22.0;
    //String results = ultrasonic.get_n_readings_pct(temp, 10);
    //Serial.println(results);
    //display.fillRect(0, 0, 200, 200, GxEPD_WHITE);
    //display.setTextColor(GxEPD_BLACK);
    //display.setCursor(0, 0);
    //display.println(results);
    //display.update();
    //Particle.publish("results", results);
    checkEnvironment();
    sendResult();
    //
    //WiFi.off();
}

void connectWiFi() {
    String known[4][2] = {
        { "Dragon IX", "00000000"},
        { "LittleDragon", "00000000"},
        { "DARKOPL_2", "00000000" },
        { "SMOKI_SOKOLEC", "00000000" }
    };
    WiFi.on();
    WiFiAccessPoint aps[20];
    int found = WiFi.scan(aps, 20);
    if (found > 0) {
        for (int i = 0; i < found; i++) {
            Serial.println(aps[i].ssid);
            for (int j = 0; j < 3; j++) {
                if(String(aps[i].ssid) == known[j][0]) {
                      Serial.println("MATCH!");
                      WiFi.setCredentials(known[j][0].c_str(),known[j][1].c_str());
                      WiFi.connect();
                      while ( WiFi.connecting()) {
                        // print dots while we wait to connect
                        Serial.print(".");
                        delay(300);
                      }
                      Serial.println("\nYou're connected to the network");
                      break;
                }
            }
        }
    }
    Particle.connect();
    Particle.process();

}

void sendResult() {
    delay(1000);
    Udp.begin(UDP_PORT);

    unsigned long timestamp;
    char buff[32] = {0};
    char message[32] = {0};

    UnicornProto packet;
    strcpy(packet.device, System.deviceID().c_str());
    Serial.println(packet.device);

    packet.timestamp = current.timestamp;
    packet.temperature = current.temperature;
    packet.humidity = current.humidity;
    packet.light = current.light;
    packet.pressure = current.pressure;
    packet.rain_intensity = current.rain_intensity;
    packet.snow_intensity = current.snow_intensity;
    packet.CO2 = current.CO2;
    packet.battery = current.battery;

    //Sending Side
    char b[sizeof(packet)];
    memcpy(b, &packet, sizeof(packet));
    if (Udp.sendPacket(b, sizeof(packet), UDP_ADDR, UDP_PORT) < 0) {
        Particle.publish("UDP Error");
    }
    //unsigned char* ptr= (unsigned char*)&packet;
    //if (Udp.sendPacket(ptr, sizeof(packet), UDP_ADDR, UDP_PORT) < 0) {
    //    Particle.publish("UDP Error");
    //}
    //Udp.beginPacket(UDP_ADDR, UDP_PORT);
    //unsigned char* ptr= (unsigned char*)&current;
    //int sent = Udp.write(ptr, sizeof(current));
    //Particle.publish("UDP sent bytes", String(sent));
    //Udp.endPacket();
    delay(1000);
    int count = Udp.receivePacket((byte*)message, 127);
    memcpy(&timestamp, message, sizeof(timestamp));
    Particle.publish("UDP timestamp", String(timestamp));
    if (Udp.parsePacket() > 0) {
        // Read timestamp from response
        Udp.read(buff, sizeof(timestamp));
        //buff = htonl(buff);
        memcpy(&timestamp, buff, sizeof(timestamp));
        // Ignore other chars
        while(Udp.available())
            Udp.read();
    }
    Udp.stop();
}

void checkEnvironment() {
    digitalWrite(PIN_SENSOR_ON, HIGH);
    start_dht_acquire();
    //update time
    //currentTime = rtc.now();
    //Particle.publish("Measurment time", rtc.ISODateString(currentTime));
    previous = get_nth_result(0);
    if (!previous.timestamp){
        // make some inital assumptions
        previous.humidity = 25.0;
        previous.temperature = 21.0;
    }
    String prev = "T:" + String(previous.temperature) + " H:" + String(previous.humidity) + " P:" + String(previous.pressure);
    prev += " A" + String(previous.altitude) + " C:" + String(previous.CO2)  + " L:" + String(previous.light)  + " R:" + String(previous.timestamp);
    Particle.publish("previous", prev);

    SingleResult previous2 = get_nth_result(2);
    String prev2 = "T:" + String(previous2.temperature) + " H:" + String(previous2.humidity) + " P:" + String(previous2.pressure);
    prev2 += " A" + String(previous2.altitude) + " C:" + String(previous2.CO2)  + " L:" + String(previous2.light)  + " R:" + String(previous2.timestamp);
    Particle.publish("previous2", prev2);
    currentTime = rtc.now();
    current.timestamp = rtc.nowEpoch();

    // check battery level
    get_battery_level(current);

    // pressure + temp
    if (bmp_online) {
        get_pressure(current);
        Particle.publish("environment/temperature", String(current.temperature));
        Particle.publish("environment/pressure", String(current.pressure));
        Particle.publish("environment/altitude", String(current.altitude));
        delay(1000);
    } else {
        current.temperature = previous.temperature;
    }
    // humidity + temp
    bool dht_done = false;
    if (bmp_online) {
        get_humidity(current, previous, dht_done);
        Particle.publish("dht22/temperature", String(current.humidity));
        Particle.publish("dht22/humidity", String(current.temperature));
        delay(1000);
    }

    // GAS
    if (mq_online) {
        current.CO2 = mq.getCorrectedPPM(current.temperature, current.humidity);
        Particle.publish("environment/CO2", String(current.CO2));
        // get_gas(temp, humidity, ppm);  //for calibration output
    }
    // light
    uint16_t luxvalue;
    if (bh_online) {
        current.light = bh.readLightLevel();
        Particle.publish("environment/light", String(current.light));
    }

    // check for rain
    current.rain_intensity = ultrasonic.get_n_readings_pct(current.temperature, 40);
    Particle.publish("environment/rain_intensity", String(current.rain_intensity));

    // check for snow
    current.snow_intensity = 0;
    if (current.temperature <= 10) {
        current.snow_intensity = get_snow();
    }
    Particle.publish("environment/snow", String(current.snow_intensity));

    // save results
    save_current(current);

    // show results
    updateInk(current);
    digitalWrite(PIN_SENSOR_ON, LOW);
}

void get_battery_level(SingleResult& result) {
    digitalWrite(PIN_BATTERY_ON, HIGH);
    delayMicroseconds(4);
    uint16_t lvl = analogRead(PIN_BATTERY_MEASURE);
    Particle.publish("battery analog input read", String(lvl));
    lvl = (uint16_t) round_float_to_int(423*lvl/3465);
    Particle.publish("battery V", String(lvl));
    digitalWrite(PIN_BATTERY_ON, LOW);
    //range is 0 to 70
    lvl = lvl > 450 ? 100 : (lvl < 380 ? 0 : lvl-380);
    result.battery = lvl;

}

void get_pressure(SingleResult& result) {
    bmp.setEnabled(0);
    bmp.triggerMeasurement();
    bmp.getTemperature(result.temperature);
    bmp.getPressure(result.pressure);
    bmp.getAltitude(result.altitude);
    result.pressure = result.pressure / 100;
}

void get_gas(float& temperature, float& humidity, float& corrected_ppm) {
    // assume current humidity. Recommended to measure with DHT22
    float rzero = mq.getRZero();
    float correctedRZero = mq.getCorrectedRZero(temperature, humidity);
    float resistance = mq.getResistance();
    float ppm = mq.getPPM();
    corrected_ppm = mq.getCorrectedPPM(temperature, humidity);
    Particle.publish("environment/rzero", String(rzero));
    Particle.publish("environment/correctedRZero", String(correctedRZero));
    Particle.publish("environment/resistance", String(resistance));
}

void dht_wrapper() {
    DHT.isrCallback();
}

bool get_dht_status() {
    DHT.acquire();
    int result = DHT.getStatus();
    if (result == DHTLIB_OK) {
        return true;
    }
    return false;
}
void start_dht_acquire(){
    if (!bDHTstarted) {
        DHT.acquire();
        bDHTstarted = true;
    } else {
        Particle.publish("DHT not finished YET");
    }
}
void get_humidity(SingleResult& result, SingleResult& prev_result, bool& performed) {
    if (!DHT.acquiring()) {
        int status = DHT.getStatus();
        switch (status) {
        case DHTLIB_OK:
            Particle.publish("OK");
            performed = true;
            break;
        case DHTLIB_ERROR_CHECKSUM:
            delay(1000);
            Particle.publish("Error\n\r\tChecksum error");
            break;
        case DHTLIB_ERROR_ISR_TIMEOUT:
            delay(1000);
            Particle.publish("Error\n\r\tISR time out error");
            break;
        case DHTLIB_ERROR_RESPONSE_TIMEOUT:
            delay(1000);
            Particle.publish("Error\n\r\tResponse time out error");
            break;
        case DHTLIB_ERROR_DATA_TIMEOUT:
            delay(1000);
            Particle.publish("Error\n\r\tData time out error");
            break;
        case DHTLIB_ERROR_ACQUIRING:
            delay(1000);
            Particle.publish("Error\n\r\tAcquiring");
            break;
        case DHTLIB_ERROR_DELTA:
            delay(1000);
            Particle.publish("Error\n\r\tDelta time to small");
            break;
        case DHTLIB_ERROR_NOTSTARTED:
            delay(1000);
            Particle.publish("Error\n\r\tNot started");
            break;
        default:
            delay(1000);
            Particle.publish("Unknown error");
            break;
        }

        result.humidity = DHT.getHumidity();
        result.temperature = (result.temperature + DHT.getCelsius()) / 2;
        // dew = DHT.getDewPoint();
        bDHTstarted = false;
    } else {
        // get previous
        Particle.publish("Error: DHT still in progress!");
        result.humidity = prev_result.humidity;
        result.temperature = (result.temperature + prev_result.temperature) / 2;
    }
}

int get_snow(int n) {
    int successes = 0;
    for (int i=0; i<n; i++) {
        if (analogRead(PIN_TRCT) < 3990) {
            successes++;
        }
        delay(IR_PROBE_DELAY);
    }
    return successes ? (successes / n * 100) : successes;
}

const unsigned char *get_cloud_icon(int rain_intensity, int snow_intensity){
    if ((snow_intensity > 0) && (snow_intensity <= 15)) {
        return icons_snow_1;
    } else if ((snow_intensity > 15) && (snow_intensity <= 60)) {
        return icons_snow_2;
    } else if (snow_intensity > 60){
        return icons_snow_3;
    }
    if (rain_intensity==0 ) {
        return icons_cloud;
    } else if (rain_intensity<=15) {
        return icons_rain_1;
    } else if (rain_intensity<=60) {
        return icons_rain_2;
    } else {
        return icons_rain_3;
    }
}

const unsigned char *get_sun_icon(uint16_t light, int timesptamp){

}


void draw_bar_chart(GxEPD_Class& ink, int x, int y, float* data, int data_size){
    // (x, i) top left container corener (icnlusive)
    // accepts arry of floats to plot
    // plots in 100x59px box

    // char base
    ink.drawFastHLine(x+3, y+56, 94, GxEPD_RED);
    for (int i=0; i<data_size; i++) {
        uint32_t val = round_float_to_int(data[i]);
        ink.fillRect(x+6+(i*4), y+4+(52-val), 3, val, GxEPD_BLACK);
    }
}

void draw_pressure_chart(SingleResult& result) {
    SingleResult *results = get_all_results();
    int num_results = get_num_of_results(results, result.timestamp);
    num_results = num_results > 22 ? 22 : num_results;
    //Particle.publish("num_results", String(num_results));
    //delay(1000);

    float p_results[num_results], delta, offset, min_p=0, max_p=0;
    int j = 0;
    for (int i=num_results; i>0; i--){
        p_results[j] = results[i].temperature ;
        if (j==0) {
            max_p = p_results[j];
            min_p = p_results[j];
        }
        if (p_results[j]  > max_p) max_p = p_results[j];
        if (p_results[j]  < min_p) min_p = p_results[j];
        j++;
    }
    // get rid of negative values
    if (min_p < 0) {
        for (int i=0; i<num_results; i++)
            p_results[i] +=  min_p * -1;
        max_p += min_p * -1;
        min_p += min_p * -1;
    }
    Particle.publish("minmax", String(min_p) + " " + String(max_p));
    delta = max_p - min_p;
    if (delta <= 1) {
        offset = min_p + 1;
        max_p = 2;
    } else {
        offset = min_p - delta * 0.1;
        max_p -= offset;
    }
    for (int i=0; i<num_results; i++)
        p_results[i] =  max_p ? ((p_results[i] - offset) * 52/(max_p)) : 0;
    delete[] results;

    draw_bar_chart(display, 99, 80, p_results, num_results);
}

void updateInk(SingleResult& result){
    const GFXfont* f8 = &DejaVu_Sans_Mono_8;
    const GFXfont* f12 = &DejaVu_Sans_Mono_12;
    const GFXfont* f16 = &Nimbus_Sans_L_Bold_Condensed_16;

    const unsigned char *cloud_icon = get_cloud_icon(result.rain_intensity, result.snow_intensity);
    //const unsigned char *sun_icon = get_sun_icon(light);

    display.setRotation(3);
    display.fillRect(0, 0, 200, 200, GxEPD_WHITE);
    // First row (status | date)
    display.drawRect(0, 0, 100, 20, GxEPD_BLACK);

    //battery
    display.drawRect(3, 3, 33, 14, GxEPD_BLACK);
    display.fillRect(36, 7, 3, 6, GxEPD_BLACK);
    for (int i=0; i<4; i++){
        display.fillRect(4+i*8, 5, 7, 10, GxEPD_RED);
    }
    display.setTextColor(GxEPD_BLACK);
    display.setFont(f12);
    display.setCursor(44, 14);
    display.print("100%");

    display.fillRect(100, 0, 100, 20, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(f8);
    display.setCursor(110, 14);
    display.println(get_time_string(rtc, currentTime));

    // 2nd row (TMP | HUM | Pressure)
    display.fillRect(0, 19, 200, 24, GxEPD_RED);
    display.drawRect(0, 19, 50, 60, GxEPD_BLACK);
    display.drawRect(49, 19, 51, 60, GxEPD_BLACK);
    display.drawRect(99, 19, 100, 60, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(f12);
    display.setCursor(15, 35);
    display.println("TMP");
    display.setCursor(65, 35);
    display.println("HUM");
    display.setCursor(115, 35);
    display.println("PRESSURE");
    display.setTextColor(GxEPD_BLACK);
    display.setFont(f16);
    display.setCursor(5, 65);
    display.print(result.temperature, 1);
    display.print("C");
    display.setCursor(55, 65);
    display.print(result.humidity, 1);
    display.print("%");
    display.setCursor(109, 65);
    display.print(result.pressure, 1);
    display.print(" HPa");

    // 4th row (RAIN / CLOUDS)

    display.drawRect(0, 80, 100, 59, GxEPD_BLACK);
    display.drawRect(99, 80, 100, 59, GxEPD_BLACK);
    display.drawBitmap(icons_sun, 24, 85, 50, 50, GxEPD_WHITE);
    //display.drawBitmap(cloud_icon, 124, 85, 50, 50, GxEPD_WHITE);
    draw_pressure_chart(result);

    // 5th row (CO2 / DEW POINT)
    display.fillRect(0, 140, 200, 24, GxEPD_RED);
    display.drawRect(0, 140, 100, 60, GxEPD_BLACK);
    display.drawRect(99, 140, 100, 60, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(f12);
    display.setCursor(15, 157);
    display.println("AIR C02");
    display.setCursor(115, 157);
    display.println("Light");
    display.setTextColor(GxEPD_BLACK);
    display.setFont(f16);
    display.setCursor(12, 187);
    display.print(result.CO2, 1);
    display.print(" ppm");
    display.setCursor(135, 187);
    display.print(result.light);
    display.print(" LUX");

    display.update();
}
