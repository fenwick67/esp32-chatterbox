Turn your ESP32 into a public signing board anybody can leave messages on.

The UI is simple and looks like this: [https://cdn.rawgit.com/fenwick67/esp32-chatterbox/ee65f343/index.html](https://cdn.rawgit.com/fenwick67/esp32-chatterbox/ee65f343/index.html)

# Compiling

Use the arduino toolchain, and make sure you've added `https://dl.espressif.com/dl/package_esp32_index.json` to your Boards Manager.

# Usage

Connect to the ESP32 over wifi (AP name "ChatterBox"), then navigate to a non-https URL. Note that phones will probably detect the captive portal and prompt you to "log in", this will take you to the same interface.

From there, you can leave messages as you see fit. They're put in the UI just the same as you typed them. No usernames, timestamps or anything.

If you want to clear all the messages, connect over serial (115200 baud) and send a lowercase `c`.

# TODO: 

[ ] Make SPIFFS delete old messages when we start to run out of room (but how do I detect it?)
[ ] Limit message size