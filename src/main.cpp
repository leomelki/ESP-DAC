#include <Arduino.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"


AnalogAudioStream out;
Equilizer3Bands eq(out);
BluetoothA2DPSink a2dp_sink;

ConfigEquilizer3Bands cfg_eq;

// In the setup function:
void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
    Serial.printf("AVRC metadata rsp: attribute id 0x%x, %s\n", data1, data2);
}

// Write data to AudioKit in callback
void read_data_stream(const uint8_t *data, uint32_t length) {
    eq.write(data, length);
}

void setup() {
    Serial.begin(115200);
    
    auto cfg = out.defaultConfig();
    out.begin(cfg);

    // setup equilizer
    cfg_eq = eq.defaultConfig();
    cfg_eq.setAudioInfo(cfg); // use channels, bits_per_sample and sample_rate from out config
    cfg_eq.freq_low = 40;
    cfg_eq.freq_high = 2500;
    cfg_eq.gain_low = 0.5;
    cfg_eq.gain_medium = 1;
    cfg_eq.gain_high = 0.6;
    eq.begin(cfg_eq);

    a2dp_sink.set_stream_reader(read_data_stream, false);
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_auto_reconnect(false);
    a2dp_sink.start("Leo's Miniboom");
}
void loop() {
    delay(100);
}