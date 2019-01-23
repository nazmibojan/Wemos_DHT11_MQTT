#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define wifi_ssid "Mako Brimob"
#define wifi_password "G0beyondexpectation!"

#define DHTPIN D4
#define DHTTYPE DHT11

bool debug = false;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  setup_wifi();
  dht.begin();
}

/*
 * @brief Initialize connection to wifi
*/
void setup_wifi(void)
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi OK ");
  Serial.print("=> ESP8266 IP address: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  // Read humidity
  float h = dht.readHumidity();
  // Read temperature in Celcius
  float t = dht.readTemperature();

  if (debug) {
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print(" | Humidity : ");
    Serial.println(h);
  }  
}
