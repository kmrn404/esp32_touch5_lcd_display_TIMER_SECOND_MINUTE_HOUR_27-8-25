#include <WiFi.h>
#include <Arduino.h>
// Define relay pins
const int RELAY1_PIN = 32;
const int RELAY2_PIN = 33;
const int RELAY3_PIN = 25;
const int RELAY4_PIN = 26;
const int RELAY5_PIN = 27;
const int RELAY6_PIN = 14;

const int BUTTON_PIN1 = 16;
const int BUTTON_PIN2 = 17;
const int BUTTON_PIN3 = 5;
const int BUTTON_PIN4 = 18;
const int BUTTON_PIN5 = 4;
const int BUTTON_PIN6 = 15;

bool buttonState1 = LOW;
bool buttonState2 = LOW;
bool buttonState3 = LOW;
bool buttonState4 = LOW;
bool buttonState5 = LOW;
bool buttonState6 = LOW;

bool lastbuttonState1 = HIGH;
bool lastbuttonState2 = HIGH;
bool lastbuttonState3 = HIGH;
bool lastbuttonState4 = HIGH;
bool lastbuttonState5 = HIGH;
bool lastbuttonState6 = HIGH;

bool relayState1 = LOW;
bool relayState2 = LOW;
bool relayState3 = LOW;
bool relayState4 = LOW;
bool relayState5 = LOW;
bool relayState6 = LOW;

// Define STA credentials (your AP's SSID and password)
const char *ssid = "ESP32_AP";  // Replace with your AP's SSID
const char *password = "123456789";  // Replace with your AP's password

WiFiClient client;

int count_wifi = 0;
int count_sent_data = 0;
String statusMessage;
String relayData;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi (STA mode)
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while ((WiFi.status() != WL_CONNECTED) && (count_wifi <= 7)) {
    delay(500);
    Serial.print(".");
    count_wifi++;
  }
  Serial.println("Connected to WiFi!");

  // Connect to the AP server (Transmitter ESP32)
  if (client.connect("192.168.4.1", 8080)) {  // 192.168.4.1 is the default AP IP address
    Serial.println("Connected to the Transmitter ESP32");
  } else {
    Serial.println("Connection to the Transmitter ESP32 failed!");
  }

  // Set relay pins as output
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(RELAY5_PIN, OUTPUT);
  pinMode(RELAY6_PIN, OUTPUT);

  //Set button pins as input
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  pinMode(BUTTON_PIN4, INPUT_PULLUP);
  pinMode(BUTTON_PIN5, INPUT_PULLUP);
  pinMode(BUTTON_PIN6, INPUT_PULLUP);

  //trun off all the relays
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(RELAY4_PIN, HIGH);
  digitalWrite(RELAY5_PIN, HIGH);
  digitalWrite(RELAY6_PIN, HIGH);
}

