[%bs.raw {|require('./Profile.css')|}];

let str = ReasonReact.string;

let baseUrl = "http://localhost:5000/analytics/profiles?login=";

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
  | Fetch
  | FetchSuccess(profile)
  | FetchError;

let component = ReasonReact.reducerComponent("Profile");

let make = (~user="", _children) => {
  ...component,
  initialState: () => Loading,
  didMount: self => {
    self.send(Fetch);
  },

  reducer: (action, state) =>
    switch (action) {
    | Fetch => 
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (self => 
          Js.Promise.(
            Fetch.fetch(baseUrl ++ user)
            |> then_(Fetch.Response.json)
            |> then_(json =>
                json 
                |> Decode.profile
                |> (profile => self.send(FetchSuccess(profile)))
                |> resolve
              )
            |> catch(err => 
                err
                |> Js.log
                |> ((_) => self.send(FetchError))
                |> resolve
              )
            |> ignore
          )
        )
      )
    | FetchSuccess(profile) => ReasonReact.Update(Success(profile))
    | FetchError => ReasonReact.Update(Error)
    },
  render: self => {
    switch self.state {
    | Loading => <Loader/>
    | Error => ReasonReact.null
    | Success({stargazersCount, watchersCount, forksCount, topKeywords, topLanguages, matches}) =>
      <div className="profile-holder">

        <span className="tag" title="The total count of stars this user has for all repos">
          <b>(str("Stars: "))</b>
          (str(string_of_int(stargazersCount)))
        </span>
        <span className="tag" title="The total count of watchers this user have">
          <b>(str("Watchers: "))</b>
          (str(string_of_int(watchersCount)))
        </span>
        <span className="tag" title="The total count of forks this userhas for all repos">
          <b>(str("Forks: "))</b>
          (str(string_of_int(forksCount)))
        </span>

        <div>
          <b>(str("Keywords: "))</b>
          (
            topKeywords
            |> List.map(
              ({ word, count }) => 
              <span key=word className="keyword">
              (str(word)) (str({js| × |js})) (str(string_of_int(count)))
              </span>
            )
            |> Array.of_list
            |> ReasonReact.array
          )
        </div>

        <br/>

        <div>
          <b>(str("Languages: "))</b>
          (
            topLanguages
            |> List.map(
              ({ lang, score }) => {
                let color = switch (Color.StringMap.find(lang, Color.colors)) {
                | exception Not_found => "#000000"
                | result => result
                };
                let percentage = string_of_int(int_of_float(score));

                <span key=(lang) className="keyword"
                style=(
                  ReactDOMRe.Style.make(
                    ~background=Color.hexToRgbA(color, 0.2),
                    ~color=color,
                    ()
                  )
                )
                >
                  (str(lang ++ "  " ++ percentage ++ "%"))
                </span>
              }
            )
            |> Array.of_list
            |> ReasonReact.array
          )
        </div>
        <br/>
        <h3>(str("Similar Users"))</h3>
        <div className="matches-holder">
          (
            matches
            |> List.map(({login, score, avatarUrl, htmlUrl}) =>
                <a href=("/users/" ++ login) className="matches-item link" key=login>
                  <div>
                    <img className="matches-item__img" src=(avatarUrl) width="80" height="auto" />
                  </div>
                  <b className="matches-item__name"> (ReasonReact.string(login)) </b>
                </a>
              )
            |> Array.of_list
            |> ReasonReact.array
          )
        </div>
        <br/>
        <br/>
      </div>
    }
  },
};