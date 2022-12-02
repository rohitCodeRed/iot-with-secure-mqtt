
function handle_publish(mqttConnection,socketConnection,data){

    mqttConnection.publish(data.topic, data.message);
}


module.exports ={
    handle_publish
}