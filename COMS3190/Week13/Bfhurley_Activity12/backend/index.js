var express = require("express");
var cors = require("cors");
var app = express();
var fs = require("fs");
var bodyParser = require("body-parser");

app.use(cors());
app.use(bodyParser.json());

const port = "8080";
const host = "localhost";

const { MongoClient } = require("mongodb");

const url = "mongodb://127.0.0.1:27017";
const dbName = "secoms319";
const client = new MongoClient(url);
const db = client.db(dbName);

app.listen(port, () => {
    console.log("App listening at http://%s.%s", host, port);
});

app.get("/listRobots", async (req, res) => {
    await client.connect();
    console.log("Node connected successfully to GET MongoDB");
    const query = {};
    const results = await db
        .collection("robot")
        .find(query)
        .limit(100)
        .toArray();
    console.log(results);
    res.status(200);
    res.send(results);
});

// Add a new robot into the collection
app.post("/addRobots", async (req, res) => {
    try {
        await client.connect();
        const newDocument = {
            id: req.body.id,
            name: req.body.name,
            price: req.body.price,
            description: req.body.description,
            imageUrl: req.body.imageUrl,
        };
        const result = await db.collection("robot").insertOne(newDocument);
        res.status(200);
        res.send(result);
    } catch (error) {
        console.error("Could not add the new Robot" + error);
        res.status(500);
        res.send("Error adding new robot");
    } finally {
        await client.close();
    }
});

app.put("/updateRobot/:id", async (req, res) => {
    try {
        await client.connect();
        const updateDoc = {
            $set: req.body,
        };
        const result = await db
            .collection("robots")
            .updateOne({ id: Number(req.params.id) }, updateDoc);
        if (result.modifiedCount === 0) {
            return res.status(404).send({ error: "No robot found with that ID" });
        }
        res.status(200).send(result);
    } catch (error) {
        console.error("Error updating robot:", error);
        res.status(500).send({ error: "An internal server error occurred" });
    } finally {
        await client.close();
    }
});

app.delete("/deleteRobot/:id", async (req, res) => {
    try {
        const id = Number(req.params.id);
        await client.connect();
        console.log("Robot to delete :", id);
        const query = { id: id };
        // read data from robot to delete to send it to frontend
        const robotDeleted = await db.collection("robot").findOne(query);
        if (robotDeleted) {
            // delete
            const results = await db.collection("robot").deleteOne(query);
            res.status(200);
            res.send(results);
        } else {
            res.status(404);
            res.send("Robot Not Found");
        }
    } catch (error) {
        console.error("Error deleting robot:", error);
        res.status(500).send({ message: "Internal Server Error" });
    }
});