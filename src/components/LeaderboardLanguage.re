[%bs.raw {|require('./LeaderboardLanguage.css')|}];

let baseUrl = "http://localhost:5000/analytics?type=leaderboard_languages";

type language = {
  score: int,
  lang: string
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
      lang: json |> field("lang", string)
    };
  let counter = json =>
    Json.Decode.{
      analyticType: json |> field("type", string),
      languages: json |> field("languages", list(language)),
      createdAt: json |> field("createdAt", string),
      updatedAt: json |> field("updatedAt", string)
    };
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
  | Success(counter);

type action =
  | Fetch
  | FetchSuccess(counter)
  | FetchError;

let component = ReasonReact.reducerComponent("LeaderboardLanguage");

let make = (~heading="", ~subheading="", _children) => {
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
    | Error => <Error/>
    | Success({analyticType, languages, createdAt, updatedAt}) =>
      let max = languages |> List.map(({score}) => score) |> max_list;
      let min = languages |> List.map(({score}) => score) |> min_list;
      <div>
        <h1 className="leaderboard-language__heading"> (str(heading)) </h1>
        <p className="leaderboard-language__last-updated">
          (str("Last updated " ++ Date.parseDate(updatedAt)))
        </p>
        <p className="leaderboard-language__subheading"> (str(subheading)) </p>
        <div className="leaderboard-language__list">
          (
            languages
            |> List.map(({score, lang}) => {
                 let width =
                   ceil(float_of_int(score) /. float_of_int(max) *. 100.);
                 <div className="leaderboard-language__item" key=lang>
                   <div>
                     <span className="leaderboad-language__item-language">
                       (str(lang))
                     </span>
                     <span className="leaderboard-language__item-count">
                       (str(string_of_int(score)))
                     </span>
                   </div>
                   <span
                     className="progress"
                     style=(
                       ReactDOMRe.Style.make(
                         ~width=string_of_float(width) ++ "%",
                         ~background=Color.StringMap.find(lang, Color.colors),
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
    }
};