import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import toxi.geom.*; 
import toxi.geom.mesh.*; 
import toxi.processing.*; 
import peasy.*; 
import controlP5.*; 
import oscP5.*; 
import netP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Spherical_VU_Meter extends PApplet {

/* Spherical 3D Vu-Meter with Kemar Head
 * Copyright (c) 2015 Pierre Lecomte */


/* toxiclibs to handle .stl mesh files
 * http://toxiclibs.org/
 */ 



TriangleMesh mesh;
ToxiclibsSupport gfx;

/* PeasyCam to have a nice mouse-driven camera
 * http://mrfeinberg.com/peasycam/
 */

PeasyCam cam;

/* oscP5 to drive the speakers level and sources position
http://www.sojamo.de/libraries/oscP5/
*/



OscP5 oscP5;

PFont f;
Atom Mimi[], Source1, Source2;
float[] rayons, position1, position2, position1sph, position2sph, dB;
float[][] positions1, positions2; // past of the Source1 position to create a blur effect
int nb, nb2,hmin=20, hmax=120;
float deltar = 1, r1 = 1-deltar, r2 = 8-deltar, dBmin=-70, dBmax=6; // radius min and max of the spheres and dBmin and dBmax of the VU-Meter, deltar is to have a minimum radius to render a sphere even if no signal
Table table;

public void setup() {
  
  //RENDERING
  
  //surface.setResizable(true);
  colorMode(HSB, 360,100,100);
  
  //FONT
  f = createFont("Arial",40,true);
  
  //KEMAR HEAD MESH
  mesh=(TriangleMesh)new STLReader().loadBinary(sketchPath("data/kemar_head_5000.stl"),STLReader.TRIANGLEMESH);
  mesh.scale(0.095f);
  mesh.rotateZ(-6*PI/7);
  gfx=new ToxiclibsSupport(this);

  //CAMERA
  cam = new PeasyCam(this,360);
  cam.setMinimumDistance(50);
  cam.setRotations(-0.68f,-1.30f,0.88f);
  cam.setMaximumDistance(2000);
  
  //SPEAKERS SET
  table = loadTable("lebedev50.csv", "header"); // coordinates stored in a csv files WARNING: The Y-axis is flipped
  nb = table.getRowCount();
  Mimi= new Atom[nb]; 
  for (TableRow row : table.rows()) {
    int id = row.getInt("id");
    float x = row.getFloat("x");
    float y = row.getFloat("y");
    float z = row.getFloat("z");
    //float r = row.getFloat("r");
    Mimi[id] = new Atom(x,y,z);
   
    
    // Sources
    Source1 = new Atom(0,0,0);
    Source2 = new Atom(0,0,0);
         
    // RAYONS & dB INITIAL VALUES
    rayons = new float[nb];
    dB = new float[nb];
    for(int i=0; i<nb;i++){
     dB[i]=dBmin;
     rayons[i] = 1;//(i+1)/5; 
     
    // Source1 POSITION INITIAL
    position1 = new float[3];
    position1[0] = 107;
    position1[1] = 0;
    position1[2] = 0;
    
    position1sph = new float[3];
    position1sph[0] = 1.07f;
    position1sph[1] = 0;
    position1sph[2] = 0;
   
    
    // Source2 POSITION INITIAL
    position2 = new float[3];
    position2[0] = 107;
    position2[1] = 0;
    position2[2] = 0;
    
    position2sph = new float[3];
    position2sph[0] = 1.07f;
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
  
  //OSC
  // starts oscP5, telling it to listen for incoming messages at port 5001 */
  oscP5 = new OscP5(this,5511);
      
}

public void draw() {
  background(0);
  lights();
  cam.beginHUD(); // To have a 2D layer above the 3D
  drawPalette();
  drawCoordinates();
  cam.endHUD();
  drawHead();
  drawSphere();
  drawSources();
  drawAxis();
}

public void drawHead() {
  noStroke();
  fill(color(0,0, 80));
  gfx.mesh(mesh,true,0);
  translate(-1,0,5);
}

public void drawPalette() {
    textFont(f,15);
  for (int h=hmin; h< hmax; h++) { // dB = (dBmin-dBmax)/(hmax-hmin)*h + (dBmax*hmax-dBmin*hmin)/(hmax-hmin)
      if (h <= (dBmax*hmax - dBmin*hmin)/(dBmax - dBmin)){ // if dBmax >= dB >= 0
        fill(color(0,100,100));
        } 
      else { // else if 0 >= dB > dBmin
        fill(color(h,100,100));
      }
    noStroke();
    rect(10,10+6*(h-hmin),40,6);
    if (h%4==0){
    text(nfp(round((dBmin-dBmax)/(hmax-hmin)*h + (dBmax*hmax-dBmin*hmin)/(hmax-hmin)),0,0),55,15+6*(h-hmin));
    }
    }
}

public void drawCoordinates() {
  textFont(f,40);       
  fill(color(0, 100, 100));
  text("X : " + nfp(position1[0]/100,1,2) + " m",110,50);
  text("Y : " + nfp((-1)*position1[1]/100,1,2) + " m",110,90); // BE CAREFUL HERE Y IS REVERSED TO ACCORD THE AXIS SENS IN PROCESSING
  text("Z : " + nfp(position1[2]/100,1,2) + " m",110,130);
  
  text("R : " + nfp(position1sph[0],1,2) + " m",110,200);
  text('\u03B8'+" : " + nfp(round(position1sph[1]),3) + " "+ '\u00B0',110,240);
  text('\u03B4'+" : " + nfp(round(position1sph[2]),3) + " \u00B0",110,280);
  
  fill(color(60,100, 100));
  text("X : " + nfp(position2[0]/100,1,2) + " m",110,380);
  text("Y : " + nfp((-1)*position2[1]/100,1,2) + " m",110,420); // BE CAREFUL HERE Y IS REVERSED TO ACCORD THE AXIS SENS IN PROCESSING
  text("Z : " + nfp(position2[2]/100,1,2) + " m",110,460);
  
  text("R : " + nfp(position2sph[0],1,2) + " m",110,510);
  text('\u03B8'+" : " + nfp(round(position2sph[1]),3) + " "+ '\u00B0',110,550);
  text('\u03B4'+" : " + nfp(round(position2sph[2]),3) + " \u00B0",110,590);
}

public void drawSphere() {
  noStroke();
  for ( int i=0; i<nb; i++) {
    int col = color(0,100,100); // r = (r2-r1)/(dBmax - dBmin) dB + (dBmin*r2 - dBmax*r1)/(dBmax - dBmin)
    if (dB[i]>=0) { // if dB >=0
      col = color(0,100,100);
    }
    else {
      col = color((-dB[i]*hmax + dBmax*hmax + dB[i]*hmin - dBmin*hmin)/(dBmax - dBmin),100,100); // linear interpolation color
    }
      fill(col);
      Mimi[i].changeRadius(rayons[i]);
      Mimi[i].render();
      }
  }
  
public void drawSources() {
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
  
  public void drawAxis() {
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
  
public void oscEvent(OscMessage theOscMessage) {
  String v=theOscMessage.addrPattern();
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
    // get the osc values transmitted by Faust under name /output1 /output2 etc..
    for ( int i=0; i<nb; i++) {
        if (v.equals("/output"+str(i+1))) {
      rayons[i]= (r2-r1)/(dBmax-dBmin)*theOscMessage.get(0).floatValue()+(dBmax*r1-dBmin*r2)/(dBmax-dBmin)+deltar; //linear interpolation : dBmin ==> r1 10 dBmax ==> r2
      dB[i]=theOscMessage.get(0).floatValue();
    }
    }
}

class Atom { 
//attributes
float aX, aY, aZ, aRadius;

//constructor
//Atom(float aX, float aY, float aZ,float aRadius) {
Atom(float aX, float aY, float aZ) {
this.aX = aX;
this.aY = aY;
this.aZ = aZ;
this.aRadius = 20;
}

public float getRadius() {
    return this.aRadius; 
}

public void changeRadius(float r) {
  this.aRadius=r;
}

public void changePosition(float newaX, float newaY, float newaZ) {
  this.aX=newaX;
  this.aY=newaY;
  this.aZ=newaZ;
}

public void render() {
  pushMatrix(); 
  translate(aX, aY, aZ);
  sphere(aRadius);
  popMatrix();
}
} 
  public void settings() {  fullScreen(P3D,2); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Spherical_VU_Meter" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
