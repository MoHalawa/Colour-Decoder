//The main meat and bones of this code comes mostly from converting the raw RGB values of the sensor array to RYB values that the LED can output

int red_lev  = 0, blue_lev = 0, green_lev = 0; //raw sensor values
int RED, GREEN, BLUE; //processed values are capatalized
float YELLOW, red, yellow, blue; //lower case variables are the processed values after standardization

int base_red = 0, base_green = 0, base_blue = 0; //We will take base values so we can account for environmental effects

int redpin = 11; //setting up pins for the RYB LED (Output)
int bluepin = 10;
int greenpin = 9;

int redPho = A1; //setting up pins for the sensor array (input)
int bluePho = A0;
int greenPho = A2;


void setup() {
  Serial.begin(9600);
  pinMode(redPho, INPUT);
  pinMode(bluePho, INPUT);
  pinMode(greenPho, INPUT);
 
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  
  base_red = analogRead(redPho); //receiving those base values on start up 
  base_blue = analogRead(bluePho);
  base_green = analogRead(greenPho);
}

void loop() {
  
  float minimum = 2000; //the sensors cannot exceed a value of 1023, so we know all values will be lower than 2000
  
  red_lev = analogRead(redPho); //We now take our raw values for the: red filter sensor
  blue_lev = analogRead(bluePho);//blue filter sensor
  green_lev = analogRead(greenPho);//green filter sensor

  //If a red sheet of paper where used as our input, the red sensor would receive the least amount of light (red screen filters red light)
  //This means that after we process our values to account for surrounding light, we need the values to be inversely proportional to what they are
  //To do that we simply take the max value for our LED (255) and subtract our processed values

  RED = 255-(base_red - red_lev);
  BLUE = 255-(base_blue - blue_lev);
  GREEN =255-(base_green - green_lev);

  //Now we have a set of RGB values, however the LED is in RYB
  //We know some amount of Yellow + some amount of Blue will = Green
  //Through testing the LED it was found that this equation is a decent approximation:
  // (8/10)*YELLOW + (2/10)*BLUE = GREEN
  // Solving for yellow we get:
  
  YELLOW = (10/8)*GREEN -(1/4)*BLUE;

  //Now we have a set of RYB values, however even when pure colours like red are placed in our system, our RYB values will all be non-zero
  //This is obviously and issue since the colour Green shouldn't have any red or blue pigment for example
  //To deal with this we will standardize our RYB values and have the lowest value become zero
  //The rest of the values will now be relative to zero rather than some random value

  //Putting our RYB values in an array
  int colour_array[3] = {RED, YELLOW, BLUE};
  //Finding our minimum value
  for(int i =0; i<3;i++){
    if(colour_array[i] <= minimum){
      minimum = colour_array[i];
    }
  }
  //Now we standardize
  red = RED-minimum;
  yellow = YELLOW-minimum;
  blue = BLUE-minimum;

  //Finally we can output our values to the LED, however the it should be noted that the LED is made of three smaller LEDs (red, yellow, blue)
  //The red led has a much greater intensity and thus its values have been reduced to a fraction of its original value
  analogWrite(redpin, 0.3*red);
  analogWrite(bluepin, blue);
  analogWrite(greenpin, yellow);
  
}
