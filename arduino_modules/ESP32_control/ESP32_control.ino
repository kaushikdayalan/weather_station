#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"
#include <ArduinoJson.h>


#define THINGNAME "Classic Shadow"    
#define DHTPIN 14    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define AWS_IOT_PUBLISH_TOPIC   "weather/post_data"
#define POWER_PIN  17 
#define SIGNAL_PIN 36

int value = 0;
const char* ssid     = "LJ-Net";    
const char* password = "40065120334001630306";  


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
String arr_days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----)EOF";

// Device Certificate                                            
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIURc7bByjC4wR8hgU9PyHVwXHuHSEwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMTExNzEzNDQz
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMhAjHO1cn+7xrCKd3Nj
6YBpWIRzIV+DBytbmsx6nqxKdokRmGgHz/teY+iBG59R/MftAn5RXaJgGpJxGKCT
MhGHIMh3xarHUzqTfDv/UDpYMJiTl2+ZzpzGuyS/BHYo7H3fZuGFEhCFy+apJN3V
SZ+3nZPwPdv4c9Qc9STsa3XabSRHRX9ZAthlGmCo8Yg95HUCsvMHEK4GARvBbkr7
pPWnqyjyJIRHPf90SSpNhZR2uRrWJV16QUQKtDbvnJjohzqUGuHr1Hj91CLxtnfe
TFq3FFkHQiKaNYh1qGomkUbZ/Ryevg5qRJAxTuqsO6H1HkAeUT9Yt8+0MVURMM3z
97UCAwEAAaNgMF4wHwYDVR0jBBgwFoAUmxSH1d/ugFK1ZbnIvA2dK05EaxUwHQYD
VR0OBBYEFBgTkq/cq7n2VZEQINw9SOqqqkZcMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBbGrMXgI9PyYLjEwgqmUMdQ3ww
C78jDKUdBzVoAg/1OgALyUDFLjkIFY7usHcvWlNqxwaYhrHiS28tQ9vdbyrxnxRr
37DhHzeDVAbp3LblwswHF8aaWTwPqkgyMMvgAyjM2zmX6c3viDIiO7iX3DpHQA7F
1NB2mVPLRXYqVnLwzHcps8jaqFG0wIJBXCd/lCW0aKLyLDwrDy+AxxxDBoJ/8wkk
7x+iT4i25JsYQ+/dDkEtW3iWHLrjzw9sP5B7vD4WgldSLzjbsIOb66Vk6kBQmKr4
PBqI3H9F9xYRcTzZranmi8QNXxyCA/LIcFC1FqAWEe66keb5fc6yUQG9BsCQ
-----END CERTIFICATE-----)KEY";

