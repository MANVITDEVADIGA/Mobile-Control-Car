#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "A53";       // Replace with your Wi-Fi network name
const char* password = ""; // Replace with your Wi-Fi password

// Motor Driver Pin Definitions
const int motor1A = 13;  // Motor 1 forward
const int motor1B = 12; // Motor 1 backward
const int motor2A = 14; // Motor 2 forward
const int motor2B = 27;// Motor 2 backward
const int L_LED_PIN = 26;    // left idicator led
const int R_LED_PIN = 25;    // right indicator led
const int B_LED_PIN = 33;    //break led
const int Re_LED_PIN = 32;    //REVERSE LIGHT

WiFiServer server(80); // Start a web server on port 80

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);

  // Initialize motor control pins
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(L_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
  pinMode(Re_LED_PIN, OUTPUT);



  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    attempts++;
    Serial.print("Attempting to connect (Attempt ");
    Serial.print(attempts);
    Serial.println(")...");

    // Provide status-specific feedback
    int status = WiFi.status();
    if (status == WL_NO_SSID_AVAIL) {
      Serial.println("Error: Network SSID not found.");
      break;
    } else if (status == WL_CONNECT_FAILED) {
      Serial.println("Error: Connection failed. Check your password.");
      break;
    } else if (status == WL_IDLE_STATUS) {
      Serial.println("Status: Wi-Fi is idle...");
    } else if (status == WL_DISCONNECTED) {
      Serial.println("Status: Wi-Fi is disconnected.");
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi successfully!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    server.begin(); // Start the web server
  } else {
    Serial.println("Failed to connect to Wi-Fi. Please check your setup.");
  }
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients
  if (client) {
    Serial.println("New Client Connected");
    String request = client.readStringUntil('\r'); // Read HTTP request
    Serial.println(request);
    client.flush();

    // Motor control based on HTTP request
    if (request.indexOf("/forward") != -1) {
      moveForward();
    } else if (request.indexOf("/backward") != -1) {
      moveBackward();
    } else if (request.indexOf("/left") != -1) {
      turnLeft();
    } else if (request.indexOf("/right") != -1) {
      turnRight();
    } else if (request.indexOf("/stop") != -1) {
      stopMotors();
    }

    // Send the control web page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP32 Car Control</h1>");
    client.println("<button onclick=\"fetch('/forward')\">Forward</button>");
    client.println("<button onclick=\"fetch('/backward')\">Backward</button>");
    client.println("<button onclick=\"fetch('/left')\">Left</button>");
    client.println("<button onclick=\"fetch('/right')\">Right</button>");
    client.println("<button onclick=\"fetch('/stop')\">Stop</button>");
    client.println("</html>");
    delay(10);
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(B_LED_PIN,LOW);
  digitalWrite(Re_LED_PIN,LOW);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(B_LED_PIN,LOW);
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  digitalWrite(Re_LED_PIN,HIGH);
  Serial.println("Moving Backward");
}

void turnLeft() {
  digitalWrite(Re_LED_PIN,LOW);
  digitalWrite(B_LED_PIN,LOW);
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  digitalWrite(L_LED_PIN, HIGH);
  delay(1000);

  // Turn the LED off
  digitalWrite(L_LED_PIN, LOW);
  delay(1000); // Wait for 1 second
   digitalWrite(L_LED_PIN, HIGH);
  delay(1000);

  // Turn the LED off
  digitalWrite(L_LED_PIN, LOW);
  delay(1000); // Wait for 1 second
   digitalWrite(L_LED_PIN, HIGH);
  delay(1000);

  // Turn the LED off
  digitalWrite(L_LED_PIN, LOW);
  delay(1000); // Wait for 1 second

  Serial.println("Turning Left");
}

void turnRight() {
  digitalWrite(Re_LED_PIN,LOW);
  digitalWrite(B_LED_PIN,LOW);
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  
  // Turn the LED on
  digitalWrite(R_LED_PIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(R_LED_PIN, LOW);
  delay(1000); // Wait for 1 second
digitalWrite(R_LED_PIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(R_LED_PIN, LOW);
  delay(1000); // Wait for 1 second
digitalWrite(R_LED_PIN, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(R_LED_PIN, LOW);
  delay(1000); // Wait for 1 second

  Serial.println("Turning Right");
}

void stopMotors() {
  digitalWrite(Re_LED_PIN,LOW);
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
  digitalWrite(B_LED_PIN,HIGH);
  Serial.println("Stopping Motors");
}