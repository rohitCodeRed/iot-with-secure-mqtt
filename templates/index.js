const CURRENT_USER_INFO={
    url:"192.168.8.103",
    endPoint:"\/iot-sensors"
}

var socket = new WebSocket("ws:\/\/"+CURRENT_USER_INFO.url+endPoint);
    // Connection opened
    socket.addEventListener('open', (event) => {
        socket.send('Hello Server!, Send latest changed data..');
});

    // Listen for messages
    socket.addEventListener('message', (event) => {
        var serverData = JSON.parse(event.data);
        console.log(serverData);

        //TODO....

    });

    socket.addEventListener('close',()=>{
        console.log("Connection got closed.. bye bye server");
    });

    socket.addEventListener("error",(err)=>{
        console.log("Error occured", err);
    });


//----------Socket code end------------------------------------
 