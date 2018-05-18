/* https://github.com/reasonml-community/reason-react-example/blob/master/src/fetch/FetchExample.re */

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
  avatarUrl: string,
  htmlUrl: string,
};

type profile = {
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
      avatarUrl: json |> field("avatarUrl", string),
      htmlUrl: json |> field("htmlUrl", string),
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

type state = 
  | Loading
  | Error
  | Success(profile);

type action =
  | FetchProfile
  | FetchProfileSuccess(profile)
  | FetchProfileError;

let component = ReasonReact.reducerComponent("Profile");

let make = (~user="", _children) => {
  ...component,
  initialState: () => Loading,
  didMount: self => {
    self.send(FetchProfile);
  },
  reducer: (action, state) =>
    switch (action) {
    | FetchProfile => 
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (self => 
          Js.Promise.(
            Fetch.fetch("http://localhost:5000/analytics/profiles?login=" ++ user)
            |> then_(Fetch.Response.json)
            |> then_(json =>
                json 
                |> Decode.profile
                |> (profile => self.send(FetchProfileSuccess(profile)))
                |> resolve
              )
            |> catch(err => 
                err
                |> Js.log
                |> ((_) => self.send(FetchProfileError))
                |> resolve
              )
            |> ignore
          )
        )
      )
    | FetchProfileSuccess(profile) => ReasonReact.Update(Success(profile))
    | FetchProfileError => ReasonReact.Update(Error)
    },
  render: self => {
    switch self.state {
    | Loading => <Loader/>
    | Error => <div>(ReasonReact.string("Error occured"))</div>
    | Success({login, totalCount, stargazersCount, matches}) =>
      <div>
        <h3> (ReasonReact.string(login)) </h3>
          <div>
            (ReasonReact.string("totalCount:"))
            <b> (ReasonReact.string(string_of_int(totalCount))) </b>
          </div>
          <div>
            (ReasonReact.string("stargazersCount:"))
            <b> (ReasonReact.string(string_of_int(stargazersCount))) </b>
          </div>
          (
            matches
            |> List.map(({login, score, avatarUrl, htmlUrl}) =>
                <div>
                  <img src=(avatarUrl) width="30" height="auto" />
                  <a href=htmlUrl> (ReasonReact.string(login)) </a>
                </div>
              )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
      </div>
    }
  },
};