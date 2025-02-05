#define ledPin 11
#define unitTime 300

struct Morse {
    char character;
    String code;
};

Morse morseTable[] = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
    {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
    {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
    {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."},
    {'0', "-----"}
};

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    Serial.println("Enter text to encode it as Morse code:");
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
      	Serial.println("Text: " + input);
        input.trim();
        textToMorseCode(input);
        Serial.println("Enter text to encode it as Morse code:");
    }
}

void textToMorseCode(String text) {
  	Serial.println("Morse code: ");
    for (int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == ' ') {
            delay(7 * unitTime);
        } else {
      		Serial.print(c);
			Serial.print(": ");
            String code = getMorseCode(c);
            if (code) {
                for (int j = 0; j < code.length(); j++) {
                  	Serial.print(code[j]);
                    digitalWrite(ledPin, HIGH);
                  	code[j] == '.' ? delay(unitTime) : delay(3 * unitTime);
                    digitalWrite(ledPin, LOW);
                    delay(unitTime);
                }
              	Serial.println();
                delay(3 * unitTime);
            }
        }
    }
    Serial.println();
}

String getMorseCode(char c) {
  	char temp = toupper(c);
    for (int i = 0; i < (sizeof(morseTable) / sizeof(morseTable[0])); i++) {
        if (morseTable[i].character == temp) {
            return morseTable[i].code;
        }
    }
    return "";
}
