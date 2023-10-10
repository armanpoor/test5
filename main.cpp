#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduinoIP(192, 168, 1, 106);
IPAddress brokerIP(192, 168, 1, 104);
const char* clientID = "arduinoClient";
const char* publishTopic = "havas/rfid/arduino";
const char* subTopic = "havas/rfid/warehouse/dl920/1";
const int port = 1883;

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.println("");
        // print arduino IP address:
        Serial.print("Local IP address: ");
        Serial.println(Ethernet.localIP());
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientID)) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(publishTopic, "Message from arduino");
            // ... and resubscribe
            client.subscribe(subTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
        }
    }
}

void setup()
{
    Serial.begin(57600);

    client.setServer(brokerIP, port);
    client.setCallback(callback);

    Ethernet.begin(mac, arduinoIP);
    // Allow the hardware to sort itself out
    delay(1500);
}

void loop()
{
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}