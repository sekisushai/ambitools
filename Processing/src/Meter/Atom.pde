
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

float getRadius() {
    return this.aRadius; 
}

void changeRadius(float r) {
  this.aRadius=r;
}

void changePosition(float newaX, float newaY, float newaZ) {
  this.aX=newaX;
  this.aY=newaY;
  this.aZ=newaZ;
}

void render() {
  pushMatrix(); 
  translate(aX, aY, aZ);
  sphere(aRadius);
  popMatrix();
}
} 
