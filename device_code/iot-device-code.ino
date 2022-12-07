#include <WiFiClientSecure.h>
#include <PubSubClient.h> 
#include <Arduino.h>
#include "DHT.h"

const char* ssid = "--------";
const char* password = "--------";

IPAddress endPoint(192, 168, 8, 101);


// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkECFH+RcdqC/1Q9lb+TgMMvZDPx2uThMA0GCSqGSIb3DQEBCwUAMF0x\n" \
"CzAJBgNVBAYTAklOMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRl\n" \
"cm5ldCBXaWRnaXRzIFB0eSBMdGQxFjAUBgNVBAMMDTE5Mi4xNjguOC4xMDEwHhcN\n" \
"MjIxMjAzMTYxMDQ5WhcNMjQwNDE2MTYxMDQ5WjB1MQswCQYDVQQGEwJJTjETMBEG\n" \
"A1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0cyBQdHkg\n" \
"THRkMQ4wDAYDVQQDDAVyb2hpdDEeMBwGCSqGSIb3DQEJARYPcm9oaXRAZ21haWwu\n" \
"Y29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4AnB+q9QeZc9TBr3\n" \
"ducdNuNosnEEb9oGU+XTE+8ASaulzh89AaFQR+CLy0GiA+z6iqFq/Yiv1ZxYbSQE\n" \
"C+ExxG0yysucSd5bqufvVfd+KGKsfyAj5QQCIjI7/c7vUa+xiLU6+xCF8roQRhwY\n" \
"Nb2mbNXRY1GpNZyC4OdevqRIC3l3egTB4TioQpfo5ramfN0CLDvGOoMbU2dKgrYI\n" \
"eDLb9t4SkKYqQsnCpQJYABoUGxodzmTswTaoKHQgjY/Y0aWhYSgpDMl+LI7cR4if\n" \
"BkaaCI7L8VgPJfCA9EJAYpHqDgvMFqg0aMMjrOwcSd7x35zRdOqj0EVcM3iTJli1\n" \
"IKvIHwIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQAKEHVBod1dzOcfhAb4Gzf8QlGK\n" \
"iXFoCO4WFAElux2N3tYcCqF3dIUz8U2RLI/mXhJmn5QM+FbNQaUIdhWFVjxBe3cy\n" \
"6HHYIr3kFrmmkX/hBFVMD9j/j6oy0WpcDgc4oNue5raf6SBpUBywvnIkSzOR4n2c\n" \
"t4ods6fNfmOu66VAYQTKVbKMLYzBs8tD02F3Mt1pCQpkQCi9uMPZVaj1Bl5WBqQk\n" \
"1e6uI5EzFKz3hKabTKoMwuTyLCMm39bBSl4wlRpYUzJiHnSbz5ilJdTpDNbMd8Yi\n" \
"vQsWygfr99P3Ij4dbgi1k1IIhWz9aH4kzgy+yn5geiXGAw+BWV/G8/HKervK\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEA4AnB+q9QeZc9TBr3ducdNuNosnEEb9oGU+XTE+8ASaulzh89\n" \
"AaFQR+CLy0GiA+z6iqFq/Yiv1ZxYbSQEC+ExxG0yysucSd5bqufvVfd+KGKsfyAj\n" \
"5QQCIjI7/c7vUa+xiLU6+xCF8roQRhwYNb2mbNXRY1GpNZyC4OdevqRIC3l3egTB\n" \
"4TioQpfo5ramfN0CLDvGOoMbU2dKgrYIeDLb9t4SkKYqQsnCpQJYABoUGxodzmTs\n" \
"wTaoKHQgjY/Y0aWhYSgpDMl+LI7cR4ifBkaaCI7L8VgPJfCA9EJAYpHqDgvMFqg0\n" \
"aMMjrOwcSd7x35zRdOqj0EVcM3iTJli1IKvIHwIDAQABAoIBAD1ArbZaREhaHzo1\n" \
"jT8HBSfCmRdgavQhyeUaStGCYLPDqDtoswdFEWv1aoMR2Lf2DLHmXBghTOG8Mzjz\n" \
"xd9xnGe161MixAI2Nx3M99SrK+TixYKkSOwOYn7y+Q/1idztij50PkHyTI0Gbfid\n" \
"KclXpq7O7saWxwqPVgEjgPXieIgaFHb2lRC16javo8P19PkFO4cUawfGo0fHinV5\n" \
"kgDnCCn2EhPg5WhifMDWhcBLkQMgXhJKB3p6LI2nfk1f57mEGdEaypJHguSrNnW/\n" \
"34Z+N5ae1XyS7AvvgeaabtGv0XjsVJtqumJXOwVL7t+MqBUtAbEb2Au8kQX7/hrQ\n" \
"Y0QqDfECgYEA/RHJ/GJ3rCji9yTLJXYW9Q4Eh6zf5TtPxhSlyKrV271fpOfAAUYf\n" \
"h5Ln6dx97ja73nTlxGgx5n92EcaKBkRpU30eZC1EyX38sQWxI1npi64wka32B4j1\n" \
"VhpezujlMoB32xaqT3o4Xr06HigO6w41b7xM4uyY5T3nRAxMjg2eznMCgYEA4qHo\n" \
"Ih2WDW/GkYYeArZyjGKJ2hxYC4MQQYWxu1oiuNS8hTwLX9xKmpOkb+14hn4xtgoH\n" \
"fYzM/iBgYjPyO6+dAqL3DqqC44quA1pwkVPfIGZy+KLCWI2HVAaLujl8qqBwoUB3\n" \
"MkSdRCSaEsU1gBdPc86nZSu66Z8Ox2OtXpZGaKUCgYEA5BECDVfYLejzEnOwOGSC\n" \
"L/qgDn23kWiwbfzxozSCYvP5nC3XFc7SX2X/Zvz06JefG84Ru72DKw03pLFedmN0\n" \
"4G7j0rgRQ90su4Q7rXY8SUuELeWu0f+VyjEdvC6bT7YDTgjRF6s33fl+xOqo9yDo\n" \
"+3eheuhqqj3ti5/h5itUMQ0CgYBCBZCuf5f7Chj6w+tyNtxf8UTfeWpc0C2oKN6C\n" \
"CdwbpD3XQoq9vlstdN1ujeuSoqiws3NDLAV78sqhUSi8IojG/9qhNyMyTnArIemx\n" \
"LJHPyoirzh531aMa411JPv4GZv7ouvL+feait1b7KOBRFHlc71LssjSdYmzXwT38\n" \
"VCLm0QKBgQDUSPPS3Q4ErSodrvBVI1dSjwwyO0tehbvo5zNBpkYoRnEIhoWvkx1X\n" \
"9HUHoe5n3eOAI5nczXtcKhmFDaC8ssQIjV4+5qDPF8eAFkY9k3vF+Z7i8N+wyE6Y\n" \
"dr8A55+soYHZe3XXvysN7brjAatueRV62sgkCgDT5CBLd9u67NjuLQ==\n" \
"-----END RSA PRIVATE KEY-----\n";


