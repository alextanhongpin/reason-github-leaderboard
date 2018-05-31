let toLocaleString: int => string = [%bs.raw
  {|
    function (number) {
      return number.toLocaleString()
    }
  |}
];

type data = {
  analyticType: string,
  count: int,
  createdAt: string,
  updatedAt: string
};

type response = {data: option(data)};

module Decode = {
  let data = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      count: json |> field("count", int),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
  let response = json =>
    Json.Decode.{data: json |> optional(field("data", data))};
};

let str = ReasonReact.string;

type state =
  | Loading
  | Error
  | Success(response);

type action =
  | Fetch
  | FetchSuccess(response)
  | FetchError;

let component = ReasonReact.reducerComponent("UserCounter");

let make = (~baseUrl, _children) => {
  ...component,
  didMount: self => self.send(Fetch),
  initialState: () => Loading,
  reducer: (action, _state) =>
    switch action {
    | Fetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Fetch.fetch(baseUrl)
              |> then_(Fetch.Response.json)
              |> then_(json =>
                   json
                   |> Decode.response
                   |> (response => self.send(FetchSuccess(response)))
                   |> resolve
                 )
              |> catch(err =>
                   err |> Js.log |> ((_) => self.send(FetchError)) |> resolve
                 )
              |> ignore
            )
        )
      )
    | FetchSuccess(response) => ReasonReact.Update(Success(response))
    | FetchError => ReasonReact.Update(Error)
    },
  render: self =>
    switch self.state {
    | Loading => <span> (str("0")) </span>
    | Error => <span> (str("0")) </span>
    | Success(response) =>
      switch response.data {
      | Some(data) => <span> (str(toLocaleString(data.count))) </span>
      | None => <span> (str("0")) </span>
      }
    }
};