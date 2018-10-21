open Express;

let app = express();

let flatMap_option = (f, opt) =>
  switch (opt) {
  | Some(x) => f(x)
  | _ => None
  };

let map_option = (f, opt) =>
  switch (opt) {
  | Some(x) => Some(f(x))
  | _ => None
  };

let getDictString = (dict, key) =>
  Js.Dict.get(dict, key) |> flatMap_option(Js.Json.decodeString);

let getDictFloat = (dict, key) =>
  Js.Dict.get(dict, key)
  |> flatMap_option(Js.Json.decodeNumber)
  |> map_option(int_of_float);

let respondWithNotFound = Response.sendStatus(Response.StatusCode.NotFound);

let sendNotFound = Middleware.from((_, _) => respondWithNotFound);

let onGet = (path, listMiddleware) =>
  App.getWithMany(app, ~path, listMiddleware);

let onPost = (path, listMiddleware) =>
  App.postWithMany(app, ~path, listMiddleware);

let safe_int_of_string = str =>
  switch (int_of_string(str)) {
  | x => Some(x)
  | exception (Failure(_)) => None
  };

type either('l, 'r) =
  | Left('l)
  | Right('r);

let fold_either = (hl, hr, elr) =>
  switch (elr) {
  | Left(l) => hl(l)
  | Right(r) => hr(r)
  };

let eitherFromOption = (errMsg, opt) =>
  switch (opt) {
  | Some(x) => Right(x)
  | None => Left(errMsg)
  };

let logging =
  Middleware.from((next, req) => {
    let time = Js.Date.toLocaleDateString(Js.Date.fromFloat(Js.Date.now()));
    String.concat(
      " :: ",
      [time, Request.methodRaw(req), Request.path(req)],
    )
    |> Js.log;
    next(Next.middleware);
  });

let setPropertyNum = (req, key, num, res) => {
  let reqData = Request.asJsonObject(req);
  Js.Dict.set(reqData, key, Js.Json.number(float_of_int(num)));
  res;
};

let getOr = (def, opt) =>
  switch (opt) {
  | Some(x) => x
  | None => def
  };

let extractIdFromReqParams = req =>
  getDictString(Request.params(req), "id")
  |> flatMap_option(safe_int_of_string)
  |> eitherFromOption("Wrong ID in request params");

let axiosInstance =
  Axios.Instance.create(
    Axios.makeConfig(~baseURL="http://localhost:3000/", ()),
  );

let makePostRequet = url => Axios.Instance.post(axiosInstance, url);

let buildPath = conts => String.concat("/", conts);

let requestUser = id =>
  Js.Promise.(
    buildPath(["user", id])
    |> makePostRequet
    |> then_(resp => resp##data |> RemoteUser.decode |> resolve)
  );

onGet(
  "/read-user/:id",
  [|
    logging,
    Middleware.from((next, req, res) =>
      req
      |> extractIdFromReqParams
      |> fold_either(
           _ => next(Next.route, res),
           id =>
             setPropertyNum(req, "theValue", id, res)
             |> next(Next.middleware),
         )
    ),
    PromiseMiddleware.from((_, req, res) => {
      let reqData = Request.asJsonObject(req);
      Js.Promise.(
        getDictFloat(reqData, "theValue")
        |> getOr(0)
        |> string_of_int
        |> requestUser
        |> then_(user =>
             resolve(res |> Response.sendString(RemoteUser.present(user)))
           )
      );
    }),
  |],
);

let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => "Listening at http://127.0.0.1:4000" |> Js.log
  };

App.listen(app, ~port=4000, ~onListen, ());
