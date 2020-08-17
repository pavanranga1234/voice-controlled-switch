#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid ="rocky";const char* password = "platinum";
const char* host = "olp.orlindustries.com";String apiKey ="12eee3cef877d1f486a1bc9a58dbde83";
String l_line="";

int i_flag=0;//in flag
int o_flag=0;//out flag
int it_flag=0;//cloud in flag
int ot_flag=0,ot_flag1=0,ot_flag2=0,ot_flag3=0;//cloud out flag
int man_flag=0;
int m_flag=0;//pir flag
int a=0;int b=0;int c=0;

void setup()
{
  bsp();
}

void loop()
{
  read_olp_dashboard();
}

void bsp()
{
  pinMode(D1,OUTPUT); // IN1
  
  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void read_olp_dashboard()
{
  WiFiClient client;
  //Serial.print("connecting to ");
  //Serial.println(host);
  if (!client.connect(host,80))
  {
    //Serial.println("connection failed");
    return;
  }
  String url = "http://orlindustries.com/olp/device_pull.php?device_api_key=" + String(apiKey);
  //Serial.print("requesting URL: ");
  //Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("Reading Data From Cloud");
  delay(1000);
  while(!client.available())
  {
     delay(1);
  }
  l_line = client.readString();
  //Serial.println(l_line);
  //delay(2000);
  if (l_line!= "")
  {
      int commaIndex=l_line.indexOf("Device Status");
      int secondCommaIndex = l_line.indexOf(',', commaIndex+1);
      int thirdCommaIndex=l_line.indexOf(',',secondCommaIndex+1);
      int fourthCommaIndex=l_line.indexOf(':',thirdCommaIndex+1);
      int fifthCommaIndex=l_line.indexOf('}',fourthCommaIndex+1);
      
      String s1= l_line.substring(0, commaIndex);
      String s2 =l_line.substring(commaIndex+1, secondCommaIndex);
      String s3 = l_line.substring(secondCommaIndex+1,thirdCommaIndex);
      String s4  = l_line.substring(thirdCommaIndex+1,fourthCommaIndex);
      String s5 = l_line.substring(fourthCommaIndex+1,fifthCommaIndex);

      /*Serial.print("S1:");
      Serial.println(s1);
      delay(1000);
      Serial.print("S2:");
      Serial.println(s2);
      delay(1000);
      /*Serial.print("S3:");
      Serial.println(s3);
      delay(1000);
      Serial.print("Field No:");
      Serial.println(s4);*/
      if(s2!="")
      {
        int commaIndex1=s2.indexOf(':');
        int secondCommaIndex1 = s2.indexOf('"', commaIndex1+1);
        String s51= s2.substring(0, commaIndex1);
        String s52 =s2.substring(commaIndex1+1, secondCommaIndex1);
        Serial.print("Field Data :");
        //Serial.println(s51);
        Serial.println(s52);
        if(s52.indexOf(" lighton<br/>")!=-1 && it_flag==0)
        {
          it_flag=1;
          ot_flag=0;
          Serial.println("Requested To Put Light On...");
          bulb_on();
        }
        else if(s52.indexOf(" lightoff")!=-1 && ot_flag==0)
        {
          ot_flag=1;
          it_flag=0;
          Serial.println("Requested To Put Light Off...");
          bulb_off();
        }
      }  
    }
    Serial.println("Data Read");
}
void bulb_on(void)
{
  digitalWrite(D1,LOW);
  delay(1000);
}

void bulb_off(void)
{
  digitalWrite(D1,HIGH);
  delay(1000);
}