void loop() {
  
  if (client.available()) {
    // Read incoming data (relay states)
    relayData = client.readStringUntil('\n');  // Read the data as a string
    Serial.println("Received relay states: " + relayData);

    // Parse the data and set relay states
    int delimiter1 = relayData.indexOf(',');
    int delimiter2 = relayData.indexOf(',', delimiter1 + 1);
    int delimiter3 = relayData.indexOf(',', delimiter2 + 1);
    int delimiter4 = relayData.indexOf(',', delimiter3 + 1);
    int delimiter5 = relayData.indexOf(',', delimiter4 + 1);
    int delimiter6 = relayData.indexOf(',', delimiter5 + 1);

    /*bool relay1_state = relayData.substring(0, delimiter1) == "1";
    bool relay2_state = relayData.substring(delimiter1 + 1, delimiter2) == "1";
    bool relay3_state = relayData.substring(delimiter2 + 1, delimiter3) == "1";
    bool relay4_state = relayData.substring(delimiter3 + 1, delimiter4) == "1";*/

    relayState1 = relayData.substring(0, delimiter1) == "1";
    relayState2 = relayData.substring(delimiter1 + 1, delimiter2) == "1";
    relayState3 = relayData.substring(delimiter2 + 1, delimiter3) == "1";
    relayState4 = relayData.substring(delimiter3 + 1, delimiter4) == "1";
    relayState5 = relayData.substring(delimiter4 + 1, delimiter5) == "1";
    relayState6 = relayData.substring(delimiter5 + 1, delimiter6) == "1";

    // Control the relays based on received states
    /*digitalWrite(RELAY1_PIN, relay1_state ? LOW : HIGH);
    digitalWrite(RELAY2_PIN, relay2_state ? LOW : HIGH);
    digitalWrite(RELAY3_PIN, relay3_state ? LOW : HIGH);
    digitalWrite(RELAY4_PIN, relay4_state ? LOW : HIGH);*/

    // Control the relays based on received states
    digitalWrite(RELAY1_PIN, relayState1 ? LOW : HIGH);
    digitalWrite(RELAY2_PIN, relayState2 ? LOW : HIGH);
    digitalWrite(RELAY3_PIN, relayState3 ? LOW : HIGH);
    digitalWrite(RELAY4_PIN, relayState4 ? LOW : HIGH);
    digitalWrite(RELAY5_PIN, relayState5 ? LOW : HIGH);
    digitalWrite(RELAY6_PIN, relayState6 ? LOW : HIGH);
  }
  delay(100);       //500 working perfectly

  /*digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);

  delay(1000);

  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(RELAY4_PIN, HIGH);

  delay(2000);*/

    buttonState1 = digitalRead(BUTTON_PIN1);
    // Check for a state change (button press)
    if (buttonState1 != lastbuttonState1) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState1 == LOW) {
        relayState1 = !relayState1; // Toggle the LED state
        digitalWrite(RELAY1_PIN, relayState1);
        Serial.print("LED State 1: ");
        Serial.println(relayState1); // ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState1 = buttonState1;

    buttonState2 = digitalRead(BUTTON_PIN2);
    // Check for a state change (button press)
    if (buttonState2 != lastbuttonState2) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState2 == LOW) {
        relayState2 = !relayState2; // Toggle the LED state
        digitalWrite(RELAY2_PIN, relayState2);
        Serial.print("LED State 2: ");
        Serial.println(RELAY2_PIN ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState2 = buttonState2;

    buttonState3 = digitalRead(BUTTON_PIN3);
    // Check for a state change (button press)
    if (buttonState3 != lastbuttonState3) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState3 == LOW) {
        relayState3 = !relayState3; // Toggle the LED state
        digitalWrite(RELAY3_PIN, relayState3);
        Serial.print("LED State 3: ");
        Serial.println(RELAY3_PIN ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState3 = buttonState3;

    buttonState4 = digitalRead(BUTTON_PIN4);
    // Check for a state change (button press)
    if (buttonState4 != lastbuttonState4) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState4 == LOW) {
        relayState4 = !relayState4; // Toggle the LED state
        digitalWrite(RELAY4_PIN, relayState4);
        Serial.print("LED State 4: ");
        Serial.println(RELAY4_PIN ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState4 = buttonState4;

  buttonState5 = digitalRead(BUTTON_PIN5);
    // Check for a state change (button press)
    if (buttonState5 != lastbuttonState5) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState5 == LOW) {
        relayState5 = !relayState5; // Toggle the LED state
        digitalWrite(RELAY5_PIN, relayState5);
        Serial.print("LED State 5: ");
        Serial.println(RELAY5_PIN ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState5 = buttonState5;

  buttonState6 = digitalRead(BUTTON_PIN6);
    // Check for a state change (button press)
    if (buttonState6 != lastbuttonState6) {
      // If the button is now LOW (pressed, with INPUT_PULLUP)
      if (buttonState6 == LOW) {
        relayState6 = !relayState6; // Toggle the LED state
        digitalWrite(RELAY6_PIN, relayState6);
        Serial.print("LED State 6: ");
        Serial.println(RELAY6_PIN ? "ON" : "OFF");
        sendRelayStatus();
      }
      // Small delay for debouncing
      delay(50); // Adjust as needed
    }
    lastbuttonState6 = buttonState6;

    // Send current relay states back to Transmitter
// String statusMessage = String(relayState1 ? "1" : "0") + "," +
//                        String(relayState2 ? "1" : "0") + "," +
//                        String(relayState3 ? "1" : "0") + "," +
//                        String(relayState4 ? "1" : "0") + ", \n";

// if (client.connected()) {
//   client.print(statusMessage);
//   Serial.print("Sent back to Transmitter: ");
//   Serial.println(statusMessage);
// }

  if (!client.connected()) {
    Serial.println("Lost connection. Reconnecting...");
    client.stop();
    if (client.connect("192.168.4.1", 8080)) {
      Serial.println("Reconnected!");
      //sendRelayStatus();
    } else {
      delay(50);     //500 working
      return;
    }
  }
}


void sendRelayStatus()
{
  statusMessage = String(relayState1 ? "1" : "0") + "," +
                       String(relayState2 ? "1" : "0") + "," +
                       String(relayState3 ? "1" : "0") + "," +
                       String(relayState4 ? "1" : "0") + "," +
                       String(relayState5 ? "1" : "0") + "," +
                       String(relayState6 ? "1" : "0") + "," + "\n";
  
  //relayData = statusMessage;

  if (client.connected()) {

    while(count_sent_data <= 4){
    client.print(statusMessage);
    Serial.print("Sent back to Transmitter: ");
    Serial.println(statusMessage);
    delay(150);
    count_sent_data++;
    }

    count_sent_data = 0;
  }

  else if (client.connect("192.168.4.1", 8080)) {
      Serial.println("Reconnected in button loop!");
      client.print(statusMessage);
      Serial.print("Sent button states to ESP32 Transmitter: ");
      Serial.println(statusMessage);
      //relayData = statusMessage;
    } else {
      delay(50);     //500 working
      //return;
    }
}