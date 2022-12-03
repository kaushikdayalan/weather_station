const express = require('express');
const router = express.Router();
const AWS = require('aws-sdk');

AWS.config.update({
    region: 'us-east-1',
    accessKeyId: 'AKIA44FNDXAESABAMAUF',
    secretAccessKey: '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'
});

const dynamodb = new AWS.DynamoDB.DocumentClient();
const dynamodbTableName = "weather_data";

router.get('/all', async (req, res)=>{
    const params = {
        TableName: dynamodbTableName
    }
    try{
        const allWeatherData = await scanDynamoRecords(params, []);
        const body = {
            weatherData: allWeatherData
        }
        res.json(body);
    }
    catch(error){
        console.error("error: /all ", error);
        res.status(500).send(error);
    }
});

async function scanDynamoRecords(scanParams, itemArray){
    try{
        const DynamoData = await dynamodb.scan(scanParams).promise();
        itemArray = itemArray.concat(DynamoData.Items);
        if(DynamoData.LastEvaluatedKey){
            scanParams.ExclusiveStartKey = DynamoData.LastEvaluatedKey;
            return await scanDynamoRecords(scanParams, itemArray);
        }
        return itemArray;
    }catch(error){
        throw new Error(error);
    }
}

module.exports = router;
