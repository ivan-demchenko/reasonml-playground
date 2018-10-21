open Express;
open Utils;
open HttpHelpers;

let app = express();

let sendBadRequest = msg =>
  Middleware.from((_, _, res) =>
    res |> statusBadRequest |> Response.sendString(msg)
  );

let sendNotFound = msg =>
  Middleware.from((_, _, res) =>
    res |> statusNotFound |> Response.sendString(msg)
  );

let onGet = (path, listMiddleware) =>
  App.getWithMany(app, ~path, listMiddleware);

let onPost = (path, listMiddleware) =>
  App.postWithMany(app, ~path, listMiddleware);

onGet(
  "/",
  [|CustomMiddleware.logging, CustomMiddleware.sendHtml("Hello there!")|],
);

let readIdParam =
  Either.from_option(("MissingArguments", "Missing param: [:id]"))
  <| getDictString("id")
  <| Request.params;

let parseId =
  Either.from_option(("BadArguments", "Wrong :id")) <| safe_int_of_string;

onPost(
  "/user/:id",
  [|
    CustomMiddleware.logging,
    Middleware.from((_, req, res) =>
      req
      |> readIdParam
      |> Either.flatMap(parseId)
      |> Either.fold(
           ((errType, errMsg)) =>
             res
             |> statusBadRequest
             |> sendJson(makeErrorJson(errType, errMsg)),
           userId => sendJson(makeSuccessJson(userId, "Bob"), res),
         )
    ),
  |],
);

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log("Listening at http://127.0.0.1:3000")
  };

App.listen(app, ~port=3000, ~onListen, ());