const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDmzCCAoOgAwIBAgIUAzZjp4Vu/62PgkxXkFcq9Ifa404wDQYJKoZIhvcNAQEL\n" \
"BQAwXTELMAkGA1UEBhMCSU4xEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoM\n" \
"GEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDEWMBQGA1UEAwwNMTkyLjE2OC44LjEw\n" \
"MTAeFw0yMjEyMDMxNjA4MDZaFw0yMzEyMDMxNjA4MDZaMF0xCzAJBgNVBAYTAklO\n" \
"MRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRz\n" \
"IFB0eSBMdGQxFjAUBgNVBAMMDTE5Mi4xNjguOC4xMDEwggEiMA0GCSqGSIb3DQEB\n" \
"AQUAA4IBDwAwggEKAoIBAQDmN9OA7DHAGd0n8J2l3eiu972Eei+ooejQHhf0dhMi\n" \
"X8/kub8UHjCJbGrp8CgNruFAUyy8pipAJpTyOzADmR+woD+cyvq99MUvG+y6KOPJ\n" \
"QYnpP8oLJNr61AEjDKTgUufimz9Xg9o4bfn967lrUXjp3aMDaCnzC89nHJ9XFb5z\n" \
"KJJutVbZNy69ebUFiIZwBLTwYEj1xMb2MbPVSX+9Y6slGUi1nOiLNRMQMAH6cnCj\n" \
"UEo+ni9U8D5XyVw7T+YThJK8tyW2kTDhWt1TfB1enliX5nnw19R4jvLbq76Zo14i\n" \
"HBwuwgFtxgUwxM6zm9QDCtTuMJ5MTVc7AnBYpi0xKGhNAgMBAAGjUzBRMB0GA1Ud\n" \
"DgQWBBR8L1/1xjSLmzlzgSNXfnL5d/JdLTAfBgNVHSMEGDAWgBR8L1/1xjSLmzlz\n" \
"gSNXfnL5d/JdLTAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQA3\n" \
"XXwLYVjbDEXzvIYeWtiXuocASLbeGD9fohNm4/IXBNvuEqfq6+hcDJ+JRKYuUdN8\n" \
"w4E38donzc/rceN+sve6FBKj2kQSa83h4v6D2ybtaR+5PjxEbSHPnFv2KAInXPmz\n" \
"X//veRSXcAmVFteGs0jYLX/32sgjJO2JYfxG23OrY2CTD51zo027xU+7jNatPMQ2\n" \
"uxtFp+zV0v7ErC3f9JvkRz3TXVnh8grPHON2qfX+P1gsZeoxEY6SwKECpb06nqOr\n" \
"4yDhJsX3a52g7uHibDETNEn949Sh4dcYgxJSnrF/VYCJozHx9z0OS/T5Mryb0C/O\n" \
"85f0i+CFz1VsP46Dh+zv\n" \
"-----END CERTIFICATE-----\n";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(endPoint, 8883, msgReceived, wiFiClient);

