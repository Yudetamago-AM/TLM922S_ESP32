#include <SoftwareSerial.h>

SoftwareSerial tlmser;
const uint tlm922s_rx = 5;
const uint tlm922s_tx = 4;

uint8_t rbuf[200] = {'\0'};

void clear_rbuf() {
    for (int16_t i = 0; i < 200; i++) rbuf[i] = '\0';
}

void setup() {
    Serial.begin(115200); // パソコン・ESP32の間の通信（シリアルモニタで見れるやつ）
    Serial.println("\nTLM922S SoftwareSerial test");

    // SoftwareSerialの初期化（初期設定）
    tlm922s_init(&tlmser, tlm922s_rx, tlm922s_tx);

    // 試しに「mod get_ver」を送って応答を見てみる
    uint8_t tmp[] = "mod get_ver";
    uint8_t len = sizeof(tmp) / sizeof(uint8_t); // 配列長求める
    tlm922s_send(&tlmser, tmp, len); // コマンド送信
    delay(500);
    tlm922s_read(&tlmser, rbuf, 200); // rbufに受信したものを入れる
    
    for (int8_t i = 0; rbuf[i] != '\0'; i++) Serial.printf("%c", rbuf[i]);
    Serial.println(); // 単なる改行

    // p2p
    bool isok = false;
    isok = tlm922s_p2p_set_sync(&tlmser, 12);
    if (isok) Serial.printf("check: OK\n");
    else Serial.printf("check: NG\n");
}

void loop() {
    // do nothing
    delay(1000);
    Serial.println("end");
}