#include <stdarg.h>
#include <EEPROM.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_BUZZER                A0
#define PIN_BUTTON_UP             A1
#define PIN_BUTTON_DOWN           A2
#define PIN_BUTTON_ACTION         A3
#define PIN_VALVE_AIR             2
#define PIN_VALVE_C02             3
#define PIN_VALVE_WATER           4
#define PIN_VALVE_CLEANER_IN      5
#define PIN_VALVE_SANITIZER_IN    6
#define PIN_VALVE_CLEANER_OUT     7
#define PIN_VALVE_SANITIZER_OUT   8
#define PIN_VALVE_DRAIN           9
#define PIN_PUMP                  10
#define PIN_LED                   11

#define DISPLAY_I2C_ADDRESS       0x27

#define CTRL_WATER          0b000000001
#define CTRL_CLEANER_IN     0b000000010
#define CTRL_SANITIZER_IN   0b000000100
#define CTRL_AIR            0b000001000
#define CTRL_CO2            0b000010000
#define CTRL_DRAIN          0b000100000
#define CTRL_CLEANER_OUT    0b001000000
#define CTRL_SANITIZER_OUT  0b010000000
#define CTRL_PUMP           0b100000000

#define CONFIG_DRAIN            (CTRL_DRAIN)
#define CONFIG_RINCE            (CTRL_PUMP + CTRL_WATER + CTRL_DRAIN)
#define CONFIG_RINCE_PURGE      (CTRL_AIR + CTRL_DRAIN)
#define CONFIG_RINCE_PURGE_CO2  (CTRL_CO2 + CTRL_DRAIN)
#define CONFIG_CLEAN            (CTRL_PUMP + CTRL_CLEANER_IN + CTRL_CLEANER_OUT)
#define CONFIG_CLEAN_PURGE      (CTRL_AIR + CTRL_CLEANER_OUT)
#define CONFIG_SANITIZE         (CTRL_PUMP + CTRL_SANITIZER_IN + CTRL_SANITIZER_OUT)
#define CONFIG_SANITIZE_PURGE   (CTRL_AIR + CTRL_SANITIZER_OUT)
#define CONFIG_PRESSURIZE       (CTRL_CO2)
#define CONFIG_END              0

#define LED_BLINK_PERIOD    2

typedef struct step_s {
  unsigned int config;
  int duration;
} step_t;

typedef struct mode_s {
  char *name;
  step_t *steps;
} mode_t;

step_t STEPS_DRAIN[] = {
  {CONFIG_DRAIN, 3},
  {CONFIG_END, 0}
};

step_t STEPS_KEG_FULL[] = {
  {CONFIG_DRAIN, 10},
  {CONFIG_RINCE, 10},
  {CONFIG_RINCE_PURGE, 10},
  {CONFIG_CLEAN, 10},
  {CONFIG_CLEAN_PURGE, 10},
  {CONFIG_RINCE, 10},
  {CONFIG_RINCE_PURGE, 10},
  {CONFIG_SANITIZE, 10},
  {CONFIG_SANITIZE_PURGE, 10},
  {CONFIG_RINCE, 10},
  {CONFIG_RINCE_PURGE_CO2, 10},
  {CONFIG_PRESSURIZE, 10},
  {CONFIG_END, 0}
};

step_t STEPS_DRAFT[] = {
  {CONFIG_RINCE, 10},
  {CONFIG_RINCE_PURGE, 10},
  {CONFIG_SANITIZE, 10},
  {CONFIG_SANITIZE_PURGE, 10},
  {CONFIG_RINCE, 10},
  {CONFIG_RINCE_PURGE, 10},
  {CONFIG_END, 0}
};

mode_t MODES[] = {
  {"Vidange", STEPS_DRAIN},
  {"Fut", STEPS_KEG_FULL},
  {"Tireuse", STEPS_DRAFT}
};

int MODES_NUMBER = sizeof(MODES) / sizeof(mode_t);

Bounce buttonUp = Bounce(); 
Bounce buttonDown = Bounce(); 
Bounce buttonAction = Bounce(); 

LiquidCrystal_I2C lcd(DISPLAY_I2C_ADDRESS, 16, 2);

#define CHAR_UP_DOWN  1
byte CHAR_UP_DOWN_SETUP[] = {
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100
};

#define EEPROM_ADDRESS_MODE  0

typedef enum state_e {
  STATE_SELECT,
  STATE_SELECT_UPDATE,
  STATE_RUN,
  STATE_RUN_UPDATE,
  STATE_TERMINATE,
  STATE_CANCEL
} state_t;


state_t state = STATE_SELECT;

int mode = 0;
int mode_start_time;
int mode_full_time;

int step;
int step_start_time;

void lcd_printf(char *fmt, ...)
{
  char buf1[17];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf1, sizeof(buf1)-1, fmt, args);
  va_end(args);

  char buf2[17];
  snprintf(buf2, sizeof(buf2)-1, "%-16s", buf1);
  lcd.print(buf2);
}

void select()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd_printf("Mode :");
  lcd.setCursor(0, 1);
  lcd_printf("%c %s", CHAR_UP_DOWN, MODES[mode].name);
  
  digitalWrite(PIN_LED, HIGH);

  state = STATE_SELECT_UPDATE;
}

void select_update()
{
  int new_mode = mode;

  buttonUp.update();
  if( buttonUp.fell() ) {
    new_mode--;
  }
  
  buttonDown.update();
  if( buttonDown.fell() ) {
    new_mode++;
  }
  
  buttonAction.update();
  if( buttonAction.fell() ) {
    state = STATE_RUN;
  }

  if( new_mode < 0 ) {
    new_mode = MODES_NUMBER - 1;
  }
  else if( new_mode >= MODES_NUMBER ) {
    new_mode = 0;
  }

  if( new_mode != mode ) {
    mode = new_mode;
    lcd.setCursor(0, 1);
    lcd_printf("%c %s", CHAR_UP_DOWN, MODES[mode].name);
  }
}

