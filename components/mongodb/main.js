
const mongoose = require('mongoose');


let mongoHost = process.env.MONGO_SERVICE_HOST;
if (typeof mongoHost === 'undefined') {
  mongoHost = '127.0.0.1';
}
const connectUrl = `mongodb://${mongoHost}:27017/account`;
console.log(connectUrl);
const mongooseConnect = () => {
  mongoose.connect(connectUrl, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    server: { auto_reconnect: true }
  });
};

try {
  mongooseConnect();
} catch (e) {
  console.log('mongodb connect error.');
  setTimeout(mongooseConnect, 1000);
}

const db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function () {
  console.log('mongodb connected');
});
db.on('disconnected', function () {
  console.log('mongodB disconnected');
  mongooseConnect();
});
const AccountCollection = mongoose.model('account', {
  _id: String,
  name: String,
  info: String,
  contact: [
    {
      id: String
    }
  ]
});

async function main(){
  const id = 'ice';
  const queryResult = await AccountCollection.findById(id);
  if (queryResult == null) {
    throw `user does not exist <id:${id}>`;
  }

  queryResult.updateOne({
    $push: {
      contact: {
        id: 'wzh' 
      }
    }
  }).then(() => console.log(`add contact has save`));

  const accountResult = await AccountCollection.findById(id);
  if (accountResult == null) {
    throw `user does not exist <id:${id}>`;
  }
}

main();