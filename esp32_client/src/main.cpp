#include <Arduino.h>
#include <MQTTClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char ssid[] = "DANTAS_2G";
const char pass[] = "omegaz1z2";

const char* serverCA = R"EOF(-----BEGIN CERTIFICATE-----
MIIDuTCCAqGgAwIBAgIUHWcJH03YpZa89SWexmGGtKdLw3owDQYJKoZIhvcNAQEL
BQAwbDELMAkGA1UEBhMCQlIxHDAaBgNVBAgME1JpbyBHcmFuZGUgZG8gTm9ydGUx
DjAMBgNVBAcMBU5hdGFsMQ0wCwYDVQQKDARNRENHMRMwEQYDVQQLDApFbmdlbmhh
cmlhMQswCQYDVQQDDAJDQTAeFw0yNDA0MDUyMjIzNTJaFw0yOTA0MDUyMjIzNTJa
MGwxCzAJBgNVBAYTAkJSMRwwGgYDVQQIDBNSaW8gR3JhbmRlIGRvIE5vcnRlMQ4w
DAYDVQQHDAVOYXRhbDENMAsGA1UECgwETURDRzETMBEGA1UECwwKRW5nZW5oYXJp
YTELMAkGA1UEAwwCQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC3
iTtIjTg0xVDQKtww+uLgXCgh2GlVDEK7NjJrRiaNMvcsEA0PY9gyYaEB0g4o3GrQ
WJXCrAcGB20web8/xqn8wKRRXzaBhRoH0FbGvZBxZ/UxTLhqCCjv5sv/l4z/hzlR
qA1iFfXQXbA1W+c/EnFtJVvVipBtSN93BI8Dy64nZGaLDs1f6o8/PljTaV1lHNzi
nYoIeN9W2X5qSsUx8Jk4e8aXC6xo/sMoEqmGJPLGupRrBYeXXrCRhVp3MTwfe0MY
/Z1/wZag1DM2pcfpT23Fr9mSLv2Ji5fA6J2+PK7uKj5XHjO8h1PczjDtfXmX06yW
EWSMfX2dT+u/6D6VruuNAgMBAAGjUzBRMB0GA1UdDgQWBBTKVIEd8aOaWr22K3YS
CGnKZcjSBDAfBgNVHSMEGDAWgBTKVIEd8aOaWr22K3YSCGnKZcjSBDAPBgNVHRMB
Af8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQBB6vFxMYkUO3NxUaMleCO0FT+U
iGimxTzLijVRVoBpgJIg0v/dIllmhPZmzEKJ2NaAPqW+LfmZ2BXlWuhtfAHbzlkt
KJhpokqq38zJVqPj3wMnDdU4y6V/XeK0BTKMh5f5sTOKw2sI/LHLyxnvzcXRCW48
VmWLaqkQEtVTxI8hfdf+cF10XzVwQ6x1FsBH8ZyTv0rBoLCm0CjjuomG1EyS/LYo
yYkYXJ8hJAhFcdD71aLT1JLK2QipUI6M43Bf7L/HAq8HZhgFek47oFHzMd3T9lqz
DO9ukCTBY8BHjSFSwOdYKot1j/K0vGk7QxUPhfAx1+f2Fu32d4KREdlTwj33
-----END CERTIFICATE-----)EOF";

