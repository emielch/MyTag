class TagSettings {
  int ID;
  boolean isTimer;
  color col;
  int time;
}


class TagBlock { 
  PVector pos, initPos, offs, closePos;
  color col;
  float openFac = 0;
  boolean opened = false;
  boolean hovering = false;
  float openScale;
  
  TagSettings settings;

  TagBlock (float x, float y) {  
    pos = new PVector(x, y);
    initPos = new PVector(x, y);
    closePos = new PVector(x, y);
    
    settings = new TagSettings();
    settings.col = color(0,100,200);
    settings.isTimer = true;
    settings.time = 160;
  } 
  
  void setID(int i){
    settings.ID = i;
  }

  void draw() {
    if(opened && openFac<1) openFac += 0.05;
    if(!opened && openFac>0) openFac -= 0.05;
    
    pos.x = width/2;
    pos.y = height/2;
    openScale = 5*esf;
    
    pos.lerp(closePos,1-openFac);
    
    pushMatrix();
    
    translate(pos.x, pos.y);
    scale(sf*esf + openFac*openScale*sf); 
    rotate(0.5*PI);
    col = settings.col;    
    colorMode(HSB,360,100,100);
    if(hovering) col = color(hue(col), saturation(col)-50*(1-openFac), brightness(col));
    fill(col);
    stroke(0);
    strokeWeight(1/sf);
    beginShape();
    vertex(60, 0);
    vertex(30, 30*sqrt(3));
    vertex(-30, 30*sqrt(3));
    vertex(-60, 0);
    vertex(-30, -30*sqrt(3));
    vertex(30, -30*sqrt(3));
    endShape(CLOSE);
    popMatrix();
    
  }

  boolean inShape(float x, float y) {
    PVector p = new PVector(x, y);
    if (pos.dist(p)<60*(sf*esf+openFac*openScale*sf)) {
      return true;
    }
    return false;
  }
  
  boolean clicked(){
    opened = !opened;
    if(opened) col = color(200);
    else col = color(128);
    return opened;
  }

  void setOffset(PVector p) {
    closePos = PVector.add(PVector.mult(initPos,esf),p);   
    closePos.mult(sf);
  }

  void setColor(color c) {
    col = c;
  }
  
  void hoverOn(){
    hovering = true;
  }
  
  void hoverOff(){
    hovering = false;
  }
}
