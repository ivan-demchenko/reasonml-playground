module UserFacingError = {
  type error = {
    error_type: string,
    message: string,
  };

  let make = (error_type, message) => {error_type, message};

  let toJson = err =>
    Json.Encode.(
      object_([
        ("error_type", err.error_type |> string),
        ("message", err.message |> string),
      ])
    );
};

let id = x => x;

let (<|) = (f, g, x) => f(g(x));

let safe_int_of_string = str =>
  switch (int_of_string(str)) {
  | x => Some(x)
  | exception (Failure(_)) => None
  };

let map_option = (f, opt) =>
  switch (opt) {
  | Some(x) => f(x)
  | _ => None
  };

let getDictString = (key, dict) =>
  map_option(Js.Json.decodeString, Js.Dict.get(dict, key));
