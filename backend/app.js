const express = require('express');
const app = express();
const port = process.env.PORT || 3000;

const weatherStationRouter = require('./routes/weather');

app.use(express.json());

app.use('/weather', weatherStationRouter);

app.get('/', (req, res) =>{
    res.status(200);
    res.send('<h1>Node.js CURD API for the weather station</h1><h4>Message: Success</h4><p>Version: 1.0</p>')
})

app.listen(port, ()=> {
    console.log("Weather station API online \n port: " + port)
})