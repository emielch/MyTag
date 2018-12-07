// color, timer, moment 

class Slider {
  float val; // 0-1
  PVector pos;
  PVector size;
  boolean hueMode;

  Slider(boolean _hueMode) {
    pos = new PVector(0, 0);
    size = new PVector(10, 10);
    hueMode = _hueMode;
  }

  void init(int x, int y, int w, int h) {
    pos = new PVector(x, y);
    size = new PVector(w, h);
  }

  void draw(color col) {
    noStroke();
    colorMode(RGB, 255);
    fill(0);
    rect(pos.x-2, pos.y-2, size.x+4, size.y+5);

    colorMode(HSB, size.x, size.x, size.x);

    for (int i=(int)pos.x; i<int(pos.x+size.x); i++) {
      float j = i-pos.x;
      if (hueMode) stroke(j, size.x, size.x);
      else stroke(hue(col), size.x*2-j*2, j*2);   //min(size.x, j*2)
      line(i, pos.y, i, pos.y+size.y);
    }

    float xPos = pos.x+size.x*val;
    noStroke();
    fill(0);
    rect(xPos-5, pos.y-7, 10, size.y+14);
  }

  boolean mouseClicked(int x, int y) {
    if (x>pos.x && x<pos.x+size.x && y>pos.y && y<pos.y+size.y) {
      val = map(x, pos.x, pos.x+size.x, 0, 1);
      return true;
    }
    return false;
  }
}




class TagButtons {
  PVector pos;
  TagSettings s;

  Slider hueSlider;
  Slider lightSlider;
  TimeSelector timeSelector;
  ModeSwitch timeSwitch;

  TagButtons() {
    pos = new PVector(width/2, height/2);
    hueSlider = new Slider(true);
    lightSlider = new Slider(false);
    timeSelector = new TimeSelector();
    timeSwitch = new ModeSwitch("Timer", "Moment");
  }

  void rePos() {
    int sliderW = int(450*sf*esf);
    int sliderH = int(50*sf*esf);
    int switchW = int(280*sf*esf);
    hueSlider.init(width/2-sliderW/2, height/2+sliderH*2, sliderW, sliderH);
    lightSlider.init(width/2-sliderW/2, int(height/2+sliderH*3+20*sf*esf), sliderW, sliderH);
    timeSelector.init(width/2-sliderW/2, height/2-sliderH*3, sliderW, sliderH*4);
    timeSwitch.init(width/2-switchW/2, int(height/2-sliderH*4.5), switchW, sliderH);
  }


  void draw() {
    if (s == null) return;

    hueSlider.draw(s.col);
    lightSlider.draw(s.col);
    timeSelector.draw();
    timeSwitch.draw();
  }

  void mouseClicked(int x, int y) {
    if (s == null) return;
    boolean changed = false;

    if (hueSlider.mouseClicked(x, y)) {
      colorMode(HSB, 360, 100, 100);
      s.col = color(hueSlider.val*360, saturation(s.col), brightness(s.col));
      changed = true;
    }

    if (lightSlider.mouseClicked(x, y)) {
      colorMode(HSB, 360, 100, 100);
      s.col = color(hue(s.col), 200-lightSlider.val*200, lightSlider.val*200);
      changed = true;
    }

    if (timeSelector.mouseClicked(x, y)) {
      s.time = timeSelector.time;
      changed = true;
    }

    if (timeSwitch.mouseClicked(x, y)) {
      s.isTimer = timeSwitch.val;
      changed = true;
    }


    if (changed && js!=null) {
      colorMode(RGB, 255);
      int isTimer = s.isTimer ? 1 : 0;
      js.setBlock(s.ID, (int)red(s.col), (int)green(s.col), (int)blue(s.col), isTimer, s.time, 1);
    }
  }


  void setSettings(TagSettings _s) {
    s = _s;

    if (s !=null) {
      colorMode(HSB, 360, 100, 100);
      hueSlider.val = hue(s.col)/360.;
      float sat = saturation(s.col);
      if (sat==100) lightSlider.val = brightness(s.col)/200.;
      else lightSlider.val = 1-sat/200.;

      timeSelector.setTime(s.time);
      timeSwitch.val =  s.isTimer;
    }
  }
}
