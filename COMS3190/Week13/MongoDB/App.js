var express = require("express");
var cors = require("cors");
var app = express();
var fs = require("fs");
var bodyParser = require("body-parser");
app.use(cors());
app.use(bodyParser.json());
const port = "8081";
const host = "localhost";

const { MongoClient } = require("mongodb");
const url = "mongodb://127.0.0.1:27017";
const dbName = "secoms319";
const client = new MongoClient(url);
const db = client.db(dbName);

app.listen(port, () => {
    console.log("App listening at http://%s:%s", host, port);
});

app.get("/listRobots", async (req, res) => {
    await client.connect();
    console.log("node connected to GET MongoDB")

    const query = {};

    const results = await db.collection.find(query).limit(100).toArray();

    console.log(results);
    res.status(200);
    res.send(results);
});

app.get("/:id", async (req, res) => {
    const robotid = Number(req.params.id);
    await client.connect();
    console.log("Node successfully connected!");
    const query = { id: robotid };
    const results = await db.collection.findOne(query);

    console.log("Results: ", results);
    res.status(200);
    res.send(results);

});

app.post("/addRobot", async (req, res) => {
    try {
        await client.connect();
        const keys = Object.keys(req.body);
        const values = Object.values(req.body);
        const newDocument = {
            id: req.body.id,
            name: req.body.name,
            price: req.body.price,
            description: req.body.description,
            imageUrl: req.body.imageUrl,
        };
        console.log(newDocument);
        const results = await db.collection("robot").insertOne(newDocument);
        res.status(200);
        res.send(results);
    } catch (error) {
        console.error("An Error occurred: ", error);
        res.status(500).send({ "error, an internal server error has occurred"});
    }
});

app.put("/updateRobot/:id", async (req, res) => {
    const id = Number(req.params.id);
    const query = { id: id };
    await client.connect();
    console.log("Robot to Update :", id);
    // Data for updating the document, typically comes from the request body
    console.log(req.body);
    const updateData = {
        $set: {
            "name": req.body.name,
            "price": req.body.price,
            "description": req.body.description,
            "imageUrl": req.body.imageUrl
        }
    };
    // Add options if needed, for example { upsert: true } to create a document if it doesn't exist
    const options = {};
    const results = await db.collection("robot").updateOne(query, updateData, options);
    res.status(200);
    res.send(results);
});