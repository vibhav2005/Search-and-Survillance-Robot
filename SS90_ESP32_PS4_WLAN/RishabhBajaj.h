const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>Web Page Update Demo</title>

  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
       padding: 3px 15px;
    }
    .tabledata {
      font-size: 24px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      //background-color: #00AA00;
    }
    .fanrpmslider {
      width: 30%;
      height: 55px;
      outline: none;
      height: 25px;
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 24px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 60px;
      margin: 0;
      padding: 20px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 50px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 44%;
    }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 32px;
      line-height: 50px;
      padding: 30px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 28px;
      text-align: left;
    }
  
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
    }
    .container {
      max-width: 1800px;
      margin: 0 2%;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }
    
  </style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">SS - 90</div>
            
          </div>
      </div>
    </header>
  
    <main class="container" style="margin-top:70px">
      <div class="category">Sensor Readings</div>
      <div style="border-radius: 10px !important;">
      <table style="width:50%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 20%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 15%; color:#000000 ;">
        <col span="1" style="background-color:rgb(180,180,180); width: 15%; color:#000000 ;">
      </colgroup>
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <tr>
        <th colspan="1"><div class="heading">Parameter</div></th>
        <th colspan="1"><div class="heading">Value</div></th>
      </tr>
      <tr>
        <td><div class="bodytext">Temperature (34)</div></td>
        <td><div class="tabledata" id = "temperature"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Humidity (35)</div></td>
        <td><div class="tabledata" id = "humidity"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">Smoke (33)</div></td>
        <td><div class="tabledata" id = "smoke"></div></td>
      </tr>
      </table>
    </div>
    <br>
    <div class="category">Toggle Button</div>
    <br>
    <div class="bodytext">LED  </div>
    <button type="button" class = "btn" id = "btn0" onclick="ButtonPress0()">Toggle</button>
    </div>
    <br>
    <br>

    <br>
  </main>
  
  </body>


  <script type = "text/javascript">
  
    var xmlHttp=createXmlHttpObject();

    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

    function ButtonPress0() {
      var xhttp = new XMLHttpRequest(); 
      var message;
       
      xhttp.open("PUT", "BUTTON_0", false);
      xhttp.send();
    }

    function response(){
      var message;
      var barwidth;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
  
      // A0
      xmldoc = xmlResponse.getElementsByTagName("TEMP"); //bits for A0
      message = xmldoc[0].firstChild.nodeValue;
      if (message > 40){
      color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      barwidth = message;
      document.getElementById("temperature").innerHTML=message;
      document.getElementById("temperature").style.width=(barwidth+"%");
      document.getElementById("temperature").style.backgroundColor=color;
      //document.getElementById("temperature").style.borderRadius="5px";
  
      // A1
      xmldoc = xmlResponse.getElementsByTagName("HUM");
      message = xmldoc[0].firstChild.nodeValue;
      if (message > 60){
      color = "#00aa00";
      }
      else {
        color = "#0000aa";
      }
      document.getElementById("humidity").innerHTML=message;
      width = message;
      document.getElementById("humidity").style.width=(width+"%");
      document.getElementById("humidity").style.backgroundColor=color;
      //document.getElementById("humidity").style.borderRadius="5px";

      // A2
      xmldoc = xmlResponse.getElementsByTagName("SMOKE");
      message = xmldoc[0].firstChild.nodeValue;
      if (message > 500){
      color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      document.getElementById("smoke").innerHTML=message;
      width = message / 10;
      document.getElementById("smoke").style.width=(width+"%");
      document.getElementById("smoke").style.backgroundColor=color;
      //document.getElementById("smoke").style.borderRadius="5px";
      
  
      xmldoc = xmlResponse.getElementsByTagName("LED");
      message = xmldoc[0].firstChild.nodeValue;
  
      if (message == 0){
        document.getElementById("btn0").innerHTML="Turn ON";
      }
      else{
        document.getElementById("btn0").innerHTML="Turn OFF";
      }
    }

    function process(){
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()",100);
    }
  
  
  </script>

</html>



)=====";
