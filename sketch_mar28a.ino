#include <ESP8266WiFi.h>

#define LED D2


const char* ssid = "yourssid";
const char* password = "yourpassword";

unsigned char status_led = 0;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);//establish connection

  while (WiFi.status() != WL_CONNECTED) {
    delay(800);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi COnnected!");//connection established

  server.begin(); // server is online
  Serial.println("Server started");
  Serial.println(WiFi.localIP());// shows IP address
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }
  Serial.println("New client");
  while (!client.available()) {
    delay(10);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  if (req.indexOf("/ledoff") != -1)
  {
    // string.indexOf(val) returns -1 if not found
    status_led = 0;
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");

  }
  if (req.indexOf("/ledon") != -1)
  {
    // string.indexOf(val) returns -1 if not found
    status_led = 1;
    digitalWrite(LED, HIGH);
    Serial.println("LED ON");

  }

  // Method 1: enter HTML page as a string and send to client
   webstring(client);

  // Method 2: using client class function
  // clientprint(client);
}

void webstring(WiFiClient client) {
  String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<html>\r\n";
  web += "<body>\r\n";
  web += "<h1>LED Status: </h1>\r\n";
  web += "<p>\r\n";
  if (status_led == 1)web += "LED ON\r\n";
  else if (status_led == 0)web += "LED OFF\r\n";
  else web += "Error!\r\n";
  web += "</p>\r\n";

  web += "<p>\r\n";

  web += "<a href = \"/ledon\">\r\n";
  web += "<button>LED On</button>\r\n";
  web += "</a>\r\n";
  web += "</p>\r\n";

  web += "<p>\r\n";
  web += "<a href = \"/ledoff\">\r\n";
  web += "<button>LED Off</button>\r\n";
  web += "</a>\r\n";

  web += "</p>\r\n";

  web += "</body>\r\n";
  web += "</html>\r\n";

  client.print(web);
}

void clientprint(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<HTML>");
  client.println("<H1> LED CONTROL </H1>");
  client.println("<br/>");

  client.println("<a href = \"/ledon\"\"> <button style = 'FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> LED ON </button></a>");
  client.println("<a href = \"/ledoff\"\"> <button style = 'FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> LED OFF </button></a>");

  client.println("</html>");

  delay(10);
  Serial.println("Client disconnected");
  Serial.println("");
}

