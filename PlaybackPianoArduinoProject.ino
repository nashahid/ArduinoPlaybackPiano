int red_button = 13;
int green_button = 12;
int blue_button = 11;
int yellow_button = 10;
int black_button = 8;
int white_button = 9;
int light = 4;
int speaker = 5; //speaker needs to be on pin 5 which is PWM - sumulates an analog signal 


//////////////////////// defining constants for tones

int const red_sound = 392; 
int const green_sound = 328; 
int const blue_sound = 292; 
int const yellow_sound = 260;
int const notone = 20025;

///////////////////////// used to determine what sound is being loaded into the array during the populate_array function 

boolean red_pushed = false;
boolean green_pushed = false;
boolean blue_pushed = false;
boolean yellow_pushed = false; 
boolean no_button_pushed = false;

///////////////////////// element in the array that is incremented to add more notes to our playback array

int element = 0; 

//////////////////////// Used to keep time of the duration of a note being played in record mode

long start_butt_time = 0;
long end_butt_time = 0; 

////////////////////

const double rec_play_time = 10000; // Length of time for the record mode 

long start_time_record = 0; //Start time for entire recording sequence
long end_time_record = 0; //End time for entire recording sequence

boolean record_mode = false;  //tells us if the device is in record mode, at which time it can start registering notes and their duration

volatile int flag = LOW;      //flag indicating a button has been pushed and has triggered an interrupt 

struct sound{  
   unsigned long duration ;
   int note ; //frequency of sound 
};

// array of type sound to record the duration and time of each note played. Also stores data for playback when white button pushed
sound playback[150] = {{2000, 0}};  

//////////////////////////////////Millis variables
volatile unsigned long timer1_millis=0;
volatile unsigned long timer1_fract=0;
/////////////////////////////////

void setup(){
Serial.begin(9600); //9600 used for serial data transmision rate, rate of bits per second to the serial monitor 

// sets the function of the button to provide substantial amount of current from a low impedence state 
pinMode(red_button, OUTPUT);
pinMode(green_button, OUTPUT);
pinMode(blue_button, OUTPUT);
pinMode(yellow_button, OUTPUT);
pinMode(black_button, OUTPUT);
pinMode(white_button, OUTPUT);
pinMode(light, OUTPUT);
pinMode(speaker, OUTPUT);

//0 = first interupt pin (pin 2),  attatched to ISR "handelr", and is triggered whenever pin changes value 
attachInterrupt(0,handler,CHANGE);


//////////////////////////////Setup variables for Millis() function
TCCR1A = 0;//set this register to 0
//prescaler values
TCCR1B = (_BV(CS11) | _BV(CS10));
//enable timer1 overflow interrrupts
TIMSK1 = (1 << TOIE1);

TCNT1 = 0;

}

void loop(){
  
 start_time_record = 0;
 end_time_record = 0;  
  
 if (flag == HIGH){
 
    flag = LOW; 
   
  
  //check to see if the record button is pushed   

    if(digitalRead(black_button)==HIGH){
      if(record_mode==false){   
        
          start_time_record = millis(); 
          end_time_record = start_time_record + rec_play_time;
          record_mode = true;
          element = 0;
      }
    }

  //when the white button is pushed, light blinks and playback begins
    if (digitalRead(white_button)==HIGH){
    
      digitalWrite(light,HIGH); 
      delay(500); 
      digitalWrite(light,LOW);
      delay(500); 
      digitalWrite(light,HIGH);
      delay(500); 
      digitalWrite(light,LOW);
      delay(500); 
   
    
      for(int i=0; i < element; i++){
        
          tone(speaker,playback[i].note);  
          delay(playback[i].duration);
          noTone(speaker);   
      }
      
      record_mode = false; 
   
    }

 // If the record button was pushed now record the ensuing notes 
    if(record_mode == true){
      long record_time = 0;
      while (record_time < end_time_record){
          record_time = millis(); 
          digitalWrite(light,HIGH); 
      
          if(digitalRead(red_button)==HIGH){
                 //red button functions
                 
                 red_pushed = true; 
                 start_butt_time = millis(); 
                 while((digitalRead(red_button)==HIGH) && (record_time < end_time_record)){
                       //play the sound
                         tone(speaker,red_sound);
                         record_time = millis();             
                 }
                 
                 noTone(speaker); 
                 end_butt_time = millis(); 
                 populate_array(start_butt_time,end_butt_time,red_pushed, red_sound); 
                 element++;
          }
      
          if(digitalRead(green_button)==HIGH){
                 //green button functions;
                 
                 green_pushed = true; 
                 start_butt_time = millis(); 
                 while((digitalRead(green_button)==HIGH) && (record_time < end_time_record)){
                       //play the sound
                         tone(speaker,green_sound); 
                         record_time = millis(); 
                 }
             
                 noTone(speaker); 
                 end_butt_time = millis(); 
                 populate_array(start_butt_time,end_butt_time,green_pushed, green_sound); 
                 element++;
       
             
          } else if(digitalRead(blue_button)==HIGH){
                  //blue button functions
     
                  blue_pushed = true;
                  start_butt_time = millis(); 
                  while((digitalRead(blue_button)==HIGH) && (record_time < end_time_record)){
                       //play the sound
                         tone(speaker,blue_sound); 
                         record_time = millis();         
                  }

                  noTone(speaker); 
                  end_butt_time = millis(); 
                  populate_array(start_butt_time,end_butt_time,blue_pushed, blue_sound); 
                  element++;
     
             
          } else if(digitalRead(yellow_button)==HIGH){
                  //yellow button
                  yellow_pushed = true;
                  start_butt_time = millis(); 
                  while((digitalRead(yellow_button)==HIGH) && (record_time < end_time_record)){
                       //play the sound
                         tone(speaker,yellow_sound); 
                         record_time = millis();      
                  }
             
                  noTone(speaker); 
                  end_butt_time = millis(); 
                  populate_array(start_butt_time,end_butt_time,yellow_pushed, yellow_sound); 
                  element++;
     
             
          } else if(  (digitalRead(yellow_button)== LOW) && (digitalRead(blue_button)== LOW) && (digitalRead(green_button)== LOW) && (digitalRead(red_button)== LOW)){
                  //no button is pushed
            
                  no_button_pushed = true;
                  start_butt_time = millis();
                  while( (digitalRead(yellow_button)== LOW) && (digitalRead(blue_button)== LOW) && (digitalRead(green_button)== LOW) && (digitalRead(red_button)== LOW) && (record_time < end_time_record) ){
                        //output no tone
                        tone(speaker,notone);
                        record_time = millis();
                  }
              
                  noTone(speaker);
                  end_butt_time = millis(); 
                  populate_array(start_butt_time,end_butt_time,no_button_pushed, notone); 
                  element++;  
      
          } 
 

      } //while loop for counting to 15s
      
      digitalWrite(light,LOW);
      record_mode = false; 
     
  } //if statement for record_mode = true
 
 }
      

}
 

void handler(){
   flag = !flag; 

}

void populate_array(long start_time, long end_time, boolean &which_button, int note){
 
  if (which_button == true) {
     playback[element].note = note ;

     playback[element].duration = (end_time - start_time);
   
     which_button = false; 
  } 
  
  
}

/////////Millis() Function///////////////////////

//Millis function interrupt
//uses 16 bit (1) more accurate
ISR(TIMER1_OVF_vect){
    timer1_millis +=262;
    timer1_fract += 144;
    if (timer1_fract >= 1000) {
      timer1_fract -= 1000;
      timer1_millis += 1;
    }
}

//Millis() function only return timer1_millis
unsigned long Millis(){    
    return timer1_millis;
}



