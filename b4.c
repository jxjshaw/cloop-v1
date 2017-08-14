#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

const int ledPin = 22; // Broadcom 23
const int butPin = 17; // Broadcom 17
bool pedalOn = false; // Light starts off
bool buttonHeldDown = false;
char startRecording[50];
char stopRecording [50];

int main(void)
{
    // Setup stuff:
    wiringPiSetupGpio(); //Broadcom pins
    pinMode(ledPin, OUTPUT);     // Set regular LED as output
    pinMode(butPin, INPUT);      // Set button as INPUT
    pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

    // Bash to record audio indefinitely until proccess is killed
    strcpy( startRecording, "arecord -D plughw:1 -f cd -vv test.wav &" );
    // Killall does not work while arecord proccess is happening...
    strcpy( stopRecording, "killall -KILL arecord && aplay -D plughw:1 test.wav" );
    // We can play audio or upload file once recording stopped && aplay -D plug:hw1 test.wav

    printf("Blinker is running! Press CTRL+C to quit.\n");

    // Loop
    while(1)
    {
        if (!digitalRead(butPin)) {
          if (buttonHeldDown) {
            continue;
          }

          if (pedalOn) {
            digitalWrite(ledPin, LOW);
            printf("LED OFF.\n");
            system(stopRecording);
            // todo: upload file
          } else {
            digitalWrite(ledPin, HIGH);
            printf("LED ON.\n");
            // todo: stopPlayback
            system(startRecording);

          }

          buttonHeldDown = true;
          pedalOn = !pedalOn;
        }
        else {
          buttonHeldDown = false;
        }

        // We need to add a delay because when the button
        // is pushed and we don't have a delay we may
        // see an inconsistent reading of the button state
        // (it flickers from pushed/not pushed when clearly
        // pushed)
        delay(75);
    }

    return 0;
}