// Device Private Key                                          
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpgIBAAKCAQEAyECMc7Vyf7vGsIp3c2PpgGlYhHMhX4MHK1uazHqerEp2iRGY
aAfP+15j6IEbn1H8x+0CflFdomAaknEYoJMyEYcgyHfFqsdTOpN8O/9QOlgwmJOX
b5nOnMa7JL8Edijsfd9m4YUSEIXL5qkk3dVJn7edk/A92/hz1Bz1JOxrddptJEdF
f1kC2GUaYKjxiD3kdQKy8wcQrgYBG8FuSvuk9aerKPIkhEc9/3RJKk2FlHa5GtYl
XXpBRAq0Nu+cmOiHOpQa4evUeP3UIvG2d95MWrcUWQdCIpo1iHWoaiaRRtn9HJ6+
DmpEkDFO6qw7ofUeQB5RP1i3z7QxVREwzfP3tQIDAQABAoIBAQCcSSRUuevenOJ4
rdiEChoWyiD3XhNRw6U4xYmOxLRJquPQHhdgOZisigJXV0wDj61FK0ii5UW6kF6s
o++ggsC0xU1kp3Vxz5a5RJnVCXHH10Q2EBpEUznZSZCNITeOMGpB4rUclQqAMqvs
HV5Ucz2+lUM6Tmk44DMl/GOmqY1EsA5hRIUCzWraIUcw8Y5WHrt8urdQlqX65DUU
4X3kSOSWwjTDsXMf6VCd/fJIEewZ0v+RlsdLyAyHZKhhXYpwaTMkaOgcf4CVgh/6
1Avdruuw2YfhQUajUEZEBgFwxtTPh/FbV5AoGwWGgJ5ScQsIF9Pl4HESZ19DEyWC
nN0R11EBAoGBAO2m949fnr2mcs96CsWb6cZvJ7wtW2AW3qJmsOssOwmob2XsOstj
xMT/F054nTcOgezfZpGJbGXsQfP11haZISCLn9DdF4f0SEuSPU26hCUFaATuHNK/
hdDfkT9r1T08A2VaOKa3+R/eEtBjXs6J/yeXTHw6E/TW38REW+8v57lFAoGBANe2
ZRTscYIURpJSgPILxnICx/wFXZPfou+CR7HCNFb+d0G+fzeQ6+/n2HF8Er7wTwrf
c2B7v2pkFztZZQ+mMJH1lfNiRCCHVpbvYXiv7LITRpFlTeWLSTLtgCQW69t30z62
ZuodBxuE0ohxe8QOZZOwvvpsoaCqN8i1pMf69dOxAoGBAM7Qk/Ygh5uRo65tDbzE
n2AB0bmQW0m2/U6lQWeXmVUhULiJKO1hSJcLS3QNwPtt06GZFMCosD3OqeKt2P7X
ih2xhIYWMektrFCf1luzalZxawGHWmwJImIQfW2M4QimFmp4Zn6n8dRKxWK2Z4Yk
T3lb0ML3I/DZoehCeSMhNG5lAoGBAMbJrG9S0evJGZwAchoc5dztWTSUET+TyUil
K8LRsIA28tngXsZrRDKK2vkCGRhixsTDVUUl5Sl7Ybls0WhRg9MOmgh3l0bO0eo4
Yf/hR+R3RQH6dIXEDUnkesFHakWPMFpLp5258HHYvlfFFQSBK/Sl3elTsMMp+kx5
sIgmauKRAoGBAOOda4uYsOJvZgBlq3J688WRbdwnCcs21huxzOlT8CSFYrk+eOBP
oQ0c+WDO/q5E/+x56lTHn8/zsJvMJg7iKZuovLZmW1CRfOzHlAvIU3awEG0WA5Go
1JfdXsrCfhUPoILwkAyhxXa17REbcQw650PJzdXLdNYcnqBTXhjrvn19
-----END RSA PRIVATE KEY-----)KEY";

const char AWS_IOT_ENDPOINT[] = "a1tmhchnoirllr-ats.iot.us-east-1.amazonaws.com";
float h ;
float t;
 
DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME)){
    Serial.print(".");
    delay(100);}
 
  if (!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;}
  else{Serial.println(client.connected());} 
  
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<500> doc;
  StaticJsonDocument<200> doc1;
  StaticJsonDocument<200> doc2;
  doc["device_id"] = "weather_station_1";
  doc["time"] = formattedDate;
  doc2["humidity"] = h;
  doc2["temperature"] = t;
  if(value > 600){
    doc["rain"] = "yes";
  }
  else{
    doc["rain"] = "no";
  }
  doc1["reported"] = doc2;
  doc["state"] = doc1;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Message published");  
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {  //  waiting with Wifi connectivity
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(POWER_PIN, OUTPUT);  
  connectAWS();
  dht.begin();
  digitalWrite(POWER_PIN, LOW);
// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}
void loop() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  
  // The formattedDate comes with the following format:
  // yyyy.mm.ddThh:mm:ssZ
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);
  Serial.println(formattedDate);
  h = dht.readHumidity();
  t = dht.readTemperature();
  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);
  //  publishMessage();
  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Serial.print("rain sensor: ");
  Serial.println(value);
  Serial.println(formattedDate);
  publishMessage();
  client.loop();
  delay(2000);
}
