#include <SoftwareSerial.h>
#include <string.h>

/// @brief SoftwareSerialの読み出しバッファをクリアする
static void _clear_buf(SoftwareSerial *ss) {
    if (ss->available()) {
        while(ss->available()) {
            uint8_t ch = (uint8_t)ss->read(); // 最適化よけのつもり
        }
    }
}

/// @brief tml922s用のSoftwareSerialを初期化する
/// @param ss SoftwareSerialのバスのアドレス
/// @param p_rx RXピンのピン番号
/// @param p_tx TXピンのピン番号
void tlm922s_init(SoftwareSerial *ss, uint8_t p_rx, uint8_t p_tx) {
    delay(100);
    ss->begin(115200, SWSERIAL_8N1, p_rx, p_tx, false, 256);
    delay(500);
    ss->enableTx(false);

#if 0
    ss->enableTx(true);
    
    for (int8_t i = 0; i < len; i++) Serial.printf("%c", tmp[i]);
    ss->write(tmp, len);
    ss->enableTx(false);

    //delay(1000);
    Serial.println("get:");
    if (ss->available()) {
        while(ss->available()) {
            byte ch = (byte)ss->read();
            Serial.printf("%c", ch);
        }
        Serial.println();
    }
#endif
}

/// @brief 任意のコマンドをSoftwareSerialで送る
/// @param ss SoftwareSerialのバスのアドレス
/// @param str 送る文字列（CRは不要）。
/// @param len 送る文字列の長さ。呼び出す側でsizeof等で求める。
void tlm922s_send(SoftwareSerial *ss, uint8_t *src, size_t len) {
    _clear_buf(ss);

    ss->enableTx(true);
    ss->write(src, len);
    ss->write('\r');
    ss->enableTx(false);
}

/// @brief TLM922Sの応答を読み出す
/// @param ss SoftwareSerialのバスのアドレス
/// @param dst 受信する文字列バッファのアドレス。
/// @param max 文字列バッファ（dst）の長さ。これを超えるものは無視される。
void tlm922s_read(SoftwareSerial *ss, uint8_t *dst, size_t max) {
    int16_t i = 0;
    if (ss->available()) {
        while (ss->available() && (i < max)) {
            dst[i] = (uint8_t)ss->read();
            i++;
        }
    }

    //_clear_buf(ss);
}

/// @brief p2p通信におけるsync wordを設定する
/// @param ss SoftwareSerialのバスのアドレス
/// @param sync_word 0x00から0xFFまでの範囲
/// @return 正常な応答があったらtrue、異常な応答ならfalse（未実装）
bool tlm922s_p2p_set_sync(SoftwareSerial *ss, uint8_t sync_word) {
    // クソコード、まねすべきでない
    char tobesent[20] = {'\0'};
    char tmp[] = "p2p set_sync ";
    char sync_word_c[2];
    char rbuf[30];
    char ok[] = "Ok";
    snprintf(sync_word_c, 3, "%d", sync_word);
    snprintf(tobesent, 16, "%s%s", tmp, sync_word_c);
    tlm922s_send(ss, (uint8_t *)tobesent, 15);
    //Serial.printf("%s\n", tobesent);
    delay(1000);
    tlm922s_read(ss, (uint8_t *)rbuf, 30);
    delay(500);
    //Serial.printf("rbuf: %s\n", rbuf);
    //if (strncmp(&rbuf[6], ok, 5) == 0) return true;
    //return false;
    return true;
}

