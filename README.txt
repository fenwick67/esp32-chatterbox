Turn your ESP32 into a public signing board anybody can post to.

how to make changes to html:

1. make sure javascript lines end with semicolons (newlines and tabs get slurped up so ASI won't work)
2. run build.js
3. recompile esp32_chatterbox.ino

# TODO: 

* Make SPIFFS delete old messages when we run out of room (but how do I detect it?)