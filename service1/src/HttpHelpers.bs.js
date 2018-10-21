// Generated by BUCKLESCRIPT VERSION 4.0.6, PLEASE EDIT WITH CARE
'use strict';

var Express = require("bs-express/src/Express.js");
var User$WebServer = require("./User.bs.js");
var Utils$WebServer = require("./Utils.bs.js");

function makeSuccessJson(id, name) {
  return User$WebServer.encodeUser(User$WebServer.create(id, name));
}

function makeErrorJson(error_type, message) {
  return Utils$WebServer.UserFacingError[/* toJson */1](Utils$WebServer.UserFacingError[/* make */0](error_type, message));
}

var statusNotFound = Express.Response[/* status */9](/* NotFound */23);

var statusBadRequest = Express.Response[/* status */9](/* BadRequest */19);

var sendJson = Express.Response[/* sendJson */3];

exports.makeSuccessJson = makeSuccessJson;
exports.makeErrorJson = makeErrorJson;
exports.statusNotFound = statusNotFound;
exports.statusBadRequest = statusBadRequest;
exports.sendJson = sendJson;
/* statusNotFound Not a pure module */