#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 //код адаптирован для платы "wemos d1 mini (esp8266)" с модулем wi-fi
    //ПАРОЛИ И СЕТИ

const char* ssid = "Имя вашей сети";
const char* pass = "пароль от этой сети";

    //ПИНЫ
const int pinD5_Green = D5; // Зеленый
const int pinD6_Red = D6; // Красный
const int pinD7_Blue = D7; //Синий


ESP8266WebServer server(80);

void setup() {
  pinMode(pinD5_Green, OUTPUT);
  pinMode(pinD6_Red, OUTPUT);
  pinMode(pinD7_Blue, OUTPUT);
  Serial.println("Пины настроены");


  Serial.begin (115200);
  WiFi.begin(ssid, pass); // ПАРОЛЬ И ИМЯ СЕТИ!!  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("Connected");
  Serial.println(WiFi.localIP());

   // Определяем обработчик для корневого URL
  server.on("/light", handleLight);

  server.begin();
  Serial.println("Сервер запущен");

}

void loop() {
  server.handleClient(); // для входящих запросов (обработка)
}

// пасхолко

void handleRoot() {
  String html = "<form action=\"/submit\" method=\"POST\">";
  html += "<a href='https://www.youtube.com/watch?v=ARt4KSNxGNw'>Sosal?</a>";
  html += "<input type=\"text\" name=\"value\">";
  html += "<input type=\"submit\" value=\"send\">";
  html += "</form>";
  server.send(200, "text/html", html);
}

void handleLight() {
  //server.send(200, "text/plain", "hello");
  if (server.method() == HTTP_GET) {
    // Получаем параметры из URL
    String state = server.arg("state"); 
    String color = server.arg("color"); 
    String message = server.arg("message");
    int R = server.arg("D6").toInt(); //сами параметры (1 или 0)
    int G = server.arg("D5").toInt();
    int B = server.arg("D7").toInt();
    // присваимваем параметры в стринговые переменные для вывода в консоль
    String r = String(R); // сами параметры (1 или 0), но в виде стринги
    String g = String(G);
    String b = String(B);

    // Выводим полученные значения в Serial Monitor
    Serial.println("\nMessage: " + message);
    Serial.println("State: " + state);
    Serial.println("Color: " + color);
    Serial.println("D5: " + g);
    Serial.println("D6: " + r);
    Serial.println("D7: " + b);
     
    if (R == 1){
      digitalWrite(pinD6_Red, HIGH);
    }
    else if (R == 0){
        digitalWrite(pinD6_Red, LOW);
    }
    if (G == 1){
      digitalWrite(pinD5_Green, HIGH);
    }
    else if (G == 0){ ////////////
        digitalWrite(pinD5_Green, LOW);
    }
    if (B == 1){
      digitalWrite(pinD7_Blue, HIGH);
    }
    else if (B == 0){
        digitalWrite(pinD7_Blue, LOW);
    }
    

    // Отправляем ответ клиенту
    String response = "Message: " + message + ", State: " + state + ", Color:" + color + ", RGB: " + r + "." + b + "." + b + ".";
    server.send(200, "integer/plain", response);
  } else {
    server.send(405, "text/plain", "Метод не поддерживается");
  }
}



