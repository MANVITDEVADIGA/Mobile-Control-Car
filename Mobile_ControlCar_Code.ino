#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "S20";             // Replace with your Wi-Fi network name
const char* password = "123456789";           // Replace with your Wi-Fi password

// Motor Driver Pin Definitions
const int motor1A = 5;               // Motor 1 forward direction
const int motor1B = 18;              // Motor 1 backward direction
const int motor2A = 4;               // Motor 2 forward direction
const int motor2B = 15;              // Motor 2 backward direction

WiFiServer server(80);              // Create a web server on port 80

void setup() {
  Serial.begin(115200);             // Start the Serial Monitor
  delay(1000);                      // Short delay for stabilization

  // Set motor pins as outputs
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);

  // Start connecting to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int attempts = 0;

  // Retry connection for a maximum of 20 attempts (approx 20 seconds)
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    attempts++;

    Serial.print("Attempting to connect (Attempt ");
    Serial.print(attempts);
    Serial.println(")...");

    // Provide specific feedback on Wi-Fi connection status
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

  // Check if connected and start the server
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi successfully!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    server.begin();  // Start the web server
  } else {
    Serial.println("Failed to connect to Wi-Fi. Please check your setup.");
  }
}

void loop() {
  WiFiClient client = server.available();  // Check if a client has connected

  if (client) {
    Serial.println("New Client Connected");

    // Read the HTTP request from the client
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();  // Clear the remaining data from the buffer

    // Control the motors based on the URL endpoint requested
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

    // Send a simple web page with control buttons
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
    delay(10);  // Short delay before closing the connection
  }
}

// Function to move the car forward
void moveForward() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  Serial.println("Moving Forward");
}

// Function to move the car backward
void moveBackward() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  Serial.println("Moving Backward");
}

// Function to turn the car to the right
void turnRight() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
  Serial.println("Turning Left");  // **Correction needed: This should say "Turning Right"**
}

// Function to turn the car to the left
void turnLeft() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
  Serial.println("Turning Right");  // **Correction needed: This should say "Turning Left"**
}

// Function to stop all motors
void stopMotors() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
  Serial.println("Stopping Motors");
}
