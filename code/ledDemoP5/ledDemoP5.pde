import controlP5.*;
import processing.serial.*;

ControlP5 control;
Serial port;
String[] ports;
public int LED_Width = 6;
public int LED_Height = 3;
public int LED_RowSpace = 4;
public int LED_ColSpace = 1;
public int LED_RowCount = 8;
public int LED_ColCount = 4;
public int LED_RotStep = 0;
public int LED_Rot = 45;
public int LED_Off = 20;
public int LED_On = 100;
public int LED_Dis = 50;
public int LED_Trig = 255;
public int LED_Hue = 0;
public int LED_Sat = 0;
//quin's wide screen: 110
//quin's laptop screen:
public int ppi = 110;
float panelPct = (279.0-43.0)/323.0;
float panelHeightmm = 128.5;
float panelWidthmm = 20;
public boolean PlaceLEDs = false;
PImage panelbg;

ArrayList<PVector> ledPlacement = new ArrayList<PVector>();
boolean firstActiveClicked = false;
boolean firstInactiveClicked = false;

int numNodes = 0;
int numActive = 0;
int currTrigger = -1;
int triggerMillis = 0;
int triggerDuration = 50;

void setup() {
  size(600, 800);
  panelbg = loadImage("components/4HP PANEL_01.png");
  control = new ControlP5(this);
  ports = Serial.list();
  control.addScrollableList("Serial_Port")
    .addItems(ports);
  String[] numbers = {
    "LED_Width", "LED_Height", 
    "LED_Hue", "LED_Sat", 
    "LED_RowSpace", "LED_ColSpace", 
    "LED_RowCount", "LED_ColCount",
    "LED_Rot", "LED_RotStep",
    "LED_On", "LED_Off",
    "LED_Dis", "LED_Trig",
    "ppi"
  };
  int y = 5;
  for(int i = 0; i < numbers.length; i++, y+=20){
    control.addNumberbox(numbers[i])
      .setPosition(width-100, y)
      .align(ControlP5.LEFT, ControlP5.CENTER, ControlP5.LEFT_OUTSIDE, ControlP5.CENTER);
  }
  control.addToggle("PlaceLEDs")
    .setPosition(width - 100, y)
    .align(ControlP5.LEFT, ControlP5.CENTER, ControlP5.LEFT_OUTSIDE, ControlP5.CENTER);
  y += 20;
  control.addButton("SavePlacement")
    .setPosition(width - 100, y);
  y += 20;
  control.addButton("LoadPlacement")
    .setPosition(width - 100, y);
  y += 20;
  
  rectMode(CENTER);
  colorMode(HSB);
  noStroke();
}

int getLedBrightness(int index){
  if (index < numActive){
    if (index == currTrigger){
      if (triggerMillis >= millis()){
        return LED_Trig;
      } else {
        currTrigger = -1;
      }
    }
    return LED_On;
  }
  if (index < numNodes){
    return LED_Dis;
  }
  return LED_Off;
}
      

void draw() {
  colorMode(RGB);
  background(0xf9, 0xf9, 0xf1);
  colorMode(HSB);
  imageMode(CENTER);
  pushMatrix();
  float scl = ((ppi/25.4)*(panelWidthmm/panelPct))/panelbg.width;
  translate(width/2, height/2);
  scale(scl);
  image(panelbg, 0, 0);
  popMatrix();
  //rect(width/2, height/2, ppi/24.5*panelWidthmm, ppi/24.5*panelHeightmm);
  float currRot = LED_Rot * PI / 180;
  if (PlaceLEDs){
    for(int i = 0; i < ledPlacement.size(); i++, currRot += (LED_RotStep * PI / 180)){
      pushMatrix();
      translate(ledPlacement.get(i).x, ledPlacement.get(i).y);
      rotate(currRot);
      fill(LED_Hue, LED_Sat, getLedBrightness(i));
      rect(0, 0, LED_Width, LED_Height);
      popMatrix();
    }
  } else {
    int gridwidth = (LED_ColSpace + LED_Width) * LED_ColCount - LED_ColSpace;
    int gridheight = (LED_RowSpace + LED_Height) * LED_RowCount - LED_RowSpace;
    pushMatrix();
    translate((width + gridwidth) / 2, (height + gridheight) / 2);
    int LEDindex = 0;
    for(int i = 0; i < LED_RowCount; i++){
      pushMatrix();
      translate(-LED_Width/2, 0);
      for(int j = 0; j < LED_ColCount; j++, currRot += (LED_RotStep * PI / 180), LEDindex++){
        rotate(currRot);
        fill(LED_Hue, LED_Sat, getLedBrightness(LEDindex));
        rect(0, 0, LED_Width, LED_Height);
        rotate(-currRot);
        translate(-LED_Width-LED_ColSpace, 0);
      }
      popMatrix();
      translate(0, -LED_Height-LED_RowSpace);
    }
    popMatrix();
  }
}

void mouseClicked(){
  println("click");
  if (PlaceLEDs){
    if (!firstActiveClicked){
      firstActiveClicked = true;
      firstInactiveClicked = false;
    } else {
      println("add at" + mouseX + "," + mouseY);
      ledPlacement.add(new PVector(mouseX, mouseY));
    }
  } else {
    if (!firstInactiveClicked){
      firstInactiveClicked = true;
      firstActiveClicked = false;
    }
  }
}

public void SavePlacement(){
  println("save");
  firstActiveClicked = false;
  selectOutput("Select a file to write to", "outputJson");
}

public void LoadPlacement(){
  println("load");
  firstActiveClicked = false;
  selectInput("Select file to load", "inputJson");
}

public void inputJson(File selection){
  if (selection != null){
    JSONObject json = loadJSONObject(selection.getAbsolutePath());
    LED_Width = json.getInt("LED_Width");
    LED_Height = json.getInt("LED_Height");
    JSONArray pl = json.getJSONArray("led");
    ledPlacement.clear();
    for(int i = 0; i < pl.size(); i++){
      JSONObject led = pl.getJSONObject(i);
      ledPlacement.add(new PVector(led.getFloat("x"), led.getFloat("y")));
    }
    PlaceLEDs = true;
  }
}

public void outputJson(File selection){
  if (selection != null){
    JSONObject json = new JSONObject();
    JSONArray pl = new JSONArray();
    for(int i = 0; i < ledPlacement.size(); i++){
      JSONObject led = new JSONObject();
      led.setFloat("x", ledPlacement.get(i).x);
      led.setFloat("y", ledPlacement.get(i).y);
      pl.setJSONObject(i, led);
    }
    json.setJSONArray("led", pl);
    json.setInt("LED_Width", LED_Width);
    json.setInt("LED_Height", LED_Height);
    saveJSONObject(json, selection.getAbsolutePath());
  }
}

void Serial_Port(int n) {
  port = new Serial(this, ports[n], 9600);
  port.bufferUntil('.');
}

void serialEvent(Serial p){
  String in = p.readString();
  int num = parseInt(in.substring(1, in.length() - 1));
  println(in.charAt(0));
  println(num);
  switch(in.charAt(0)){
    case 'n':
      numNodes = num;
      break;
    case 'a':
      numActive = num;
      break;
    case 't':
      currTrigger = num;
      triggerMillis = millis() + triggerDuration;
      break;
  }
}
