const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const axios = require('axios');
const mqtt = require('mqtt')

const options = {'username': 'mydev@ttn',
                 'password': 'NNSXS.ZREHHZZYMBUFCENADJTYOP5H3NXHMMZ6XRQ7DLI.ENEXKPLQIZ6U2G64ENXHJOJVQST6OSFSUMY4ZAM5NL3GM3XBOP3A'}
const client  = mqtt.connect('mqtt://eu1.cloud.thethings.network', options)

client.on('connect', function () {
  client.subscribe('v3/mydev@ttn/devices/eui-a8610a33373a6a0c/up', function (err) {
    console.log("subscribe res: " + err)
  })
})

client.on('message', function (topic, message) {
  sendSpotAir(JSON.parse(message.toString()))
  client.end()
})


async function sendSpotAir(body) {
    
  console.log(JSON.stringify(body))
  let wind_speed     = body.uplink_message.decoded_payload.analog_in_1
  let wind_direction = body.uplink_message.decoded_payload.analog_in_2
  let timestamp = Math.floor(new Date().getTime() / 1000) //body.uplink_message.rx_metadata.timestamp
  let val = {"provider": "flyman", 
             "password": "KJjxDr07VIWICPIjY1Gp", 
             "station": "1", 
             "timestamp": timestamp, 
             "direction": wind_direction,
             "vmoy": wind_speed}

  console.log("posting value to spotair: " + JSON.stringify(val))
  const res2 = await axios.post('https://intdata.spotair.mobi/balises/releve-put.php', val);

}

