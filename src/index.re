[%bs.raw {|require('./index.css')|}];
[%bs.raw {|require('normalize.css')|}];

[@bs.module "./registerServiceWorker"]
external register_service_worker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(
  <App message="Welcome to React and Reason" />,
  "root",
);

/* Push our current URI to the router after we render the application.
This will persist the current route when you hit refresh. */
ReasonReact.Router.push("");

register_service_worker();
