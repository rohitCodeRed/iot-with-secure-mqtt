
const SENSOR_DATA = {
    "timestamp":"0",
    "topic":"sensor_data",
    "data":[
      {"sensor":"Temperature","value":"0","type":"celcius"},
      {"sensor":"Humidity","value":"0","type":"%"},
    //   {"sensor":"Vibration","value":"0","type":"binary"}
  ]};

function handle_subscription(mqttConnection,socketConnections,inData){
    //send data to all web site sockets...
    for(let i=0;i<socketConnections.length;i++){
        let formedData = formData(inData);
        socketConnections[i].send(JSON.stringify(formedData));
    }
    
}

function formData(data){
    
    let currentDate = new Date(Date.now());
    let message = data["message"];
    let result ={"topic":data["topic"]};
    let sensorData ={
        "timestamp":parseInt((currentDate.getTime())/1000),
        "data":[
            {"sensor":"Temperature","value":"0","type":"celcius"},
            {"sensor":"Humidity","value":"0","type":"%"}
        ]};

    let vibrationData = {
        "timestamp":parseInt((currentDate.getTime())/1000),
        "data":[
            {"sensor":"Vibration","value":"0","type":"binary"}
        ]
    };

    if(data){
        if(data["topic"] == "sensor_data"){
            let parser=":";
            let dataArr = message.split(parser);
            if (dataArr.length == 2){
                sensorData["data"][0]["value"] = dataArr[0];
                sensorData["data"][1]["value"] = dataArr[1];
                // dataObj["data"][2]["value"] = dataArr[2];
            }
            sensorData["topic"] = data["topic"];
            result = sensorData;

        }else if(data["topic"] == "vibrate_data"){
            vibrationData["data"][0]["value"] = (message=="Y") ? "1":"0";
            vibrationData["topic"] = data["topic"];
            result = vibrationData;
        }
        
    }
    
    return result;
}


module.exports ={
    handle_subscription
}