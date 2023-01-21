int flag = 0; 
int input_pin = A3;
int LED = 13; 
int val = 0;
int count;
float EMA_a = 0.7;    // initialization of EMA alpha
int EMA_S = 0;        // initialization of EMA S
float threshold;
float filtered_voltage;
int temp = 0;
bool is_counting;

void setup() 
{   
  Serial.begin(9600); 
  count = 0;
  is_counting = 1;
  filtered_voltage = 0.0;
  threshold = 3.5;
  EMA_S = analogRead(input_pin);
  temp = 0;
} 
void loop() 
{   
  val = analogRead(input_pin);
  //  Serial.print(" Absolute Voltage:");
  //  Serial.println((val/1023.00)*5.00);
  //  Serial.print(" Absolute Parts:");
  //  Serial.println(val);
  
  EMA_S = (EMA_a*val) + ((1-EMA_a)*EMA_S);
  filtered_voltage = (EMA_S/1023.00)*5.00;
  
  Serial.print(" Filtered Voltage:");
  Serial.println(filtered_voltage);
  Serial.print(" Filtered Parts:");
  Serial.println(EMA_S);

  if ((filtered_voltage > threshold) && is_counting)
  {
    is_counting = !is_counting;
    count++;
  }
  else if ((filtered_voltage < threshold) && !is_counting)
  {
    is_counting = !is_counting;
  }
  Serial.print(" Step Count:");
  Serial.println(count);
  Serial.println("                                      ");
  
  delay(1000);
}  