const char* clientCert = R"KEY(-----BEGIN CERTIFICATE-----
MIIDZjCCAk4CFFjMjU2mzIeDhtaDm5QOkBJ/sLJFMA0GCSqGSIb3DQEBCwUAMGwx
CzAJBgNVBAYTAkJSMRwwGgYDVQQIDBNSaW8gR3JhbmRlIGRvIE5vcnRlMQ4wDAYD
VQQHDAVOYXRhbDENMAsGA1UECgwETURDRzETMBEGA1UECwwKRW5nZW5oYXJpYTEL
MAkGA1UEAwwCQ0EwHhcNMjQwNDA1MjIyNjU1WhcNMjUwMzMxMjIyNjU1WjBzMQsw
CQYDVQQGEwJCUjEcMBoGA1UECAwTUmlvIEdyYW5kZSBkbyBOb3J0ZTEOMAwGA1UE
BwwFTmF0YWwxDTALBgNVBAoMBE1EQ0cxEzARBgNVBAsMCkVuZ2VuaGFyaWExEjAQ
BgNVBAMMCXB1Ymxpc2hlcjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AL/Rlk4chdfchohRnNOQ9ckAVoIob9QKwe40qlgvEJr3cxb3+JHyIln2kd7YxxaX
VrH3F5GF5yPuDYIV16XkF0kOra1JddmclfBs9zN4qV1s+weGMhmyl0Nk8qcUt5/Q
9Nrn/H5qMPjktxA0G13VFlhF5CPCiBZd5D3LYYjrvKNN2+EaTe+G5zd8ZrjQOJhs
aDXzChCqU8TKGwsL8dVhe4xQVlltGzkspnu+f99wESYm6/Pf8TiiL+jZe1avW93d
83RE1uRehdzdR57YIOFW42Hgjfl33DHRj8i+ofExIgiVE6blHCrOi9d+fkYd5cim
CaqeMj7FeLrrYnVKOp4Cbp8CAwEAATANBgkqhkiG9w0BAQsFAAOCAQEApdQctNmW
ZiyDTmVZ+OsKrAqDFwzHdpst7jxpDhfsr8CAV4QQ2v3UzZltHYtVybYQGILLu+Fn
5aM2zfXWhHi+x6yElgINBopF2DIfBUrffriGAmSj3xbxk8U4oiUxB30gKn9m1Uxb
o93ez4m6jXkIfP2wgkIVbwDdMgGYV30KJfA3MuoAXGTxEeBu9gCXyB2Rzu77u85g
D44+pcz/vt2db+CNajUnZWGeJw5V1rdFi78rZ9BtJv/cAgUsaH9LaD1b6jbLXEzZ
oxBY4sB+cmfWXNo2g9KxHvyfy5aDWsGJdJ9bi81KbDMOsaEzNjlbTXJISs0Rmd+Y
pXMDVo0qedwrtg==
-----END CERTIFICATE-----)KEY";

