
const SENSOR_DATA = {
    "timestamp":"0",
    "data":[
      {"sensor":"Temperature","value":"0","type":"celcius"},
      {"sensor":"Humidity","value":"0","type":"%"},
      {"sensor":"Vibration","value":"0","type":"binary"}
  ]};

function handle_subscription(mqttConnection,socketConnections,inData){
    //send data to all web site sockets...
    for(let i=0;i<socketConnections.length;i++){
        let formedData = formData(inData["message"]);
        socketConnections[i].send(JSON.stringify(formedData));
    }
    
}

function formData(data){
    let parser=":";
    let currentDate = new Date(Date.now());
    let dataObj ={
        "timestamp":parseInt((currentDate.getTime())/1000),
        "data":[
            {"sensor":"Temperature","value":"0","type":"celcius"},
            {"sensor":"Humidity","value":"0","type":"%"},
            {"sensor":"Vibration","value":"0","type":"binary"}
        ]};

    if(data){
        let dataArr = data.split(parser);
        if (dataArr.length == 3){
            dataObj["data"][0]["value"] = dataArr[0];
            dataObj["data"][1]["value"] = dataArr[1];
            dataObj["data"][2]["value"] = dataArr[2];
        }
    }
    
    return dataObj;
}


module.exports ={
    handle_subscription
}