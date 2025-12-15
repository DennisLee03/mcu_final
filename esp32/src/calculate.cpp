#include "calculate.h"
#include "heart_rate.h"

// --- HRV (rMSSD) 變數宣告開始 ---
const byte HRV_WINDOW_SIZE = 20; // 採樣最近 20 次心跳差異來計算
long diffSquaredBuffer[HRV_WINDOW_SIZE]; // 儲存 (RR差異)^2 的陣列
byte hrvSpot = 0;
byte hrvCount = 0; // 記錄目前收集了幾筆資料
long prevDelta = 0; // 記錄上一次的心跳間隔
float rmssd = 0.0; // 最終的 HRV 數值
// --- HRV (rMSSD) 變數宣告結束 ---

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float temperature;
float beatsPerMinute;
int beatAvg;
long irValue;
String calc_rmssd(uint32_t data) {
  irValue = data;   // infrared light intensity
  if (irValue < 4000) {
      beatsPerMinute = 0;
      beatAvg = 0;
      // 重置 HRV 相關變數
      hrvCount = 0;
      hrvSpot = 0;
      prevDelta = 0;
      rmssd = 0;
  }
  if (checkForBeat(irValue) == true)
  {
    float prev_beatsPerMinute = beatsPerMinute;
    int prev_beatAvg = beatAvg;
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    // --- HRV 計算邏輯開始 ---
    // 只有當 delta 在合理範圍內（例如 60bpm~180bpm 之間，即 330ms~1000ms）才計算，過濾雜訊
    // 這裡放寬一點：250ms (240bpm) 到 2000ms (30bpm)
    if (delta > 250 && delta < 2000) {
        
        // 我們需要"連續"兩次的心跳間隔才能計算差異
        if (prevDelta > 0) {
            long diff = delta - prevDelta; // 步驟1: 計算相鄰差值
            long squaredDiff = diff * diff; // 步驟2: 平方

            // 存入陣列 (Circular Buffer)
            diffSquaredBuffer[hrvSpot++] = squaredDiff;
            hrvSpot %= HRV_WINDOW_SIZE; // 循環索引
            if (hrvCount < HRV_WINDOW_SIZE) hrvCount++;

            // 步驟3 & 4: 計算平均值並開根號
            if (hrvCount >= 2) { // 至少要有2個數據點
                unsigned long sumSquared = 0;
                for (byte i = 0; i < hrvCount; i++) {
                    sumSquared += diffSquaredBuffer[i];
                }
                // rMSSD 公式 = sqrt(平均值(差值平方))
                rmssd = sqrt(sumSquared / hrvCount);
            }
        }
        prevDelta = delta; // 更新 prevDelta 供下一次使用
    }
    // --- HRV 計算邏輯結束 ---

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
      rateSpot %= RATE_SIZE; // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    // set a threshold to avoid fluctuation
    if (prev_beatsPerMinute > beatsPerMinute + 30) beatsPerMinute = prev_beatsPerMinute;
    if (prev_beatAvg > beatAvg + 30) beatAvg = prev_beatAvg;


  }

  String result;
  result += "IR=";
  result += irValue;
  result += ", BPM=";
  result += beatsPerMinute;
  result += ", Avg BPM=";
  result += beatAvg;
  result += ", rMSSD=";
  result += rmssd;

  Serial.println(result);

  return result;
}