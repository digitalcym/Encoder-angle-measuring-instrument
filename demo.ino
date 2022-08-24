#define ENCODER_A1_PIN 2 // 编码器1 黑线
#define ENCODER_A2_PIN 3 // 编码器2 黑线
#define ENCODER_B1_PIN 4 // 编码器1 白线
#define ENCODER_B2_PIN 5 // 编码器2 白线
float rad1=0.000;
float rad2=0.000;
volatile long counter1 = 0; // 定义 counter 计数器，用于计数
volatile long counter2 = 0;
int temp_a1;
int temp_b1;
int temp_a2;
int temp_b2;
float last_reading1 = 0;
float last_reading2 = 0;
int select;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  pinMode(ENCODER_A1_PIN, INPUT_PULLUP);// 因为编码器E6B2-CWZ6C为集电极开路，所以需将中断的引脚设置为输入PULLUP模式，上拉电阻
  pinMode(ENCODER_B1_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A2_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B2_PIN, INPUT_PULLUP);

  // 使用attachInterrupt设定绑定中断功能的引脚、触发时要执行的函式、以及执行方式
  attachInterrupt(0, interrupt_a1_change, CHANGE);//0指引脚2
  attachInterrupt(1, interrupt_a2_change, CHANGE);//1指引脚3
}


void loop() {
  if(counter1>11988){
    counter1=0;//角度超过360度时清零
  }
  if (counter1<0){
    counter1=11988.;//角度小于零度时重置
  }
   rad1=(360./1998./6.)*counter1;
  if(counter2>11988){
    counter2=0;
  }
  if(counter2<0){
    counter2=11988;
  }
  rad2=(360./1998./6.)*counter2;
  
  if (last_reading1 != rad1||last_reading2 != rad2) {
    Serial.print("方位角");
    Serial.print(String(rad1,8)); // 串口显示 小数点后八位
    Serial.print("度");
    Serial.print("   ");
    Serial.print("俯仰角");
    Serial.print(String(rad2,8));// 串口显示 小数点后八位
    Serial.println("度");
    last_reading1 = rad1;
    last_reading2 = rad2;
  }

  if (Serial.available()!=0){ //判断串口缓冲区有无输入
    select=Serial.parseInt(); //接受串口缓冲区输入
    if (select==1){
    counter1=0;}
    else {
    counter2=0;} 
    while(Serial.read() >= 0){ //清空串口缓冲区
    }
  }
}

// 引脚A1中断时，调用的函数
void interrupt_a1_change() { 

   temp_a1 = digitalRead(ENCODER_A1_PIN);
   temp_b1 = digitalRead(ENCODER_B1_PIN);

  // 判断编码器的选择方向
  if (temp_a1 == temp_b1)
  {
    counter1++; // 向后转 计数器-1
  } else {
    counter1--; // 向前转 计数器+1
  }
}

// 引脚A2中断时，调用的函数
void interrupt_a2_change() { 

   temp_a2 = digitalRead(ENCODER_A2_PIN);
   temp_b2 = digitalRead(ENCODER_B2_PIN);

  // 判断编码器的选择方向
  if (temp_a2 == temp_b2)
  {
    counter2++; // 向后转 计数器-1
  } else {
    counter2--; // 向前转 计数器+1
  }

}
