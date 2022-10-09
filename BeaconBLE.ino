#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include "BLEBeacon.h"

/* GPIO to indicate operation (breathing light) */
#define GPIO_BREATHING_LIGHT 2
/* BLE Beacon UUID (128 bit) */
#define BEACON_UUID "2fc03570-8ae7-407f-a375-3d2d74d8fc0f"
/* IMPORTANT: the manufacturer code/ID is a unique identifier, registered in the Bluetooth SIG
* (Special Interest Group). Therefore, it is an identifier globally known. For testing purposes
* use any ID but, in case of making a commercial product, NEVER USE A COMPANY ID OR MANUFACTURER
* OTHER THAN YOURS. This could lead to legal problems. Here, in this example, the purpose is learning only.
* --- List with manufacturers registered in SIG ---
* https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
* Important: the code must be used in that program with different endian than offered in the list
* For example, if the manufacturer is listed as 0x1234 in the list, it should be used here as 0x3412.
*/
#define ID_FABRICANTE_BEACON 0x4C00
/* Beacon Major and Minor (assuming 1 group (major) with 1 beacon only (minor) */
#define MAJOR_BEACON 1
#define MINOR_BEACON 1
/* Beacon data (type, manufacturer, uuid, major, minor and tx power) */
#define BEACON_DATA ""
#define BEACON_DATA_SIZE 26
/* Info: https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile/ */
#define BEACON_DATA_TYPE 0xFF
/* Objeto global para gerar o advertising do BLE */
BLEAdvertising *pAdvertising;  // BLE Advertisement type
/* Prototypes */
void configura_beacon(void);


void configura_beacon(void) {
  /* Create an object to manage the Beacon */
  BLEBeacon ble_beacon = BLEBeacon();
  /* manufacturer ID unique identifier */
  ble_beacon.setManufacturerId(ID_FABRICANTE_BEACON);
  /* Configure Beacon proximity, major and minor */
  ble_beacon.setProximityUUID(BLEUUID(BEACON_UUID));
  ble_beacon.setMajor(MAJOR_BEACON);
  ble_beacon.setMinor(MINOR_BEACON);
  /* Configure BLE advertiser */
  BLEAdvertisementData advertisement_data = BLEAdvertisementData();
  BLEAdvertisementData scan_response_data = BLEAdvertisementData();
  /* Indicates Classic Bluetooth should not be supported */
  advertisement_data.setFlags(0x04);
  /* Reporting beacon data */
  std::string strServiceData = "";
  strServiceData += (char)BEACON_DATA_SIZE;
  strServiceData += (char)BEACON_DATA_TYPE;
  strServiceData += ble_beacon.getData();
  advertisement_data.addData(strServiceData);
  /* configure data information to be sent by the beacon
   * and scan information in the advertiser */
  pAdvertising->setAdvertisementData(advertisement_data);
  pAdvertising->setScanResponseData(scan_response_data);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Doing beacon initialization...");
  /* Configure breathing light */
  pinMode(GPIO_BREATHING_LIGHT, OUTPUT);
  digitalWrite(GPIO_BREATHING_LIGHT, LOW);
  /* Create and configure a BLE device and server */
  BLEDevice::init("ESP32 - Beacon BLE");
  BLEServer *pServer = BLEDevice::createServer();
  /* Initialize and configure advertising */
  pAdvertising = BLEDevice::getAdvertising();
  BLEDevice::startAdvertising();
  configura_beacon();
  /* Starts working as beacon (advertiser kicks in) */
  pAdvertising->start();
  Serial.println("The beacon has been initialized and is already operating.");
}

void loop() {
  digitalWrite(GPIO_BREATHING_LIGHT, LOW);
  delay(500);
  digitalWrite(GPIO_BREATHING_LIGHT, HIGH);
  delay(500);
}