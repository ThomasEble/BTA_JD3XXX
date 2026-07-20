#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <Wire.h>

I2SStream out;
BluetoothA2DPSink a2dp_sink(out);

// --- TPA6130A2 I2C control ---
#define TPA6130_ADDR   0x60   // 7-bit slave address, 1100000b per datasheet
#define TPA6130_SDA    19     // confirmed per JD3004 net assignment
#define TPA6130_SCL    18     // confirmed per JD3004 net assignment
#define TPA6130_SD_PIN 4      // placeholder - confirm against JD3004 net assignment for SD (pin 6)

void tpa6130_write(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(TPA6130_ADDR);
  Wire.write(reg);
  Wire.write(val);
  uint8_t err = Wire.endTransmission();
  if (err != 0) {
    Serial.print("TPA6130A2 I2C write failed, reg 0x");
    Serial.print(reg, HEX);
    Serial.print(" err=");
    Serial.println(err);
  }
}

void tpa6130_init() {
  pinMode(TPA6130_SD_PIN, OUTPUT);
  digitalWrite(TPA6130_SD_PIN, HIGH);   // release hardware shutdown
  delay(10);                            // datasheet: 5 ms startup time from shutdown, pad it

  Wire.begin(TPA6130_SDA, TPA6130_SCL);

  // Reg 1 (0x01): HP_EN_L=1, HP_EN_R=1, Mode[1:0]=00 (stereo), SWS=0
  tpa6130_write(0x01, 0xC0);

  // Reg 2 (0x02): Mute_L=0, Mute_R=0, Volume[5:0] = 0x2D (~-3.5 dB, conservative start)
  tpa6130_write(0x02, 0x2D);
}

void setup() {
  Serial.begin(115200);

  auto cfg = out.defaultConfig(TX_MODE);
  cfg.sample_rate = 44100;
  cfg.channels = 2;
  cfg.bits_per_sample = 16;
  cfg.pin_bck  = 26;
  cfg.pin_ws   = 25;
  cfg.pin_data = 22;
  // bump DMA buffering for more slack against underruns:
  cfg.buffer_count = 8;
  cfg.buffer_size  = 512;
  out.begin(cfg);

  tpa6130_init();

  a2dp_sink.start("BTA_JD3004");
}

void loop() {}