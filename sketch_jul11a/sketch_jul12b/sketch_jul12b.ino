// Pin number for the built-in LED
const int ledPin = 13;

// Morse code timings in milliseconds
const int dotDuration = 200;
const int dashDuration = 3 * dotDuration;
const int interSymbolGap = dotDuration;
const int interCharacterGap = 3 * dotDuration;

// Morse code lookup table (letters A-Z and numbers 0-9)
const char* morseCode[] = {
  ".-",   // A
  "-...", // B
  "-.-.", // C
  "-..",  // D
  ".",    // E
  "..-.", // F
  "--.",  // G
  "....", // H
  "..",   // I
  ".---", // J
  "-.-",  // K
  ".-..", // L
  "--",   // M
  "-.",   // N
  "---",  // O
  ".--.", // P
  "--.-", // Q
  ".-.",  // R
  "...",  // S
  "-",    // T
  "..-",  // U
  "...-", // V
  ".--",  // W
  "-..-", // X
  "-.--", // Y
  "--..", // Z
  "-----",  // 0
  ".----",  // 1
  "..---",  // 2
  "...--",  // 3
  "....-",  // 4
  ".....",  // 5
  "-....",  // 6
  "--...",  // 7
  "---..",  // 8
  "----."   // 9
};

// State machine variables
unsigned long previousTime = 0;
int currentSymbol = 0;
int currentCharacter = 0;
bool isSymbolOn = false;

void setup() {
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
}

void loop() {
  // Message to transmit in Morse code
  const char* message = "HELLO WORLD";

  // Current time in milliseconds
  unsigned long currentTime = millis();

  // Check if it's time to switch the LED state
  if (currentTime - previousTime >= (isSymbolOn ? (currentSymbol == '.' ? dotDuration : dashDuration) : interSymbolGap)) {
    // Toggle the LED state
    digitalWrite(ledPin, isSymbolOn ? LOW : HIGH);
    isSymbolOn = !isSymbolOn;

    // Move to the next symbol or character
    if (isSymbolOn) {
      currentSymbol++;
    } else {
      currentSymbol = 0;
      currentCharacter++;
    }

    // Check if the end of the message is reached
    if (currentCharacter >= strlen(message)) {
      // Wait for the inter-character gap and reset the state machine
      if (currentTime - previousTime >= interCharacterGap) {
        currentCharacter = 0;
        previousTime = currentTime;
      }
    } else {
      // Get the Morse code representation of the current character
      char c = toupper(message[currentCharacter]);
      if (c == ' ') {
        // If it's a space, wait for the inter-character gap
        previousTime = currentTime;
      } else {
        const char* code = morseCode[c - 'A'];
        currentSymbol = 0;
        previousTime = currentTime;
      }
    }
  }
}
