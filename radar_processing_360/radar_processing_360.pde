import processing.serial.*;

Serial myPort;
String data = "";
String[] values;

int angle = 0, distance = 0;

void setup() {
  size(800, 800);
  background(0);

  // ⚠️ Change "COM3" to your Arduino port (check in Arduino IDE > Tools > Port)
  myPort = new Serial(this, "COM4", 9600);
  myPort.bufferUntil('.');
}

void draw() {
  background(0);

  // Move origin to center
  translate(width/2, height/2);

  // Radar circles
  stroke(0, 255, 0);
  noFill();
  ellipse(0, 0, 600, 600);
  ellipse(0, 0, 400, 400);
  ellipse(0, 0, 200, 200);

  // Sweep line
  stroke(0, 255, 0);
  line(0, 0, 300 * cos(radians(angle)), -300 * sin(radians(angle)));

  // Draw detected object (if within range)
  if (distance > 0 && distance < 200) {  // max 200 cm
    float x = distance * cos(radians(angle));
    float y = -distance * sin(radians(angle));

    fill(255, 0, 0);
    noStroke();
    ellipse(x, y, 10, 10);
  }
}

// Read incoming Serial data
void serialEvent(Serial p) {
  data = p.readStringUntil('.');
  if (data != null) {
    data = trim(data);
    values = split(data, ',');
    if (values.length == 2) {
      angle = int(values[0]);
      distance = int(values[1]);
    }
  }
}
