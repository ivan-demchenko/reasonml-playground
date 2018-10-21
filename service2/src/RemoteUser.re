type remoteUser = {
  id: string,
  name: string,
};

let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
  };

let present = user =>
  String.concat(": ", ["Here we go", user.id, user.name]);
