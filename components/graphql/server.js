
var express = require('express');
var { graphqlHTTP } = require('express-graphql');
var { buildSchema } = require('graphql');

// Construct a schema, using GraphQL schema language
var schema = buildSchema(`
  type WechatAccount{
    id: String!
  }
  type Account {
    id: String!
    name: String!
    wechatAccount: WechatAccount!
    signature: String!
  }
 
  type Query {
    account(id: String): Account
    test: String!
  }
`);

class WechatAccount {
  constructor(id) {
    this.id = id;
  }
  id() { return this.id; }
}

// This class implements the RandomDie GraphQL type
class Account {
  constructor(id) {
    this.id = id;
    this.name = 'niceice';
    this.wechatAccount = new WechatAccount('niceice220');
    this.signature = 'do is finished';
  }

  id() { return this.id; }
  name() { return this.name; }
  wechatAccount() { return this.wechatAccount; }
  signature() { return this.signature; }
}

// The root provides the top-level API endpoints
var root = {
  account: ({ id }) => { return new Account(id); },
  test: ()=>{return "test";}
}

var app = express();
app.use('/graphql', graphqlHTTP({
  schema: schema,
  rootValue: root,
  graphiql: true,
}));
app.listen(4000);
console.log('Running a GraphQL API server at http://localhost:4000/graphql');