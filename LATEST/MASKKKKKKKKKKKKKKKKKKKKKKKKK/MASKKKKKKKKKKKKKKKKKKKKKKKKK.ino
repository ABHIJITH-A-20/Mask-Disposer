#define IR1 3
#define IR2 4
#define IR3 5
#define DCwater_pump 11
#define coil 6

// Motor A connections
int in1 = 10;
int in2 = 9;
// Motor B connections
int in3 = 8;
int in4 = 7;
int RESET = 12;
// CONSTANTS FOR IR INPUT STORAGES
int H=0,M=0,L=0,C=0,K,d,W=0,J=1,F=0,E=0,PO=0,CO,PC,I=0;


//DELAY OF LID CLOSE
unsigned long L_CLS_Started = 0;
const long interval_L_CLS = 1000;

//DELAY OF HAND SANITIZER
unsigned long H_Started = 0;
const long interval_H =1000;

//DELAY OF PASSAGE OPEN
unsigned long PASSAGE_Started = 0;
const long interval_PASSAGE = 1000;

//DELAY OF PASSAGE CLOSE
unsigned long PASSAGE_CLS_Started = 0;
const long interval_PASSAGE_CLS = 1000;
int PT=10;

//DELAY OF COIL FIRING
unsigned long COIL_Started = 0;
const long interval_COIL = 1000;
int CT=10;
int Q=5;
unsigned long currentMillis = millis();


//-------------------------------------------------------------------------------

void setup()
{
pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(DCwater_pump, OUTPUT);
  pinMode(coil, OUTPUT);
  //RESET ARDUINO
  pinMode(RESET, OUTPUT);
 
    // Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  Serial.begin(9600);
}
//-------------------------------------------------------------------------------

void loop()
  {
    san_disp();
    lid();
    mask_count();
    it_burns();

    }
//-------------------------------------------------------------------------------

// SANITIZER DISPENSER
void san_disp()
{unsigned long currentMillis = millis();
       H = digitalRead(IR3);
              if (H ==LOW)
                {
                    if (currentMillis - H_Started >= interval_H) 
                    {
                      H_Started = currentMillis;
                      digitalWrite(DCwater_pump, LOW);
                      Serial.println("  pump off ");
                    }
                    
                    else
                     {             
                              digitalWrite(DCwater_pump, HIGH);
                              Serial.print("  pump on ");
                     }
                } 
      
               else
                     {             
                              digitalWrite(DCwater_pump, LOW);
                              Serial.print("  pump off 2");
                     }
}
//-------------------------------------------------------------------------------

//LID opening 
void lid()
{
      L = digitalRead(IR1);
          if (L==LOW)
            { 
                      // Turn on motor A / OPEN lid
                  digitalWrite(in1, HIGH);
                  digitalWrite(in2, LOW);
                  Serial.println("  lid open");
            }
      
          else
              {
                  // Turn off motor A
                digitalWrite(in1, LOW);
                digitalWrite(in2, LOW);
              }

}
//-------------------------------------------------------------------------------

//count the mask and close the lid.
void mask_count()
{
           M = digitalRead(IR2);
             if (M==LOW)
             {d=1;
                }
      if(d==1){ 
        unsigned long currentMillis_M = millis();  
       if (currentMillis_M - L_CLS_Started >= interval_L_CLS) 
                { 
                  L_CLS_Started = currentMillis_M;
                  d=0;
                  K++;
                  I=1;
                  Serial.println("  ");
                  Serial.println(K);
                }
                  
                 else{
                  // Turn on motor A / close lid
                  digitalWrite(in1, LOW);
                  digitalWrite(in2, HIGH);
                  Serial.println("  lid close");
                 }                 
              }             

     if(K%Q == 0 && K!=0)
             {
                      C=Q;
                      Serial.println(C); 
                      if(I==1){
                        J=1;                         
                      }
               } 

}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------

// Burning the Masks
void it_burns()
{
  if(C==Q && d==0){
    Serial.println("  IM HERE ");
             
           if(J==1){ unsigned long currentMillis_P = millis();
              if (currentMillis_P- PASSAGE_Started >= interval_PASSAGE) 
                 {  //OPEN PASSAGE
                      PASSAGE_Started = currentMillis_P;
                      PO++;
                      if(PO==PT){  
                      W=1;
                      }
                 }
               else{
                    E=0;
                    digitalWrite(in3, HIGH);
                    digitalWrite(in4, LOW);  
                    Serial.println("  ");
                    Serial.print("  passage open");
               }
            }
     if(W==1){ unsigned long currentMillis_P_C = millis();
               // closs PASSAGE 
               if (currentMillis_P_C - PASSAGE_CLS_Started >= interval_PASSAGE_CLS){
                    PASSAGE_CLS_Started = currentMillis_P_C;   
                    PC++;
                    if(PC==PT){
                      E=1;
                     
                    }
                 }
               else{
                    J=0;
                    I=0;
                    digitalWrite(in3, LOW);
                    digitalWrite(in4, HIGH);
                    Serial.println("  ");  
                    Serial.print("  passage CLOSE");
                  }
                }
                   //coil burning
     if(E==1){
           W=0;
           digitalWrite(in3, LOW);
           digitalWrite(in4, LOW);
           digitalWrite(coil, HIGH);
           Serial.println("  "); 
           Serial.print("  coil burning ");
           delay(1000);
           digitalWrite(coil, LOW);
           Serial.print("  coil ENDS ");
           delay(1000);
           digitalWrite(coil, HIGH);
           delay(1000);
           digitalWrite(coil, LOW);
           delay(1000);
           digitalWrite(coil, HIGH);
           delay(1000);
           digitalWrite(coil, LOW);
           Serial.println("  ");  
           Serial.print("  coil ENDS ");
           CO=0;
           PC=0;
           PO=0;
           E=0;
     }

      }
 
    } 

//------------------------------------------------------------------------------

//END of FILE
