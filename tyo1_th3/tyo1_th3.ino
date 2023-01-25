#include "painlessMesh.h"
#include "AsyncTCP.h"


#define   MESH_PREFIX     "meshimeshi"
#define   MESH_PASSWORD   "sanasala"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hi from Feather! :)";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

// GPIO where LED is connected to
const int ledPin = 25;  //LED output
const int fanPin = 21;  //Fan control
const int potPin = 26;  //A0, pot
const int freq = 5000;
int potValue = 0;

// setting PWM properties
const int fanChannel = 0;
const int resolution = 8;

// Handle received and sent messages
String message = "";
char incomingChar;
int speed = 0;

// Timer: auxiliar variables
unsigned long previousMillis = 0;  // Stores last time temperature was published
const long interval = 3000;        // interval at which to publish sensor readings



void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  Serial.begin(115200);

  // configure fan PWM functionalitites
  ledcSetup(fanChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(fanPin, fanChannel);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  unsigned long currentMillis = millis();

  // Send pot readings
  
  // it will run the user scheduler as well
  mesh.update();
    potValue = analogRead(potPin);
  

  
  /*
  } else if (message == "fan_on") {
    digitalWrite(fanPin, HIGH);
  } else if (message == "fan_off") {
    digitalWrite(fanPin, LOW);
  }
  


//Setting fan speed between 0-255
  else if (isDigit(message[0])) {
    speed = message.toInt();
    if (speed >= 1 && speed <= 255)
      ;
    ledcWrite(fanChannel, speed);
  }
*/

  delay(20);
} 