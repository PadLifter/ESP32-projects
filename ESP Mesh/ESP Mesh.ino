#include "painlessMesh.h"
#include "AsyncTCP.h"


#define   MESH_PREFIX     "meshimeshi"
#define   MESH_PASSWORD   "sanasala"
#define   MESH_PORT       5555

// GPIO where LED is connected to

const int fanPin = 17;  //A3, Fan control
const int potPin = 34;  //A2, pot
uint adc = 0;
uint msg_val = 0;


Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
mesh.sendBroadcast((String)adc);
taskSendMessage.setInterval(TASK_SECOND * 1);

}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
 msg_val = msg.toInt();
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



void setup() {
  pinMode(fanPin, OUTPUT);
  Serial.begin(115200);


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
  
  adc = analogRead(potPin);
  // Send pot readings
if (msg_val > 1000 || adc > 1000)
digitalWrite(fanPin, HIGH);
else
digitalWrite(fanPin, LOW);
  // it will run the user scheduler as well
  mesh.update();
  
  

  
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
} 