int seconds()
{
  return millis() / 1000;
}

void controls_set(unsigned int config)
{
  digitalWrite(PIN_VALVE_AIR, !(config & CTRL_AIR));
  digitalWrite(PIN_VALVE_C02, !(config & CTRL_CO2));
  digitalWrite(PIN_VALVE_WATER, !(config & CTRL_WATER));
  digitalWrite(PIN_VALVE_CLEANER_IN, !(config & CTRL_CLEANER_IN));
  digitalWrite(PIN_VALVE_SANITIZER_IN, !(config & CTRL_SANITIZER_IN));
  digitalWrite(PIN_VALVE_CLEANER_OUT, !(config & CTRL_CLEANER_OUT));
  digitalWrite(PIN_VALVE_SANITIZER_OUT, !(config & CTRL_SANITIZER_OUT));
  digitalWrite(PIN_VALVE_DRAIN, !(config & CTRL_DRAIN));
  digitalWrite(PIN_PUMP, !(config & CTRL_PUMP));
}

unsigned int step_set(int index)
{
  step = index;
  step_start_time = seconds();
  
  controls_set(MODES[mode].steps[step].config);
  
  return MODES[mode].steps[step].config;
}

void run()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd_printf(MODES[mode].name);
  lcd.setCursor(0, 1);
  lcd_printf("Preparation");
  
  int saved_mode = EEPROM.read(EEPROM_ADDRESS_MODE);
  if( mode != saved_mode ) {
    EEPROM.write(EEPROM_ADDRESS_MODE, mode);
  }

  mode_start_time = seconds();

  mode_full_time = 0;
  for(int i=0 ; MODES[mode].steps[i].config != CONFIG_END ; i++ ) {
    mode_full_time += MODES[mode].steps[i].duration;
  }

  step_set(0);

  state = STATE_RUN_UPDATE;
}

void run_update()
{
  buttonAction.update();
  if( buttonAction.fell() ) {
    state = STATE_CANCEL;
    return;
  }

  int step_running_time = seconds() - step_start_time;
  if( step_running_time >= MODES[mode].steps[step].duration ) {
    unsigned int config = step_set( step + 1 );
    if( config == CONFIG_END ) {
      state = STATE_TERMINATE;
      return;
    }
  }

  int mode_running_time = seconds() - mode_start_time;
  int rtime_mn = mode_running_time / 60;
  int rtime_s = mode_running_time % 60;
  int ftime_mn = mode_full_time / 60;
  int ftime_s = mode_full_time % 60;

  lcd.setCursor(0, 1);
  lcd_printf(" %dmn%02d / %dmn%02d", rtime_mn, rtime_s, ftime_mn, ftime_s);

  if( mode_running_time % LED_BLINK_PERIOD < LED_BLINK_PERIOD/2 ) {
    digitalWrite(PIN_LED, HIGH);
  }
  else {
    digitalWrite(PIN_LED, LOW);
  }

}

void terminate()
{
  controls_set(0);

  digitalWrite(PIN_LED, LOW);
  
  lcd.setCursor(0, 1);
  lcd_printf(" Termine");
  
  for( int i=0 ; i<3 ; i++ ) {
    tone(PIN_BUZZER, 1760, 800);
    delay(1000);
  }

  state = STATE_SELECT;
}

void cancel()
{
  controls_set(0);
  
  digitalWrite(PIN_LED, LOW);
  
  lcd.setCursor(0, 1);
  lcd_printf(" Annule");
  
  tone(PIN_BUZZER, 1760, 800);
  delay(1000);
  
  state = STATE_SELECT;
}


void setup()
{
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ACTION, INPUT_PULLUP);
  pinMode(PIN_VALVE_AIR, OUTPUT);
  pinMode(PIN_VALVE_C02, OUTPUT);
  pinMode(PIN_VALVE_WATER, OUTPUT);
  pinMode(PIN_VALVE_CLEANER_IN, OUTPUT);
  pinMode(PIN_VALVE_SANITIZER_IN, OUTPUT);
  pinMode(PIN_VALVE_CLEANER_OUT, OUTPUT);
  pinMode(PIN_VALVE_SANITIZER_OUT, OUTPUT);
  pinMode(PIN_VALVE_DRAIN, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  controls_set(0);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  buttonUp.attach(PIN_BUTTON_UP);
  buttonDown.attach(PIN_BUTTON_DOWN);
  buttonAction.attach(PIN_BUTTON_ACTION);

  buttonUp.interval(10);
  buttonDown.interval(10);
  buttonAction.interval(10);

  lcd.begin();
  lcd.setBacklight(HIGH);
  lcd.createChar(CHAR_UP_DOWN, CHAR_UP_DOWN_SETUP);
  
  mode = EEPROM.read(EEPROM_ADDRESS_MODE);
  mode = constrain(mode, 0, MODES_NUMBER - 1);
}

void loop()
{
  switch(state) {
    case STATE_SELECT:
      select();
      break;
    case STATE_SELECT_UPDATE:
      select_update();
      break;
    case STATE_RUN:
      run();
      break;
    case STATE_RUN_UPDATE:
      run_update();
      break;
    case STATE_TERMINATE:
      terminate();
      break;
    case STATE_CANCEL:
      cancel();
      break;
  }
}

