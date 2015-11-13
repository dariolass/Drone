import processing.serial.*;

Serial serialPort;
int pitch;
int roll;
int throttle;

String inBuffer;

void setup() {
  size(800, 400);
  
  pitch = 0;
  roll = 0;
  throttle = 0;
  
  println("Available serial ports:");
  
  inBuffer = "";
  textSize(14);
  inBuffer = "NO MESSAGE";

}

void draw() {
  background(200);
  text(inBuffer, 30, 30);
  
  //This code is drawing our control grid
  for (int i = 400; i < 800; i += 50) {
    if (i == 600) {
        stroke(100);
      } else {
        stroke(230);
      }
   line(i, 0, i, 400);
   int ix = i - 400;
   line(400, ix, 800, ix);
  }
  while (serialPort.available() > 0) {
   inBuffer = serialPort.readString(); 
  }
}

void keyPressed() {
  if (key == 'a') {
    throttle -= 10;
  } else if (key == 'w' && throttle <= 2000) {
    throttle += 10;
  } else if (key == '0') {
    throttle = 0;
  } else {
    return;
  }
  String serialString = "0,0,0," + throttle + "\n";
  serialPort.write(serialString);
  print(serialString);
  inBuffer = "key pressed";
}