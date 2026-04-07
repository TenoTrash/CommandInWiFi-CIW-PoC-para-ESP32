#include <WiFi.h>

#define MAX_SSIDS 20

String seenSSIDs[MAX_SSIDS];
int ssidCount = 0;

// 🔴 Vulnerabilidad real: format string
void log_vuln(String input) {
  char buf[128];

  // input controlado por atacante usado como formato
  snprintf(buf, sizeof(buf), input.c_str());

  Serial.print("[LOG] ");
  Serial.println(buf);
}

bool alreadySeen(String ssid) {
  for (int i = 0; i < ssidCount; i++) {
    if (seenSSIDs[i] == ssid) return true;
  }
  return false;
}

void rememberSSID(String ssid) {
  if (ssidCount < MAX_SSIDS) {
    seenSSIDs[ssidCount++] = ssid;
  }
}

void processSSID(String ssid) {
  Serial.println("\n========================");
  Serial.print("SSID detectado: ");
  Serial.println(ssid);

  log_vuln(ssid);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== IoT Device (Real Vulnerability Demo) ===");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  delay(1000);
}

void loop() {
  Serial.println("\n[SCAN] Buscando redes...");
  int n = WiFi.scanNetworks();

  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);

    if (!alreadySeen(ssid)) {
      rememberSSID(ssid);
      processSSID(ssid);
    }
  }

  delay(8000);
}
