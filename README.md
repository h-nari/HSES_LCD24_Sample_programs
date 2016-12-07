# HSES_LCD24_Sample_programs

sample programs for HSES_LCD24(ESP-WROOM-02 + LCD module)

## RssDisp


<a href="https://github.com/h-nari/HSES_LCD24_Sample_programs/blob/master/img/161107a6m.jpg?raw=true"><img src="https://github.com/h-nari/HSES_LCD24_Sample_programs/blob/master/img/161107a6m.jpg?raw=true" align="left" hspace="40" width="300"/></a>

* 登録したURLからRSSを取得し、タイトルを表示しつづけるプログラム。
* ILI9341のVertical Scroll 機能を使ったスクロール表示
* 日本語表示

<br clear="left"/>

## Thermometer

<a href="https://github.com/h-nari/HSES_LCD24_Sample_programs/blob/master/img/161124a1i.jpg?raw=true"><img src="https://github.com/h-nari/HSES_LCD24_Sample_programs/blob/master/img/161124a1i.jpg?raw=true" width="300" align="left" hspace="40"/></a>

* 室温表示プログラム
* 時刻と4つの計測温度と平均値と温度のグラフを表示
* 温度は別のESP8266で計測されWifi経由でSDカードに書き込まれる
* 前日と当日の温度変化のグラフを表示

<br clear="left"/>

## WiFiScan

<a href="https://raw.githubusercontent.com/h-nari/HSES_LCD24_Sample_programs/master/img/161020a3.jpg"><img src="https://raw.githubusercontent.com/h-nari/HSES_LCD24_Sample_programs/master/img/161020a3.jpg" width="300"/ align="left" hspace="40"></a>

* WiFiをscanし、SSIDと信号強度を強度順に表示
* 日本語SSIDにも対応

<br clear="left"/>

## 別途インストールが必要なライブラリの入手先

Arduino-IDEのライブラリ管理でインストールできるものもありますが、直接githubから最新版をgit cloneするか、zipファイルをダウンロードしArduino-IDEの機能でインストールするほうが安全です。

* Adafruit_GFX
 * 小型表示デバイス用描画ライブラリ、直線、図形、文字描画など
 * https://github.com/adafruit/Adafruit-GFX-Library
* Adafruit_ILI9341
 * コントローラにILI9341を使用したLCD用描画ライブラリ、Adafruit_GFXを継承
 * https://github.com/adafruit/Adafruit_ILI9341
 * バグ修正の必要有
  * Thermometerが動かない
  * https://github.com/adafruit/Adafruit_ILI9341/pull/12
* Fontx
 * Adafruit_GFX非依存, Fontxライブラリ
 * https://github.com/h-nari/Fontx
* FontxGfx
 * Adafruit_GFX向け、Fontx描画機能ライブラリ
 * https://github.com/h-nari/FontxGfx
* Humblesoft_ILI9341
 * Adafruit_ILI9341に漢字表示機能、スクロール機能などを追加
 * https://github.com/h-nari/Humblesoft_ILI9341
* XPT2046_Touchscreen
 * タッチスクリーン・インターフェースXPT2046ライブラリ
 * https://github.com/PaulStoffregen/XPT2046_Touchscreen
* LcdTouchscreen
 * タッチスクリーンライブラリ、校正(calibration)機能など
 * https://github.com/h-nari/LcdTouchscreen
* Params
 * タッチスクリーンの校正(calibration)結果をeepromに保存・呼び出すのに使用
 * https://github.com/h-nari/Params
