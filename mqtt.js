'use strict';

const mqtt = require('mqtt');

class mQTT{
    constructor(url, port) {
        this.url = url;
        this.port = port;
        this.client = {};
      }

    connect(options){
        this.client  = mqtt.connect('mqtt://'+this.url+':'+this.port,options);
        return this.client;
    }    
}

module.exports = {
    mQTT
}