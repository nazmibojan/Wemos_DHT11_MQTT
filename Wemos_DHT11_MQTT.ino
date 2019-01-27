#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define wifi_ssid "SSID"
#define wifi_password "Password"

#define mqtt_server "IP_Mosquitto"
#define mqtt_user "user"
#define mqtt_password "pass"

#define temperature_topic "temperature"
#define humidity_topic "humidity"

// Buffer to decode MQTT messages
char message_buff[100];

long lastMsg = 0;   
long lastRecu = 0;
bool debug = true;

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
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

void reconnect() {

  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  // Send a message every second
  if(now -  lastMsg > 1000 * 1){
    lastMsg = now;
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

    client.publish(temperature_topic, String(t).c_str(), true);
    client.publish(humidity_topic, String(h).c_str(), true);
  }  
}
