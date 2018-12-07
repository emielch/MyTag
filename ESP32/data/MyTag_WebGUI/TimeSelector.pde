int triSize = 50;

class NumberSelector {
  int val;
  float numSpace = 70;
  int maxVal;
  String title;

  PVector pos;
  PVector size;

  NumberSelector(int mv, String t) {
    val = 0;
    maxVal = mv;
    title = t;
  }

  void init(int x, int y, int w, int h) {
    pos = new PVector(x, y);
    size = new PVector(w, h);
    numSpace = h-2*triSize;
  }

  boolean mouseClicked(int x, int y) {
    if (x>pos.x-triSize && x<pos.x+triSize && y>pos.y && y< pos.y+triSize) {
      val++;
      if(val>maxVal) val = 0;
      return true;
    }
    if (x>pos.x-triSize && x<pos.x+triSize && y>pos.y+size.y-triSize && y< pos.y+size.y) {
      val--;
      if(val<0) val = maxVal;
      return true;
    }
    return false;
  }

  void draw() {
    colorMode(RGB, 255);
    fill(255);
    stroke(0);
    strokeWeight(1*sf*esf);
    triangle(pos.x, pos.y, pos.x-triSize, pos.y+triSize, pos.x+triSize, pos.y+triSize);
    triangle(pos.x, pos.y+numSpace+triSize*2, pos.x-triSize, pos.y+numSpace+triSize, pos.x+triSize, pos.y+numSpace+triSize);
    fill(180);
    rect(pos.x-triSize, pos.y+triSize+10, triSize*2, numSpace-20);

    fill(0);
    textSize(64*sf*esf);
    textAlign(CENTER, CENTER);
    text(val, pos.x, pos.y+size.y/2-4*sf*esf);
    textAlign(RIGHT, CENTER);
    text(title, pos.x-triSize, pos.y+size.y/2-4*sf*esf);
  }
}







class TimeSelector {
  NumberSelector hour;
  NumberSelector minute;
  
  int time;

  PVector pos;
  PVector size;

  TimeSelector() {
    hour = new NumberSelector(23, "H:");
    minute = new NumberSelector(59, "M:");
  }

  void init(int x, int y, int w, int h) {
    triSize = int(50*sf*esf);
    pos = new PVector(x, y);
    size = new PVector(w, h);
    hour.init(int(x+triSize*2.5), y, w, h);
    minute.init(int(x+w-triSize*2.5), y, w, h);
  }  

  boolean mouseClicked(int x, int y) {
    boolean h = hour.mouseClicked(x, y);
    boolean m = minute.mouseClicked(x, y);
    
    if(h || m){
      time = hour.val*60*60 + minute.val*60;
      return true;
    }
    return false;
  }
  
  void setTime(int t){
    time = t;
    hour.val = int(time/60/60);
    minute.val = int(time/60) - hour.val*60;
  }

  void draw() {
    noFill();
    stroke(0);
    //rect(pos.x, pos.y, size.x, size.y);
    hour.draw();
    minute.draw();
  }
}


class ModeSwitch{
  boolean val;
  String option1;
  String option2;
  
  PVector pos;
  PVector size;
  
  ModeSwitch(String o1, String o2){
    val = false;
    option1 = o1;
    option2 = o2;
  }
  
  void init(int x, int y, int w, int h) {
    pos = new PVector(x, y);
    size = new PVector(w, h);
  }
  
  
  void draw(){
    colorMode(RGB,255);
    stroke(0);
    fill(80);
    rect(pos.x,pos.y,size.x,size.y);
    fill(250);
    float posx = pos.x;
    if(!val) posx += size.x/2;
    rect(posx,pos.y,size.x/2,size.y);
    
    fill(0);
    textSize(32*sf*esf);
    textAlign(LEFT, CENTER);
    text(option1, pos.x+10, pos.y+size.y/2);
    textAlign(RIGHT, CENTER);
    text(option2, pos.x+size.x-10, pos.y+size.y/2);
  }
  
  boolean mouseClicked(int x, int y) {
    if (x>pos.x && x<pos.x+size.x && y>pos.y && y< pos.y+size.y) {
      val = !val;
      return true;
    }
    return false;
  }
  
  
}
