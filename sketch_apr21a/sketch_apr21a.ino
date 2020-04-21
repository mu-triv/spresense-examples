int i = 0;
bool on = true;
const int nbr_leds = 4;
char* names[nbr_leds] = {LED0, LED1, LED2, LED3};

void setup() {
  for (int idx = 0; idx < nbr_leds; idx++) {
    pinMode(names[idx], OUTPUT);
  }
}

void loop() {
  
  for (int idx = 0; idx < nbr_leds; idx++) {
    if (idx <= i){
      digitalWrite(names[idx], on);
    }
  }
  i = (i + 1) % nbr_leds;
  if (i == 0) {
    on = !on;
  }
  delay(100);  
}
