[%bs.raw {|require('./ViewSwitcher.css')|}];

let str = ReasonReact.string;

type language = string;

type users = {
  language: string,
  users: list(Leaderboard.item)
};

module Decode = {
  let users = json =>
    Json.Decode.{
      language: json |> field("language", string),
      users: json |> field("users", list(Leaderboard.Decode.item))
    };
};

type state =
  | Language(language);

type action =
  | SelectLanguage(language);

let component = ReasonReact.reducerComponent("ViewSwitcher");

let make = (~users: list(users), ~language="", _children) => {
  ...component,
  initialState: () => Language(language),
  reducer: (action, _state) =>
    switch action {
    | SelectLanguage(language) => ReasonReact.Update(Language(language))
    },
  render: self =>
    <div>
      <div className="tag-language-holder">
        (
          users
          |> List.map(({language}) => language)
          |> List.map(language => {
               let color: string =
                 Color.StringMap.find(language, Color.colors);
               <div
                 className="tag-language"
                 key=language
                 onClick=(_evt => self.send(SelectLanguage(language)))
                 style=(
                   ReactDOMRe.Style.make(
                     ~background=Color.hexToRgbA(color, 0.2),
                     ~color,
                     ()
                   )
                 )>
                 (str(language))
               </div>;
             })
          |> Array.of_list
          |> ReasonReact.array
        )
      </div>
      (
        switch self.state {
        | Language(language) =>
          let {language, users} =
            users
            |> List.filter(({language: currLanguage}) =>
                 currLanguage == language
               )
            |> List.hd;
          <Leaderboard heading=language repos=users />;
        }
      )
    </div>
};