float sf = 1;
float esf = 1;

interface JavaScript {
  void print(String data);
  void setBlock(int ID, int r, int g, int b, int isTimer, int time, int audio);
}
JavaScript js;

boolean useMultiTouch = false;

Blocks blocks = new Blocks();

void setup(){
  size(720,1280);
  blocks.setPos(width/2,height/2);
}



void draw(){
  colorMode(RGB,255);
  background(230);
  blocks.draw();  
}

void scaleWindow(int x, int y, float scaleFac) {   // change the display settings
  sf = scaleFac;
  size(int(x*sf), int(y*sf));
  if(x<720) esf = x/720;
  else esf = 1;
  blocks.setPos(width/2,height/2);
  
  js.print("esf: " + esf);
  js.print("x: " + width + ", y: " + height + ", scale: " + sf);
}

void bindJavascript(JavaScript _js) {
  js = _js;
}


void mouseMoved() {
  if(!useMultiTouch) blocks.mouseMoved(int(mouseX*sf),int(mouseY*sf));
}

void mousePressed() {
  if(!useMultiTouch) blocks.mouseClicked(int(mouseX*sf),int(mouseY*sf));
}


void setBlock(int ID, int r, int g, int b, boolean isTimer, int time){
  colorMode(RGB,255);
  ID -= 1;
  blocks.blocks[ID].settings.col = color(r,g,b);
  blocks.blocks[ID].settings.isTimer = isTimer;
  blocks.blocks[ID].settings.time = time;
}


void setMultitouch() {  // to be called by browser JS
  useMultiTouch = true;
}


void startTouch(int id, int x, int y) {
  int xpos = int(x * sf);
  int ypos = int(y * sf);
  
  blocks.mouseClicked(xpos,ypos);
}

void moveTouch(int id, int x, int y) {
  int xpos = int(x * sf);
  int ypos = int(y * sf);

}

void endTouch(int id, int x, int y) {
  int xpos = int(x * sf);
  int ypos = int(y * sf);
}