const char* clientKey = R"KEY(-----BEGIN PRIVATE KEY-----
MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQC/0ZZOHIXX3IaI
UZzTkPXJAFaCKG/UCsHuNKpYLxCa93MW9/iR8iJZ9pHe2McWl1ax9xeRhecj7g2C
Fdel5BdJDq2tSXXZnJXwbPczeKldbPsHhjIZspdDZPKnFLef0PTa5/x+ajD45LcQ
NBtd1RZYReQjwogWXeQ9y2GI67yjTdvhGk3vhuc3fGa40DiYbGg18woQqlPEyhsL
C/HVYXuMUFZZbRs5LKZ7vn/fcBEmJuvz3/E4oi/o2XtWr1vd3fN0RNbkXoXc3Uee
2CDhVuNh4I35d9wx0Y/IvqHxMSIIlROm5RwqzovXfn5GHeXIpgmqnjI+xXi662J1
SjqeAm6fAgMBAAECggEAEx5smPguflT8fcgwqu+3Enk7piPUqEcINwS9zntRGuO7
b23OM0cglbnwpncR5Ou3AuxUx4xCp79O+QknTOFz/AlVVevNWMrK28Nooj3jwlbz
RbfiSihq+5crY8GPXy5gcWnlA06fTKIzUuShWP69CEBdxxNZJW2SAHdt5HamqdoT
y5vyckz/+5Z4p0zueqC8XsjO6OqPxX7+bUDS2Lw4hvN7KSmhwfn8s4gaJO9hSXp5
0XL848wIaoI1lCpp7oaBjwVWCaAh9SjK8cEEGyObeCHLM4zF20/kLTTkvdjVCt0V
MENE50aogH/fqZnTjtZYT3IU+Xzj3tDukG0Pph5PJQKBgQDjRUruz1+nXvGyIAuL
RNTISep9Nzoesu9N6uDcYUBaw/idKWxeOfTN9besr/LCyRTgjuxand47E33Gc73x
LaQzB0YhFg3ubxOab9vRdudu3FU9eCn/dGn/p3lSXLpBPEoH1KIpl6O58SFqzbDL
DMaNDrTjzEJ7FoHHqKeWlGXUnQKBgQDYEQhsq1j9p+oeZqIvUdVWRoEtkWuHnAzZ
zPntojOxs/ToAG1QB5G4/nDwmmnq6OLO4V6yOQo+N2HbKT0DAVv35iuMbbkEsa6u
4UWkNjZD48SScWgkrXNkexR33yBRGQYJMES+araq33HcQHMEGEgoxM+gaXAUj4nF
mDgreuGFawKBgDM/WshC5rTdCe86np/G45xOXpvhqi9l9eRCBHZtT4+oBRWbU55j
vzn6IHLGMh2OLjD8yXsdghEcMg5f5wLNYetHxp6LVJThYzA97PlssH8lG+uCfyXl
YEmPwCG8Wxt+tsrRPcfXM8on/3sP6r++n78LA+siEhOjXZxiAXhI+jVZAoGAenIM
qCjiob1K/DeFqfNiRyZlhoFmws84qSV0basCSK6HGANF75iaiJAJSRD4/VtFTaCC
ndWdbGzOU/kmzlEy4WXbQBXeSD65k/muEcV3Mzoc6DDEqCRzxHnLu7MDCXJ47TQh
gRfQtT0ztXIAvE1Wd2QxleKq1bwSg5wkGN/mdkkCgYA62uIRP5FIBfLkOf0n/iiZ
YwsAIbZcFhOFq4BLfVPo4iI0VGKbJ3oLQOJssqUfp9jolDIoG43JiRhMaJilWhF0
e0Vd9O2tyLinX1qxOTxHA8UKP4BoUyAnegFSM7Awbct8ClHbfnM/L/T3pdgZvovT
WDNRGcSuIHmTG4KFByv8lQ==
-----END PRIVATE KEY-----)KEY";

WiFiClientSecure net;
MQTTClient client;

unsigned long lastMillis = 0;

void messageReceived(String &topic, String &payload) {
  /**
   * MQTT Client message recived callback.
   */
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);

  // Connect to the WiFi
   WiFi.begin(ssid, pass);

  Serial.print("Checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
 Serial.println("WiFi connected!");

  // Configure WiFiClientSecure with the required certs
  net.setCACert(serverCA);
 net.setCertificate(clientCert);
 net.setPrivateKey(clientKey);

  // Set a timeout for our WiFiClient so it doesn't hang on disconnect
  net.setTimeout(5);
  
  // Start the mqtt client
  client.begin("andre.local", 8883, net);
  
  // Set the mqtt client message callback
 client.onMessage(messageReceived);

  // Connect to the mqtt broker
  while (!client.connect("myclient", "public", "public")) {
  Serial.print(".");
  delay(1000);
 }

 Serial.println("connected!");

  // Subscribe to the relevant topics
 client.subscribe("/test");
}

void loop() {
  // Run the mqtt client loop (handles recived messages - runs keep alive)
  client.loop();

  // Check that client is still connected - if not reconnect
  if (!client.connected()) {
    while (!client.connect("myclient", "public", "public")) {
      Serial.print(".");
      delay(1000);
   }
    Serial.println("Reconnected to MQTT Broker!");
  }

  // Publish a message to a topic every second
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    client.publish("/test", "hello world");
  }
}