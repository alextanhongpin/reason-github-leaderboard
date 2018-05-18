/* Helpers */

let str = ReasonReact.string;

type item = {
  name: string,
};

type state = {
  greeting: string,
  count: int,
  items: list(item),
};

type action =
  | Greet
  | Click;

let component = ReasonReact.reducerComponent("Test");

let make = (~name, _children) => {
  ...component,

  initialState: () => {
    greeting: "hello", 
    count: 0,
    items: [{name: "Hello"}, {name: "World"}],
  },

  /* reducer: (_action, _state) => ReasonReact.NoUpdate, */
  reducer: (action, state) => 
    switch (action) {
    | Click => ReasonReact.Update({...state, count: state.count + 1})
    | _ => ReasonReact.NoUpdate
    },

  render: ({send, state: { count, greeting, items }}) => {
    let countStr = "You've clicked this " ++ string_of_int(count);
    <div>
      (
        items
        |> List.map(
          ({ name }) => <div>(str(name))</div>
        )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
      <button onClick=(_event => send(Click))>(str(countStr)) (str(greeting)) (str(name))</button>
    </div>
  }
};