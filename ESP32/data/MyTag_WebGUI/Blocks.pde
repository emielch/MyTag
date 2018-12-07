

class Blocks {

  PVector pos;

  float spacingy = 30*sqrt(3)*2;
  float spacingx = 60;
  
  TagButtons buttons;

  TagBlock openBlock;

  TagBlock[] blocks = {
    new TagBlock(spacingx*3, spacingy), //3,4
    new TagBlock(spacingx*2, 0),   //2,3
    new TagBlock(spacingx, -spacingy), // 1,2
    new TagBlock(-spacingx, -spacingy), //1,1
    new TagBlock(0, 0),           //2,2
    new TagBlock(-spacingx*2, 0), //2,1
    new TagBlock(-spacingx*3, spacingy), //3,1
    new TagBlock(-spacingx, spacingy),  //3,2
    new TagBlock(-spacingx*2, spacingy*2), //4,1
    new TagBlock(spacingx, spacingy),    //3,3
    new TagBlock(spacingx*2, spacingy*2),   //4,3
    new TagBlock(0, spacingy*2) //4,2
    
  };


  Blocks() {
    pos = new PVector(width/2/sf, height/2/sf);
    buttons = new TagButtons();
    for (int i=0; i<blocks.length; i++) blocks[i].setID(i+1);
  }

  void setPos(float x, float y) {

    pos.x = x/sf;
    pos.y = y/sf - spacingy*esf;

    for (int i=0; i<blocks.length; i++) {
      blocks[i].setOffset(pos);
    }
    
    buttons.rePos();
  }

  void draw() {
    TagBlock onTopBlock = null;
    for (int i=0; i<blocks.length; i++) {
      if (blocks[i].openFac>0 && onTopBlock==null){
        onTopBlock = blocks[i];
        continue;
      }
      blocks[i].draw();
    }

    if (onTopBlock!=null){
      onTopBlock.draw();
      if(onTopBlock.openFac>0.95){
        buttons.draw();
      }
    }
  }


  void mouseMoved(int x, int y) {
    for (int i=0; i<blocks.length; i++) {
      if (blocks[i].inShape(x, y)) {
        blocks[i].hoverOn();
      } else {
        blocks[i].hoverOff();
      }
    }
  }


  void mouseClicked(int x, int y) {
    if (openBlock!=null) {
      buttons.mouseClicked(x,y);
      
      if (!openBlock.inShape(x, y)) {
        openBlock.clicked();
        openBlock = null;
        buttons.setSettings(null);
      }
      return;
    }
    
    for (int i=0; i<blocks.length; i++) {
      if (blocks[i].inShape(x, y)) {
        if (blocks[i].clicked()){
          openBlock = blocks[i];
          buttons.setSettings(blocks[i].settings);
        }else openBlock = null;
        return;
      }
    }
  }
}
