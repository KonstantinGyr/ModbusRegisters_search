#include <Arduino.h>
#include <ModbusRTUMaster.h>
#define SOFTWARE_SERIAL
#include <SoftwareSerial.h>

#define MODBUS_SERIAL mySerial
#define MODBUS_CONFIG SERIAL_8N1

const int8_t rxPin = 10;
const int8_t txPin = 11;
const int8_t dePin = 13;

unsigned long transactionCounter = 0;
unsigned long errorCounter = 0;
uint16_t inputRegisters[2];

const char* errorStrings[] = {
  "success",
  "invalid id",
  "invalid buffer",
  "invalid quantity",
  "response timeout",
  "frame error",
  "crc error",
  "unknown comm error",
  "unexpected id",
  "exception response",
  "unexpected function code",
  "unexpected response length",
  "unexpected byte count",
  "unexpected address",
  "unexpected value",
  "unexpected quantity"
};

SoftwareSerial mySerial(rxPin, txPin);

ModbusRTUMaster modbus(MODBUS_SERIAL, dePin);

 void printLog(uint8_t unitId, uint8_t functionCode, uint16_t startingAddress, uint16_t quantity, uint8_t error) {
  transactionCounter++;
  if (error) errorCounter++;
  char string[128];
  sprintf(string, "%ld %ld %02X %02X %04X %04X %s", transactionCounter, errorCounter, unitId, functionCode, startingAddress, quantity, errorStrings[error]);
  Serial.print(string);
  if (error == MODBUS_RTU_MASTER_EXCEPTION_RESPONSE) {
    sprintf(string, ": %02X", modbus.getExceptionResponse());
    Serial.print(string);
  }
  Serial.println();
} 




void setup() {
  Serial.begin(9600);
 
  MODBUS_SERIAL.begin(9600);
  modbus.begin(9600, MODBUS_CONFIG);

  
}

void loop() {
  uint8_t error;
  error = modbus.readInputRegisters(1, 0, inputRegisters, 2);
  /*unitId: номер идентификатора устройства, на которое необходимо отправить этот запрос.
   Допустимые значения: 1- 246.
   startAddress: адрес первого входного регистра для чтения. Допустимые типы данных: uint16_t.
   buffer: массив, в который помещаются считанные значения входных регистров. Допустимые типы данных:
   массив uint16_t.
   quantity: количество значений входного регистра для чтения.
   Это значение не должно быть больше размера массива. Допустимые типы данных: uint16_t.*/

  printLog(1, 4, 0, 2, error);
}
