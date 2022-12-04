
function handle_publish(mqttConnection,socketConnection,inData){
    if(mqttConnection){
        mqttConnection.publish(inData.topic, inData["data"]["value"]);
    } 
}


module.exports ={
    handle_publish
}