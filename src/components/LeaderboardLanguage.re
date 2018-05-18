let baseUrl = "http://localhost:5000/analytics?type=leaderboard_languages";

type language = {
  score: int,
  lang: string,
};

type counter = {
  analyticType: string,
  languages: list(language),
  createdAt: string,
  updatedAt: string
};

module Decode = {
  let language = json =>
    Json.Decode.{
      score: json |> field("score", int),
      lang: json |> field("lang", string),
    };

  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      languages: json |> field("languages", list(language)),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
};

let str = ReasonReact.string;

type state =
  | Loading
  | Error
  | Success(counter);

type action =
  | Fetch
  | FetchSuccess(counter)
  | FetchError;

let component = ReasonReact.reducerComponent("LeaderboardLanguage");

let make = _children => {
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
                   |> Decode.counter
                   |> (counter => self.send(FetchSuccess(counter)))
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
    | Error => <div> (str("Error")) </div>
    | Success({analyticType, languages, createdAt, updatedAt}) =>
      <div>
        <h2>(str("Top languages:"))</h2>
        (
          languages
          |> List.map(
            ({ score, lang }) => 
              <div key=lang>
                <b>(str(lang))</b>
                <div>(str(string_of_int(score)))</div>
              </div>
          )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    }
};
 