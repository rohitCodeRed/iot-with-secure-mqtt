const express = require('express'); //Import the express dependency
const app = express();              //Instantiate an express app, the main work horse of this server
const TEMPALTE_PATH = __dirname + "/templates";
const {mQTT} = require("./mqtt");
const websocket = require("ws");
const {handle_publish} = require("./mqtt_publisher");
const {handle_subscription} = require("./mqtt_subscriber");


const SOCKET_CLIENT_CONNECTIONS =[];

const MQTT_URL = "mqtt://192.168.8.103";
const MQTT_PORT ="3000";
const SERVER_URL = "http://192.168.8.103";
const SERVER_PORT = 4000;

const TOPICS=["iot_temperature","iot_humidity","iot_vibration","rgb_light"];

//Idiomatic expression in express to route and respond to a client request
app.get('/', (req, res) => {        //get requests to the root ("/") will route here
    res.sendFile('index.html', {root: TEMPALTE_PATH});      //server responds by sending the index.html file to the client's browser
                                                        //the .sendFile method needs the absolute path to the file, see: https://expressjs.com/en/4x/api.html#res.sendFile 
});

const server  = app.listen(SERVER_PORT, () => {            //server starts listening for any attempts from a client to connect at port: {port}
    console.log(`Now listening on port ${port}`); 
});




//Mqtt connection...
let sMQTT = new mQTT(MQTT_URL,MQTT_PORT);
let options = {
    clientId:"IOT_1",
    //port:8883,
    //host:'192.168.1.71',
    //protocol:'mqtts',

    /*rejectUnauthorized : false,
    //if using client certificates
    key: KEY,
    cert: CERT,
    //
    ca:caFile */
    
};

const mConnection = sMQTT.connect(options);

mConnection.on('connect', function () {
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

connection.on("error",function(error){ 
    console.log("Can't connect"+error);
});


//----------web socket...........
const wss = new websocket.Server({server:server,path:"/iot-sensors"});

wss.on("connection",(ws,req)=>{

    //SOCKET_CLIENT_CONNECTIONS.set(req.clientId,ws);
    SOCKET_CLIENT_CONNECTIONS.push(ws);
    ws.send("Welcome to the websocket server!!");

    ws.on("message",(msg)=>{
        console.log("Websocket working...",msg);
        let pushData = {
            topic:TOPICS[3],
            data:msg
        }

        handle_publish(mConnection,ws,pushData);
        //TODO from web site...
    });
});