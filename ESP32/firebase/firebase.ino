/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include <WiFi.h>

#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://smartmeter-e9738-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "j2kJRCDuE95uMEbmqdrutpXWzv2i7fTX0x7CwqZ1"
FirebaseData firebaseData;

void setup()
{
Serial.begin(115200);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("Wi_Fi","Pak@$%6141304");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}


void loop()
{

    if ((Firebase.setFloat(firebaseData, "/cons_kwh", 0.12345)) == true) {
      Serial.println("Data subitted");

    } else {
      Serial.println("Error");

    }

}
