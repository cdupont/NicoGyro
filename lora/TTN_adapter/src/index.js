const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const axios = require('axios');

const port = 8000;
const app = express();
app.use(cors());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.post('/', async (req, res) => {
  const body = req.body;
    
  let wind_speed     = body.uplink_message.decoded_payload.analog_in_1
  let wind_direction = body.uplink_message.decoded_payload.analog_in_2
  let timestamp = Math.floor(new Date().getTime() / 1000) //body.uplink_message.rx_metadata.timestamp
  let val = {"provider": "flyman", 
             "password": "KJjxDr07VIWICPIjY1Gp", 
             "station": "1", 
             "timestamp": timestamp, 
             "direction": wind_direction,
             "vmoy": wind_speed}

  const res2 = await axios.post('https://intdata.spotair.mobi/balises/releve-put.php', val);

  res.send(res2.data);
});

app.listen(port, () => console.log(`Hello world app listening on port ${port}!`));
