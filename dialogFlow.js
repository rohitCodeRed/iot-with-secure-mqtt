

function handleGoogleRequest(req,res,mConnection,topic){
    //console.log("DialogFlow Request: ",req.body);

    let queryResult = req.body["queryResult"];
    if(queryResult["parameters"].hasOwnProperty("sensor")){
        let sensor = queryResult["parameters"]["sensor"];
        let action = queryResult["parameters"]["sensor_actions"];

        if(action =="on"){
            //console.log("Data publish..")
            mConnection.publish(topic,"Y");
        }else if(action=="off"){
            //console.log("Data publish..")
            mConnection.publish(topic,"N");
        }

        res.send(formResponseJson(`${sensor} is ${action}, thank you.`));
        return;
    }
    res.send(formResponseJson("Not able to perform action on sensor"));

}

function formResponseJson(message){
    let jsonObj ={
        "payload": {
            "google": {
              "expectUserResponse": true,
              "richResponse": {
                "items": [
                  {
                    "simpleResponse": {
                      "textToSpeech": message
                    }
                  }
                ]
              }
            }
          },
        "fulfillmentMessages": [
            {
              "text": {
                "text": [
                    message
                ]
              }
            }
          ]
    }

    return jsonObj;
}

module.exports={
    handleGoogleRequest
}