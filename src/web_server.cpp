#include <WebServer.h>
#include <web_config.h>
#include <ArduinoJson.h>
#include <data.h>

WebServer server(80);

String SendHTML(){
  String ptr = R"V0G0N(
    <!DOCTYPE html> <html>
    <head><meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <title>Smoken PID control</title>
    <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}
    .button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}
    .button-on {background-color: #3498db;}
    .button-on:active {background-color: #2980b9;}
    .button-off {background-color: #34495e;}
    .button-off:active {background-color: #2c3e50;}
    p {font-size: 14px;color: #888;margin-bottom: 10px;}
    </style>
    </head>
    <body>
      <h1>Smoken Web Server</h1>
      <div id="chart-t1" class="container"></div>
      <a class="button button-off" href="/conf">Settings</a>
    </body>
    <script>
      var chartT = new Highcharts.Chart({
        chart:{ renderTo : 'chart-t1' },
        title: { text: 'Smoker Measurements' },
        series: [
          { name: "Smoker T", data: []},
          { name: "Food T", data: []},
          { name: "Target T", data: []},
          { name: "PWM", data: []},
          { name: "RPM", data: []},
        ],
        plotOptions: {
          line: { animation: false,
            dataLabels: { enabled: false }
          }
        },
        yAxis: {
          title: { text: 'Temperature (Celsius)' }
        },
        credits: { enabled: false }
      });
      setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            var result = JSON.parse(this.responseText);
            //console.log(result.data);
            chartT.series[0].setData(result.t1);
            chartT.series[1].setData(result.t2);
            chartT.series[2].setData(result.t3);
            chartT.series[3].setData(result.pwm);
            chartT.series[4].setData(result.rpm);
          }
        };
        xhttp.open("GET", "/data", true);
        xhttp.send();
      }, 3000 ) ;
    </script>
    </html>
  )V0G0N";

  return ptr;
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void addArray(DynamicJsonDocument &doc, std::string name, std::deque<double> dq, double factor) {
  JsonArray data = doc.createNestedArray(name);
  
  int tsize = dq.size();
  for(int i = 0; i < tsize; i++){
      data.add((int)(dq.at(i) / factor));
  }
}

void handle_data() {
  //StaticJsonDocument<1024> doc;
  DynamicJsonDocument doc(4096);
  dataMeasures dm = getMeasures();
  addArray(doc, "t1", dm.tmpsQ1, 1);
  addArray(doc, "t2", dm.tmpsQ2, 1);
  addArray(doc, "t3", dm.tmpsQ3, 1);
  addArray(doc, "pwm", dm.pwm, 1);
  addArray(doc, "rpm", dm.rpm, 1000);

  //doc["blah"] = 123;

  String output;
  serializeJson(doc, output);
  //serializeJson(doc, Serial);

  server.send(200, "application/json", output); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void initWebServer() {
  server.on("/", handle_OnConnect);
  server.on("/data", handle_data);
  //server.on("/led1on", handle_led1on);
  server.onNotFound(handle_NotFound);
  
  initWebConfig();

  server.begin();
  Serial.println("HTTP server started");
}
