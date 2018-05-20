[%bs.raw {|require('./ViewSwitcher.css')|}];

let str = ReasonReact.string;

type language = string;

type repo = {
  lang: string,
  repos: list(Leaderboard.item)
};

module Decode = {
  let repo = json =>
    Json.Decode.{
      lang: json |> field("lang", string),
      repos: json |> field("top", list(Leaderboard.Decode.item))
    };
};

type state =
  | Language(language);

type action =
  | SelectLanguage(language);

let component = ReasonReact.reducerComponent("ViewSwitcher");

let make = (~repos: list(repo), ~language="", _children) => {
  ...component,
  initialState: () => Language(language),
  reducer: (action, _state) =>
    switch action {
    | SelectLanguage(language) => ReasonReact.Update(Language(language))
    },
  render: self =>
  
    <div>
      <div className="tag-language-holder">(
        repos
        |> List.map(({lang}) => lang)
        |> List.map(lang => {
            let color: string = Color.StringMap.find(lang, Color.colors);
             <div
               className="tag-language"
               key=lang
               onClick=(_evt => self.send(SelectLanguage(lang)))
               style=(
                 ReactDOMRe.Style.make(
                   ~background=Color.hexToRgbA(color, 0.2),
                   ~color=color,
                   ()
                 )
               )>
               (str(lang))
             </div>
        }
           )
        |> Array.of_list
        |> ReasonReact.array
      )</div>
      (
        switch self.state {
        | Language(language) =>
          let {lang, repos} =
            repos |> List.filter(({lang}) => lang == language) |> List.hd;
          <Leaderboard heading=lang repos />;
        }
      )
    </div>
};