type language = {
  lang: string,
  score: float,
  count: int,
};

type keyword = {
  word: string,
  count: int,
};

type match = {
  login: string,
  score: float,
};

type state = {
  login: string,
  totalCount: int,
  topLanguages: list(language),
  topKeywords: list(keyword),
  stargazersCount: int,
  watchersCount: int,
  forksCount: int,
  matches: list(match),
};

module Decode = {
  let language = json => 
    Json.Decode.{
      lang: json |> field("lang", string),
      score: json |> field("score", float),
      count: json |> field("count", int),
    };

  let keyword = json => 
    Json.Decode.{
      word: json |> field("word", string),
      count: json |> field("count", int),
    };

  let match = json => 
    Json.Decode.{
      login: json |> field("login", string),
      score: json |> field("score", float),
    };

  let profile = json =>
    Json.Decode.{
      login: json |> field("login", string),
      totalCount: json |> field("totalCount", int),
      topLanguages: json |> field("topLanguages", list(language)),
      topKeywords: json |> field("topKeywords", list(keyword)),
      stargazersCount: json |> field("stargazersCount", int),
      watchersCount: json |> field("watchersCount", int),
      forksCount: json |> field("forksCount", int),
      matches: json |> field("matches", list(match)),
    };
};

/* NoOp */
type action =
  | FetchProfile(state)
  | NoOp;

let component = ReasonReact.reducerComponent("Profile");

let make = (~name="hello", _children) => {
  ...component,

  initialState: () => {
    login: "",
    totalCount: 0,
    topLanguages: [],
    topKeywords: [],
    stargazersCount: 0,
    watchersCount: 0,
    forksCount: 0,
    matches: [],
  },

  didMount: (self) => {
    Js.Promise.(
      Fetch.fetch("http://localhost:5000/analytics/profiles?login=alextanhongpin")
      |> then_(Fetch.Response.json)
      |> then_(json => {
        let data = json
        |> Decode.profile;

        self.send(FetchProfile(data));
        Js.Promise.resolve();
      })
    ) |> ignore;
  },

  reducer: (action, state) =>
    switch action {
    | FetchProfile(result) => ReasonReact.Update(result)
    | NoOp => ReasonReact.NoUpdate
    },

  render: ({ state: {
    login, 
    totalCount, 
    stargazersCount
  }}) => 
    <div>
      <h3>(ReasonReact.string(login))</h3>

      <div>
        (ReasonReact.string("totalCount:"))
        <b>(ReasonReact.string(string_of_int(totalCount)))</b>
      </div>

      <div>
        (ReasonReact.string("stargazersCount:"))
        <b>(ReasonReact.string(string_of_int(stargazersCount)))</b>
      </div>
    </div>
};