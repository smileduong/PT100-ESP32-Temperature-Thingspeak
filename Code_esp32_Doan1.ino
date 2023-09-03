#include"WiFi.h"
#include"ThingSpeak.h"

#define CHANNEL_ID 1441034
#define CHANNEL_API_KEY "BTDV7H2SG6J0ZGO4" 
WiFiClient client;
int counter = 0;

#define WIFI_NETWORK "VAN TINH T1"
#define WIFI_PASSWORD "11112222"
#define WIFI_TIMEOUT_MS 20000

const int potPin = 34;
int potValue = 0;

void connectToWiFi(){
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Failed!");
  } else{
    Serial.print("Connected!");
    Serial.println(WiFi.localIP());
  }
}
  
void setup() {
 Serial.begin(115200);
 connectToWiFi();
 ThingSpeak.begin(client);
}

void loop() {
  //  float h = random(100);
  //  float t = random(70);
   potValue = analogRead(potPin);
   float t = (float)potValue*100/3918;  //Đọc nhiệt độ
  
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    //    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Ghi dữ liệu lên 1 field
  //  int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  //  int y = ThingSpeak.writeField(myChannelNumber, 2, t, myWriteAPIKey);
  // In dữ liệu debug
  Serial.print("Temperature: ");
  Serial.print(t);
  
  // Ghi dữ liệu lên nhiều field
  int x =  ThingSpeak.setField(1, t); //setField(field, value)
  
  int z = ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  // Kiểm tra return code
  if (z == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(z));
  }

  delay(15000); // Chờ 20s trước khi gửi dữ liệu mới
}
