'use strict';

const mqtt = require('mqtt');

class mQTT{
    constructor(url, port) {
        this.url = url;
        this.port = port;
        this.client = {};
      }

    connect(options){
        this.client  = mqtt.connect(this.url+':'+this.port,options);
        //console.log("Is client connected: ",this.client.connected);
        return this.client;
    }    
}

module.exports = {
    mQTT
}