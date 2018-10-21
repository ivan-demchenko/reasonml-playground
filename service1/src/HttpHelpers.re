open Express;

let makeSuccessJson = (id, name) => User.(create(id, name) |> encodeUser);

let makeErrorJson = (error_type, message) =>
  Utils.UserFacingError.make(error_type, message)
  |> Utils.UserFacingError.toJson;

let statusNotFound = Response.StatusCode.NotFound |> Response.status;

let statusBadRequest = Response.StatusCode.BadRequest |> Response.status;

let sendJson = Response.sendJson;
