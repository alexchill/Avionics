// Include library header files

// Define misc constants
const int LOOP_DELAY = 50;

// Configure constants for I/O pin numbers
const int NAV_LED_PIN = 8;

// Define command names
const char CMD_NOP[]    = "NOP";
const char CMD_ROLL[]   = "ROLL";
const char CMD_PITCH[]  = "PITCH";
const char CMD_NAVLED[] = "NAVLED";
// Simulation-only commands
const char CMD_DELAY[] = "DELAY";
const char CMD_HALT[] = "HALT";
const char CMD_REPEAT[] = "REPEAT";

// Simulation commands
int cmdNdx = 0;
String COMMANDS[] = {
  "ROLL:30",
  "DELAY:1000",
  "NAVLED:ON",
  "ROLL:150",
  "DELAY:1000",
  "NAVLED:OFF",
  "ROLL:15",
  "REPEAT"
};

void setup() {
  // Configure Serial
  Serial.begin(9600);

  // Configure MotorLeft
  // Configure MotorRight
  // Configure RollServo
  // Configure RuddervatorServo
  // Configure NavLights
}

void loop() {
  String cmd = getCommand();
  executeCommand(cmd);
  delay(LOOP_DELAY);
}

// This a "simulation" implementation of getCommand
String getCommand() {
  String cmd = COMMANDS[cmdNdx];
  if (cmd == CMD_HALT) {
    return CMD_NOP;
  } else {
    cmdNdx++;
  }
  return cmd;
}

void executeCommand(String command) {
  Serial.print("Executing cmd: ");
  Serial.println(command);

  String cmdName;
  String cmdValueStr;

  // Look for the separator ":" which separates the command name from the command value.
  int ndxSeparator = command.indexOf(":");
  if (ndxSeparator != -1) {
    // If the command contains both a command and a value, separate out the command name and value.
    cmdName = command.substring(0, ndxSeparator);
    if (command.length() > ndxSeparator + 1) {
      cmdValueStr = command.substring(ndxSeparator + 1);
    }
  }
  else {
    // If there is no separator then then command does not specify a value
    cmdName = command;
  }

  // Execute the command
  if (cmdName == CMD_NOP) {
    // Nothing to do
    return;
  }
  else if (cmdName == CMD_ROLL) {
    int rollValue = cmdValueStr.toInt();
    // command RollServo to rollValue
  }
  else if (cmdName == CMD_NAVLED) {
    if (cmdValueStr == "ON") {
      digitalWrite(NAV_LED_PIN, HIGH);
    }
    else {
      digitalWrite(NAV_LED_PIN, LOW);
    }
  }
  else if (cmdName == CMD_DELAY) {
    int millisecs = cmdValueStr.toInt();
    delay(millisecs);
  }
  else if (cmdName == CMD_REPEAT) {
    cmdNdx = 0;
  }
  else {
    Serial.print("ERROR command not recognized: ");
    Serial.println(cmdName);
  }
}
