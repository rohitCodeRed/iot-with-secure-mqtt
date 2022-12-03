const express = require('express'); //Import the express dependency
const app = express();              //Instantiate an express app, the main work horse of this server
const TEMPALTE_PATH = __dirname + "/templates";
const {mQTT} = require("./mqtt");
const fs = require('fs');
const websocket = require("ws");
const {handle_publish} = require("./mqtt_publisher");
const {handle_subscription} = require("./mqtt_subscriber");

app.set('view engine', 'ejs');


const SOCKET_CLIENT_CONNECTIONS =[];

const MQTT_URL = "mqtts://192.168.8.101";
const MQTT_PORT ="8883";
const SERVER_IP = "192.168.8.101";
const SERVER_SOCKET_ENDPOINT="iot_sensors";
const SERVER_URL = "http://192.168.8.101";
const SERVER_PORT = 4000;

const TOPICS=["iot_temperature","iot_humidity","iot_vibration","rgb_light"];

//Idiomatic expression in express to route and respond to a client request
app.get('/', (req, res) => {        //get requests to the root ("/") will route here
    //res.sendFile('index.html', {root: TEMPALTE_PATH});      //server responds by sending the index.html file to the client's browser
    res.render("pages/index",{
        "ip": SERVER_IP,
        "port":SERVER_PORT,
        "endPoint": SERVER_SOCKET_ENDPOINT
      })                                                    //the .sendFile method needs the absolute path to the file, see: https://expressjs.com/en/4x/api.html#res.sendFile 
});

const server  = app.listen(SERVER_PORT, () => {            //server starts listening for any attempts from a client to connect at port: {port}
    console.log(`Now listening on port ${SERVER_PORT}`); 
});




//Mqtt connection...
let sMQTT = new mQTT(MQTT_URL,MQTT_PORT);
let KEY = fs.readFileSync('./certs/client.key');
let CERT = fs.readFileSync('./certs/client.crt');
let CAfile = fs.readFileSync('./certs/ca.crt');
let options = {
    clientId:"IOT_1",
    rejectUnauthorized : false,
    key: KEY,
    cert: CERT,
    ca:CAfile 
};

const mConnection = sMQTT.connect(options);

mConnection.on('connect', function () {
    console.log("Mqtt Broker connected...");
    mConnection.subscribe(TOPICS, function (err) {
      if (!err) {
        console.log("Topics :"+TOPICS+" are subscribed succesfully");
      }
    })
});

mConnection.on('message', function (topic, message) {
    // message is Buffer
    console.log("Topic: ",topic," ",message.toString());
    let inData = {"topic":topic,"message":message};
    handle_subscription(mConnection,SOCKET_CLIENT_CONNECTIONS,inData);
    //client.end()
  });

mConnection.on("error",function(error){ 
    console.log("Can't connect-> "+error);
    mConnection.end();
});


//----------web socket...........
const wss = new websocket.Server({server:server,path:"/"+SERVER_SOCKET_ENDPOINT});

wss.on("connection",(ws,req)=>{
    console.log("Websocket working... client info:- ",req.headers.host," : ",req.headers['user-agent']);
    SOCKET_CLIENT_CONNECTIONS.push(ws);

    ws.on("message",(msg)=>{
        
        let pushData = {
            topic:TOPICS[3],
            data:msg
        }
        ws.send(JSON.stringify(pushData));

        //handle_publish(mConnection,ws,pushData);
        //TODO from web site...
    });
});