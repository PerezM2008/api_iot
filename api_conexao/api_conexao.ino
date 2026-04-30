#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define WIFI_SSID "DePaula"
#define WIFI_PASS "12345678"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

String serverName = "https://api-iot-1-gp4b.onrender.com/sensor";

float ultimaTemp = -1000;
float ultimaUmidade = -1000;

void setup() {

  Serial.begin(115200);
  delay(1000);

  dht.begin();

  Serial.print("Connectando ao WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectando!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); 

}

void loop() {
  
  if (WiFi.status () != WL_CONNECTED) {
    Serial.println("WiFi dwesconecctado!");
    delay(2000);
    return;
  }
 
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan (temp) || isnan(hum)){
    Serial.println ("Erro ao ler DHT11");
    delay (2000);
    return;
  }


  if (abs(temp - ultimaTemp) < 0.2 && abs(hum - ultimaUmidade) <  1.0) {
    return;
  }
  
  Serial.print ("Temp: ");
  Serial.print (temp);
  Serial.print (" | Umidade: ");
  Serial.print (hum);

  HTTPClient http;

  String url = serverName + "?temp" + String(temp) + "&hum=" + String(hum);

  Serial.print("Enviando para: ");
  Serial.println(url);

  http.begin(url);

  int httpResponseCode = http.GET();

  Serial.print ("Resposta HTTP: ");
  Serial.println (httpResponseCode);

  http.end();

  ultimaTemp = temp;
  ultimaUmidade = hum;

  delay(5000); 
}
