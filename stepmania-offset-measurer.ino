#define AUDIO_PIN PA1
// you can only have one of these turned on. KEYBOARD for actual use, SERIAL for debug
#define KEYBOARD 1
#define SERIAL 0

int threshold = 700; // you might need to tweak this based on your volume, noise floor, etc

#if KEYBOARD
#include <Keyboard.h>
#endif

void blink() {
  digitalWrite(LED_BUILTIN, LOW); // low is LIT, high is UNLIT
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AUDIO_PIN, INPUT);
  pinMode(USER_BTN, INPUT_PULLUP);
  #if KEYBOARD
  Keyboard.begin();
  #endif

  #if SERIAL
  Serial.begin(38400);
  #endif
  analogReadResolution(12);
  for (int i = 0; i < 4; ++i) {
    blink();
  }  
}

bool enabled = false;
int lastBtn = HIGH; // high is unpressed
void loop() {
  int btn = digitalRead(USER_BTN);
  if (lastBtn == HIGH && btn == LOW) {
    enabled = !enabled;
    int blinkCount = 1;
    if (!enabled) ++blinkCount;
    for(int i = 0; i < blinkCount; ++i) {
      blink();
    }
    
    #if SERIAL
    Serial.println("toggle");
    #endif
  }
  lastBtn = btn;

  int audioVal = analogRead(AUDIO_PIN);
  
  if (audioVal > threshold) {
    #if SERIAL
    Serial.println(audioVal);    
    #endif
    #if KEYBOARD
    if(enabled) {
      Keyboard.write('a');
    }
    #endif
    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
    digitalWrite(LED_BUILTIN, HIGH);
  }  

  #if SERIAL
  Serial.flush();
  #endif   

}
