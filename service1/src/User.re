type user = {
  id: int,
  name: string,
};

let create = (id: int, name) => {id, name};

let encodeUser = user =>
  Json.Encode.(
    object_([
      ("id", user.id |> string_of_int |> string),
      ("name", user.name |> string),
    ])
  );

let decodeUser = data =>
  Json.Decode.{
    id: data |> field("id", int),
    name: data |> field("name", string),
  };
