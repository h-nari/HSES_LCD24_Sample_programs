# HSES_LCD24_Sample_programs

sample programs for HSES_LCD24(ESP-WROOM-02 + LCD module)

## RssDisp

* 登録したURLからRSSを取得し、タイトルを表示しつづけるプログラム。
* ILI9341のVertical Scroll 機能を使ったスクロール表示
* 日本語表示


## Thermometer

* 室温表示プログラム
* 時刻と4つの計測温度と平均値と温度のグラフを表示
* 温度は別のESP8266で計測されWifi経由でSDカードに書き込まれる
* 前日と当日の温度変化のグラフを表示

## WiFiScan

* WiFiをscanし、SSIDと信号強度を強度順に表示
