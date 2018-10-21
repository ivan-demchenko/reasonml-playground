type either('l, 'r) =
  | Left('l)
  | Right('r);

let fold = (hl, hr, elr) =>
  switch (elr) {
  | Left(l) => hl(l)
  | Right(r) => hr(r)
  };

let flatMap = (fn, elr) =>
  switch (elr) {
  | Right(x) => fn(x)
  | Left(x) => Left(x)
  };

let fmap = (fn, x) =>
  switch (x) {
  | Right(x) => Right(fn(x))
  | Left(x) => Left(x)
  };

let bimap = (fl, fr, elr) =>
  switch (elr) {
  | Left(x) => Left(fl(x))
  | Right(x) => Right(fr(x))
  };

let from_option = (errMsg, opt) =>
  switch (opt) {
  | Some(x) => Right(x)
  | None => Left(errMsg)
  };
