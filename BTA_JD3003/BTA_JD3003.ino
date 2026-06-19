#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

I2SStream out;
BluetoothA2DPSink a2dp_sink(out);

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

  a2dp_sink.start("BTA_JD3003");
}

void loop() {}