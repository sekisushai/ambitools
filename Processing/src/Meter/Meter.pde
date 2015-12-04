
import peasy.*;
import controlP5.*;
import oscP5.*;
import netP5.*;


OscP5 oscP5;
//NetAddress myRemoteLocation;

PeasyCam cam;
PFont f;
Atom Mimi[], Source1, Source2;

float[] rayons, position1, position2, position1sph, position2sph;
float[][] positions1, positions2; // past of the Source1 position to create a blur effect
int nb, nb2;
float deltar = 1, r1 = 1-deltar, r2 = 10-deltar; // radius min and max of the spheres
Table table;
color[] palette = new color[120]; // 120 colors palette

void setup() { 
    // GENERAL
      size(displayWidth, displayHeight,OPENGL);
    //size(1024,1024, P3D);
    //fill(204,360);
    colorMode(HSB, 360);
    f = createFont("Arial",40,true); // STEP 3 Create Font
    
    //OSC
    // start oscP5, telling it to listen for incoming messages at port 5001 */
    oscP5 = new OscP5(this,5001);
    
    // set the remote location to be the localhost on port 5001
    //myRemoteLocation = new NetAddress("localhost",5001);
   
    //CAMERA
    cam = new PeasyCam(this,360);
    cam.setMinimumDistance(50);
    cam.setRotations(-0.68,-1.30,0.88);
    
    //cam.rotateZ(radians(90));
    //cam.rotateY(radians(-80));
    //cam.rotateX(radians(10));
    cam.setMaximumDistance(2000);
    
    // COLOR BAR
    for (int i=0; i<=119; i++) {
      palette[i]= color(i,360,360);
    }

    // DATA    // ATTENTION, LE REPERE EST INDIRECT PAR RAPPORT A MATHEMATICA Y EST TRANSFORME EN -Y
    table = loadTable("lebedev50.csv", "header"); // coordinates stored in a csv files
    nb = table.getRowCount();
    Mimi= new Atom[nb]; 
    for (TableRow row : table.rows()) {
      int id = row.getInt("id");
      float x = row.getFloat("x");
      float y = row.getFloat("y");
      float z = row.getFloat("z");
      //float r = row.getFloat("r");
      Mimi[id] = new Atom(x,y,z);
      frameRate(150);
      
      
      // Sources
      Source1 = new Atom(0,0,0);
      Source2 = new Atom(0,0,0);
           
      // RAYONS
      rayons = new float[nb];
      for(int i=0; i<nb;i++){
       rayons[i] = 1;//(i+1)/5; 
       
      // Source1 POSITION INITIAL
      position1 = new float[3];
      position1[0] = 107;
      position1[1] = 0;
      position1[2] = 0;
      
      position1sph = new float[3];
      position1sph[0] = 1.07;
      position1sph[1] = 0;
      position1sph[2] = 0;
     
      
      // Source2 POSITION INITIAL
      position2 = new float[3];
      position2[0] = 107;
      position2[1] = 0;
      position2[2] = 0;
      
      position2sph = new float[3];
      position2sph[0] = 1.07;
      position2sph[1] = 0;
      position2sph[2] = 0;
      
      
      // Sources POSITIONS
      nb2 = 30;
      positions1 = new float[3][nb2];
      positions2 = new float[3][nb2];
      
      for (int j=0;j<nb2;j++) 
      {
      positions1[0][j] = 107;
      positions1[1][j] = 0;
      positions1[2][j] = 0;
      
      positions2[0][j] = 107;
      positions2[1][j] = 0;
      positions2[2][j] = 0;
      }
      
      }
    }
} // end setup 

boolean sketchFullScreen() {  
  return true;
}

