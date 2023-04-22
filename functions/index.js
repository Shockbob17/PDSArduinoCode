// const {Telegraf} = require("telegraf");
// const admin = require("firebase-admin");
// const serviceAccount = require("./pdsbotbot.json");

// // Initialize Firebase Admin SDK with service account credentials
// admin.initializeApp({
//   credential: admin.credential.cert(serviceAccount),
//   databaseURL: "https://pdsbotbot.firebaseio.com"});
// // Get a Firestore instance from the admin SDK
// const firestore = admin.firestore();


// const bot = new Telegraf("5979109221:AAELEJh107lLcY186al-Pcq78l2FgN5dcqI");
// const updateData = {
//   age: 31,
// };

// bot.start((ctx) => {
//   ctx.reply("You have entered the start command");
// });

// bot.command("hello", async (ctx) => {
//   // Access Firestore to get data
//   const snapshot = await firestore.collection("users").doc("123").get();
//   const data = snapshot.data();
//   ctx.reply(`Hello ${data.name}!`);
// });

// bot.command("rewrite", async (ctx) => {
//   // Access Firestore to get data
//   await firestore.collection("users").doc("123").update(updateData)
//       .then(() => {
//         ctx.reply("Document successfully updated!");
//       })
//       .catch((error) => {
//         ctx.reply("Error updating document: ", error);
//       });
// });

// bot.launch();


const functions = require("firebase-functions");
const admin = require("firebase-admin");
const {Telegraf} = require("telegraf");

admin.initializeApp();
// const db = admin.firestore();
const firestore = admin.firestore();
const realtime = admin.database();
const updateData = {
  age: 31,
};


const bot = new Telegraf(functions.config().telegram.token);

bot.start((ctx) => {
  ctx.reply("Welcome to the interface for tray count!");
  ctx.reply("Use /help to see how to use the bot!");
});

bot.help((ctx) => {
  ctx.reply("There exists 2 functions of this bot:");
  ctx.reply("/realtime : This will begin the scan of the trays.");
  ctx.reply("/realread : This will return the last count of the trays.");
});

bot.command("hello", async (ctx) => {
  // Access Firestore to get data
  const snapshot = await firestore.collection("users").doc("123").get();
  const data = snapshot.data();
  ctx.reply(`Hello ${data.name}!`);
});


bot.command("beginCount", async (ctx) => {
  // Access Firestore to get data
  await firestore.collection("users").doc("123").update(updateData)
      .then(() => {
        ctx.reply("Document successfully updated!");
        ctx.reply("Use the variable to ping arduino!");
      })
      .catch((error) => {
        ctx.reply("Error updating document: ", error);
        console.log("Error updating document: ", error);
      });
});

bot.command("count", async (ctx) => {
  const snapshot = await firestore.collection("me").doc("12").get();
  const data = snapshot.data();
  ctx.reply(`Stack 1: ${data.stack1}!`);
  ctx.reply(`Stack 2: ${data.stack2}!`);
  ctx.reply(`Stack 3: ${data.stack3}!`);
  ctx.reply(`Stack 4: ${data.stack4}!`);
});

exports.telegramBot = functions.https.onRequest((req, res) => {
  bot.handleUpdate(req.body);
  res.status(200).end();
});

bot.command("realtime", async (ctx) => {
  // Store a new value on the firebase
  realtime.ref("com").set({
    active: 1,
  });
  ctx.reply("Starting the Scan.");
  // Read a value from the firebase
});

bot.command("realread", async (ctx) => {
  // Store a new value on the firebase
  const dataFromDb1 = await admin.database().ref("tray/1").once("value");
  const dataFromDb2 = await admin.database().ref("tray/2").once("value");
  const dataFromDb3 = await admin.database().ref("tray/3").once("value");
  const dataFromDb4 = await admin.database().ref("tray/4").once("value");

  ctx.reply(`Trays in stack1: ${dataFromDb1.val()}`);
  ctx.reply(`Trays in stack2: ${dataFromDb2.val()}`);
  ctx.reply(`Trays in stack3: ${dataFromDb3.val()}`);
  ctx.reply(`Trays in stack4: ${dataFromDb4.val()}`);
  // Read a value from the firebase
});
