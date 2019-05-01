#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// const char* ssid = "TP-Link_D0A85";
const char* ssid = "Incognito";
const char* pass = "leomessi";

const char* mqttServer = "149.56.36.129"; 
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(0, OUTPUT);
    Serial.begin(115200);

    WiFi.begin(ssid, pass);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Wi-Fi Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer(mqttServer, mqttPort);
    client.setCallback(MQTTcallback);
    while (!client.connected()) {
        Serial.println("Connecting to MQTT");
        if (client.connect("ESP8266")) {
            Serial.println("Connected");
        } else {
            Serial.print("Failed with state");
            Serial.println(client.state());
            delay(2000);
        }
    }
    client.publish("lampu", "Halo! saya ESP8266");
    client.subscribe("lampu");
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Topic: ");
    Serial.println(topic);
    
    Serial.print("Pesan: ");

    String message;
    for (int i = 0; i<length; i++) {
        message = message + (char)payload[i];
    }
    Serial.print(message);
    
    if (message == "#on_1") {
        digitalWrite(5, HIGH);
    } else if (message == "#on_2") {
        digitalWrite(4, HIGH);
    } else if (message == "#on_3") {
        digitalWrite(0, HIGH);
    }
    
    if (message == "#off_1") {
        digitalWrite(5, LOW);
    } else if (message == "#off_2") {
        digitalWrite(4, LOW);
    } else if (message == "#off_3") {
        digitalWrite(0, LOW);
    }

    if (digitalRead(5) == 0) {
        client.publish("status", "#status1_off");
    } else if (digitalRead(5) == 1) {
        client.publish("status", "#status1_on");
    }

    if (digitalRead(4) == 0) {
        client.publish("status", "#status2_off");
    } else if (digitalRead(4) == 1) {
        client.publish("status", "#status2_on");
    }

    if (digitalRead(0) == 0) {
        client.publish("status", "#status3_off");
    } else if (digitalRead(0) == 1) {
        client.publish("status", "#status3_on");
    }
    Serial.println();
    Serial.println("---------------");
}

void loop() {
    client.loop();
}