void draw() {

lights();
background(0);

noStroke();
cam.beginHUD();
drawPalette();
textFont(f,40);       
  fill(color(0, 360, 360));
text("X : " + nfp(position1[0]/100,1,2) + " m",100,50);
text("Y : " + nfp((-1)*position1[1]/100,1,2) + " m",100,90); // BE CAREFUL HERE Y IS REVERSED TO ACCORD THE AXIS SENS IN PROCESSING
text("Z : " + nfp(position1[2]/100,1,2) + " m",100,130);

text("R : " + nfp(position1sph[0],1,2) + " m",100,200);
text('\u03B8'+" : " + nfp(round(position1sph[1]),3) + " "+ '\u00B0',100,240);
text('\u03B4'+" : " + nfp(round(position1sph[2]),3) + " \u00B0",100,280);

  fill(color(60,360, 360));
text("X : " + nfp(position2[0]/100,1,2) + " m",100,380);
text("Y : " + nfp((-1)*position2[1]/100,1,2) + " m",100,420); // BE CAREFUL HERE Y IS REVERSED TO ACCORD THE AXIS SENS IN PROCESSING
text("Z : " + nfp(position2[2]/100,1,2) + " m",100,460);

text("R : " + nfp(position2sph[0],1,2) + " m",100,510);
text('\u03B8'+" : " + nfp(round(position2sph[1]),3) + " "+ '\u00B0',100,550);
text('\u03B4'+" : " + nfp(round(position2sph[2]),3) + " \u00B0",100,590);

cam.endHUD(); 
drawAxis();
drawSphere();
drawSources();
//println(cam.getRotations());

} // end draw

void drawAxis() {
    stroke( 0, 0, 360, 250); // middle grey 
  strokeWeight(1);
  line(0,0,0,150,0,0);
  line(0,0,0,0,-150,0);
  line(0,0,0,0,0,150);
  textSize(10);
  fill(color(0, 0, 360));
  text("x", 150, 0,0);
  text("y", 0, -150,0);
  text("z", 0, 0,150);
}

public void drawSphere() {
  noStroke();
  //r = rayonOSC(theR);
  for ( int i=0; i<nb; i++) {
      //float r=  random(r1, r2);
      float col = 120/(r1-r2)*(rayons[i]-r2);
      fill(color(col, 360, 360)); // hue = 120 = green
      Mimi[i].changeRadius(rayons[i]);
      Mimi[i].render();
      }
  }
  
  void drawSources() {
    
          for ( int j=nb2; j>1; j--) {
          positions1[0][j-1] = positions1[0][j-2];
          positions1[1][j-1] = positions1[1][j-2];
          positions1[2][j-1] = positions1[2][j-2];
          Source1.changePosition(positions1[0][j-1],positions1[1][j-1],positions1[2][j-1]);
          fill(color(0,360,360,360-360/nb2*j));
          Source1.render();
          
          positions2[0][j-1] = positions2[0][j-2];
          positions2[1][j-1] = positions2[1][j-2];
          positions2[2][j-1] = positions2[2][j-2];
          Source2.changePosition(positions2[0][j-1],positions2[1][j-1],positions2[2][j-1]);
          fill(color(60,360,360,360-360/nb2*(j)));
          Source2.render();       
          }
    positions1[0][0] = position1[0];
    positions1[1][0] = position1[1];
    positions1[2][0] = position1[2];
    noStroke();
    Source1.changeRadius(2);
    Source1.changePosition(position1[0],position1[1],position1[2]);
    fill(color(0,360,360));
    Source1.render();
    
    positions2[0][0] = position2[0];
    positions2[1][0] = position2[1];
    positions2[2][0] = position2[2];
    noStroke();
    Source2.changeRadius(2);
    Source2.changePosition(position2[0],position2[1],position2[2]);
    fill(color(60,360,360));
    Source2.render();
    
  }

void drawPalette() {
  for (int i=0; i< 120; i++) {
    fill(palette[i]);
    rect(10,10+5*i,40,5);
    //println(i);
    //println(palette[i]);
    }
}

void oscEvent(OscMessage theOscMessage) {
  String v=theOscMessage.addrPattern();
    if (v.equals("/rayons")) {
       for ( int i=0; i<nb; i++) {
         rayons[i] = theOscMessage.get(i).floatValue()/10+deltar; //+deltar To HAVE A SPHERE DISPLAYED ALTHOUGH THERE IS NO SIGNAL BUT BE CAREFUL OF THE COLOR SCALE THEN
     }
    }
        if (v.equals("/position1")) {
       for ( int i=0; i<3; i++) {
         position1[i] = theOscMessage.get(i).floatValue()*100;
     }
    }
    if (v.equals("/position2")) {
       for ( int i=0; i<3; i++) {
         position2[i] = theOscMessage.get(i).floatValue()*100;
     }
    }
    if (v.equals("/position1sph")) {
       for ( int i=0; i<3; i++) {
         position1sph[i] = theOscMessage.get(i).floatValue();
     }
    }
    if (v.equals("/position2sph")) {
       for ( int i=0; i<3; i++) {
         position2sph[i] = theOscMessage.get(i).floatValue();
     }
    }    
}


