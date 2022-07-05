#include "HX711.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "DHT.h" // including the library of DHT11 temperature and humidity sensor

const char *myWriteAPIKey = "WRITEYOUROWN"; // You will retrieve this key from ThingSpeak platform
unsigned long myChannelNumber = 0;          // Channel number from ThingSpeak
float calibrationFactor = 0;                // Calibration factor is different for each scale and depends on the weight
float zeroFactor = 0;                       // Same as calibration factor

const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D3;
HX711 scale;

#define DHTTYPE DHT22 // DHT 11
#define MQTT_MAX_PACKET_SIZE 512
#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE);

const char *ssid = "";     // SSID for your WiFi
const char *password = ""; // Password for your Wifi

WiFiClient espClient;
long retry = 0;
int timedelay = 20000;

const int sleepTimeS = 600;

void setup()
{
    pinMode(D8, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    Serial.begin(9600);
    digitalWrite(D8, HIGH);

    Serial.println("DHT sensor initialised");
    dht.begin();

    Serial.println("Scale initialised");
    delay(2000);

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    Serial.println(scale.read()); // print a raw reading from the ADC
    Serial.print("Read average: \t\t");
    Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

    scale.set_scale(calibrationFactor); // This value is obtained by using the SparkFun_HX711_Calibration sketch
    scale.set_offset(zero_factor);       // Zero out the scale using a previously known zero_factor
    //  scale.tare();                // reset the scale to 0

    Serial.println("Scale is configured");

    delay(2000);
    setup_wifi();
    delay(2000);

    ThingSpeak.begin(espClient); // Initialize ThingSpeak

    Serial.println("Measuring...");

    float weight = scale.get_units(10);
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t))
    {
        Serial.print("Publish temp: ");
        Serial.println(t);
        ThingSpeak.setField(2, t);
    }
    if (!isnan(h))
    {
        Serial.print("Publish humidity: ");
        Serial.println(h);
        ThingSpeak.setField(3, h);
    }
    if (!isnan(weight))
    {
        Serial.print("Publish weight: ");
        Serial.println(weight);
        ThingSpeak.setField(1, weight);
    }
    if (!isnan(t) || !isnan(h) || !isnan(weight))
    {
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

        if (x == 200)
        {
            Serial.println("Channel update successful.");
            // This is to show light indicator when published on ThingSpeak
            digitalWrite(D7, LOW);
            delay(500);
            digitalWrite(D7, HIGH);
            delay(500);
            digitalWrite(D7, LOW);
            delay(500);
            digitalWrite(D7, HIGH);
            delay(500);
            digitalWrite(D7, LOW);
            delay(500);
            digitalWrite(D7, HIGH);
            delay(500);
            digitalWrite(D7, LOW);
            delay(500);
            digitalWrite(D7, HIGH);
            delay(500);
        }
        else
        {
            // This is to show light indicator when there is a problem while publising on ThingSpeak
            Serial.println("Problem updating channel. HTTP error code " + String(x));
            digitalWrite(D8, LOW);
            delay(500);
            digitalWrite(D8, HIGH);
            delay(500);
            digitalWrite(D8, LOW);
            delay(500);
            digitalWrite(D8, HIGH);
            delay(500);
            digitalWrite(D8, LOW);
            delay(500);
            digitalWrite(D8, HIGH);
            delay(500);
            digitalWrite(D8, LOW);
            delay(500);
            digitalWrite(D8, HIGH);
            delay(500);
        }
    }
    else
        Serial.println("Temperature is not measured.");

    go_to_sleep();
}
void go_to_sleep()
{
    WiFi.disconnect(true);
    delay(10);
    // Put the device in sleep mode to save battery, to make it work we need to connect RST to D0 (GPIO16)
    Serial.println("ESP8266 in sleep mode");
    ESP.deepSleep(sleepTimeS * 1000000);
}
void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        retry++;
        delay(2000);
        Serial.print(".");
        if (retry == 20)
        {
            go_to_sleep();
        }
    }

    digitalWrite(D8, LOW);
    digitalWrite(D7, HIGH);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
}
