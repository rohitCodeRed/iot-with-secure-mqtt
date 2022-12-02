
function handle_subscription(mqttConnection,socketConnections,data){

    //send data to all web site sockets...
    for(let i=0;i<socketConnections.length;i++){
        socketConnections[i].send(data);
    }
    
}


module.exports ={
    handle_subscription
}