[%bs.raw {|require('./LeaderboardLanguage.css')|}];

type language = {
  count: int,
  name: string
};

type data = {
  analyticType: string,
  languages: list(language),
  createdAt: string,
  updatedAt: string
};

type response = {data: option(data)};

module Decode = {
  let language = json =>
    Json.Decode.{
      count: json |> field("count", int),
      name: json |> field("name", string)
    };
  let data = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      languages: json |> field("languages", list(language)),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
  let response = json =>
    Json.Decode.{data: json |> optional(field("data", data))};
};

let rec max_list = list =>
  switch list {
  | [] => 0
  | [head] => head
  | [head, ...rest] => max(head, max_list(rest))
  };

let rec min_list = list =>
  switch list {
  | [] => 0
  | [head] => head
  | [head, ...rest] => min(head, min_list(rest))
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

let component = ReasonReact.reducerComponent("LeaderboardLanguage");

let make = (~baseUrl, ~heading="", ~subheading="", _children) => {
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
    | FetchSuccess(counter) => ReasonReact.Update(Success(counter))
    | FetchError => ReasonReact.Update(Error)
    },
  render: self =>
    switch self.state {
    | Loading => <Loader />
    | Error => <Error />
    | Success(response) =>
      switch response.data {
      | Some({analyticType, languages, createdAt, updatedAt}) =>
        let max = languages |> List.map(({count}) => count) |> max_list;
        let min = languages |> List.map(({count}) => count) |> min_list;
        <div>
          <h1 className="leaderboard-language__heading"> (str(heading)) </h1>
          <p className="leaderboard-language__last-updated">
            (str("Last updated " ++ Date.parseDate(updatedAt)))
          </p>
          <p className="leaderboard-language__subheading">
            (str(subheading))
          </p>
          <div className="leaderboard-language__list">
            (
              languages
              |> List.map(({count, name}) => {
                   let width =
                     ceil(float_of_int(count) /. float_of_int(max) *. 100.);
                   <div className="leaderboard-language__item" key=name>
                     <div>
                       <span className="leaderboad-language__item-language">
                         (str(name))
                       </span>
                       <span className="leaderboard-language__item-count">
                         (str(string_of_int(count)))
                       </span>
                     </div>
                     <span
                       className="progress"
                       style=(
                         ReactDOMRe.Style.make(
                           ~width=string_of_float(width) ++ "%",
                           ~background=Color.getColor(name),
                           ()
                         )
                       )
                     />
                   </div>;
                 })
              |> Array.of_list
              |> ReasonReact.array
            )
          </div>
        </div>;
      | None => ReasonReact.null
      }
    }
};