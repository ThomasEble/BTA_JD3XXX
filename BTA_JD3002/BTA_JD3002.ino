#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

void setup() {
  Serial.begin(115200);

  //Added this bit to output MONO to both GPIO25 and 26
  auto cfg = out.defaultConfig(TX_MODE);
  cfg.sample_rate = 44100;
  cfg.channels = 2;
  cfg.bits_per_sample = 16;
  out.begin(cfg);
  a2dp_sink.set_mono_downmix(true);

  a2dp_sink.start("InternalDAC");  

}


void loop() {
}
