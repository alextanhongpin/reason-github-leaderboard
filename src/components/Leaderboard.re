[%bs.raw {|require('./Leaderboard.css')|}];

type item = {
  count: int,
  login: string,
  avatarUrl: option(string)
};

module Decode = {
  let item = json =>
    Json.Decode.{
      count: json |> field("count", int),
      login: json |> field("login", string),
      avatarUrl: json |> optional(field("avatarUrl", string))
    };
};

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Leaderboard");

let make = (~heading, ~repos, _children) => {
  ...component,
  render: _self =>
    <div>
      <h2> (str(heading)) </h2>
      <div className="leaderboard-holder">
        (
          repos
          |> List.mapi((i, {count, login, avatarUrl}: item) => {
               let usersHref = "/users/" ++ login;
               <div className="leaderboard-item" key=login>
                 <span className="leaderboard-item__rank">
                   (str(string_of_int(i + 1) ++ "."))
                 </span>
                 (
                   switch avatarUrl {
                   | Some(src) => <img src width="20" height="auto" />
                   | None => ReasonReact.null
                   }
                 )
                 <span className="leaderboard-item__name">
                   <Link href=usersHref> (str(login)) </Link>
                 </span>
                 <a href=("https://github.com/" ++ login)>
                   (str(string_of_int(count) ++ " repos"))
                 </a>
               </div>;
             })
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    </div>
};