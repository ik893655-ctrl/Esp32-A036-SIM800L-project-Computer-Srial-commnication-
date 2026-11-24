
/*
  ESP32 + SIM800L TTGO T-Call AM036
  GSM Browser Phone Firmware

  USB Serial: 115200
  SIM800L UART: 9600
  RX: GPIO26
  TX: GPIO27
*/

#define MODEM_RX       26
#define MODEM_TX       27
#define MODEM_PWRKEY    4
#define MODEM_POWER_ON 23
#define MODEM_RST       5

HardwareSerial SIM800(1);

String inputCommand = "";

void modemPowerOn() {
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);

  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_RST, HIGH);

  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);

  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(1000);

  digitalWrite(MODEM_PWRKEY, LOW);

  delay(5000);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("ESP32 GSM BROWSER PHONE");
  Serial.println("Starting SIM800L...");

  modemPowerOn();

  SIM800.begin(
    9600,
    SERIAL_8N1,
    MODEM_RX,
    MODEM_TX
  );

  delay(2000);

  SIM800.println("AT");
  delay(1000);

  SIM800.println("ATE0");
  delay(500);

  SIM800.println("AT+CMEE=2");
  delay(500);

  SIM800.println("AT+CLIP=1");
  delay(500);

  SIM800.println("AT+CSCS=\"GSM\"");
  delay(500);

  SIM800.println("AT+CSQ");
  delay(500);

  SIM800.println("AT+CREG?");
  delay(500);

  Serial.println();
  Serial.println("GSM MODEM READY");
  Serial.println("USB SERIAL READY");
  Serial.println("--------------------------------");
}

void loop() {

  // Browser / Laptop -> SIM800L
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputCommand.length() > 0) {
        SIM800.println(inputCommand);

        Serial.print(">> ");
        Serial.println(inputCommand);

        inputCommand = "";
      }
    } else {
      if (inputCommand.length() < 200) {
        inputCommand += c;
      }
    }
  }

  // SIM800L -> Browser / Laptop
  while (SIM800.available()) {
    char c = SIM800.read();
    Serial.write(c);
  }
}
