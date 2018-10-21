open Express;

let logging =
  Middleware.from((next, req) => {
    let time =
      Js.Date.now() |> Js.Date.fromFloat |> Js.Date.toLocaleDateString;
    Js.log(
      String.concat(
        " :: ",
        [time, Request.methodRaw(req), Request.path(req)],
      ),
    );
    next(Next.middleware);
  });

let sendHtml = text => Middleware.from((_, _) => Response.sendString(text));