struct Vibration {
    const uint8_t PIN;
    bool vibrate;
};

Vibration v1 = {4, false};

//void ARDUINO_ISR_ATTR isr() {
//      v1.vibrate= true;    
//}


#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

static bool LED=false;

void setup() {
  Serial.begin(115200); 
  delay(50); 
  Serial.println();
  Serial.println("ESP32 Secure Mqtt Connection start...");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());
  Serial.println();

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.print(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);

  Serial.println("Initiating vibration...");
  pinMode(v1.PIN, INPUT_PULLUP);
  attachInterrupt(v1.PIN, isr, RISING);

  Serial.println("Initiating Temperature and humidity sensor...");
  dht.begin();
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  if (millis() - lastPublish > 5000) {
      Serial.println();
     float h = dht.readHumidity();
     // Read temperature as Celsius (the default)
     float t = dht.readTemperature();
     
     if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        //return;
     }
     
    String msg = String(t,2);     // empty string
    msg.concat(":"); msg.concat(String(h,2));
    
    boolean rc = pubSubClient.publish("sensor_data", msg.c_str());
    rc = pubSubClient.publish("vibrate_data", "N");
    Serial.println("Published sensor data..");
    //Serial.println(msg);

    //Serial.println();
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println();
    lastPublish = millis();
  }


  if (v1.vibrate) {
        Serial.println();
        Serial.println("Sensor vibrated...");
        boolean rc = pubSubClient.publish("vibrate_data", "Y");
        Serial.println("Published vibration data... Y");
        v1.vibrate = false;
        delay(1500);
    }

  #ifdef RGB_BUILTIN
    digitalWrite(RGB_BUILTIN, LED ? HIGH : LOW);
  #endif
}


void ARDUINO_ISR_ATTR isr() {
      v1.vibrate= true;    
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.print("Message received on Topic:- "); Serial.print(topic); Serial.print(": "); Serial.print((char)payload[0]);
  if ((char)payload[0] == 'Y'){
    LED = true;
    //boolean rc = pubSubClient.publish("rgb_light", "Y");
  }else{
    LED = false;
    //boolean rc = pubSubClient.publish("rgb_light", "N");
   }
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(endPoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("Secure MQTT Broker");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("rgb_light");
  }
  pubSubClient.loop();
}