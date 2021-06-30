const { MongoClient } = require("mongodb");

// Replace the following with values for your environment.
const username = encodeURIComponent("dev");
const password = encodeURIComponent("dev");
const clusterUrl = "127.0.0.1";

const authMechanism = "DEFAULT";

// Replace the following with your MongoDB deployment's connection string.
const uri =
  `mongodb+srv://${username}:${password}@${clusterUrl}/?authMechanism=${authMechanism}`;

// Create a new MongoClient
const client = new MongoClient(uri);

// Function to connect to the server
async function run() {
  try {
    // Connect the client to the server
    await client.connect();

    // Establish and verify connection
    await client.db("admin").command({ ping: 1 });
    console.log("Connected successfully to server");
  } finally {
    // Ensures that the client will close when you finish/error
    await client.close();
  }
}
run().catch(console.dir);