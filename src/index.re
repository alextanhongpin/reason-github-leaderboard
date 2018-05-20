[%bs.raw {|require('./index.css')|}];
[%bs.raw {|require('normalize.css')|}];

let setupSmoothscroll: (_) => (_) = [%bs.raw {|
  function () {
    const smoothscroll = require('smoothscroll-polyfill')
    smoothscroll.polyfill()
  }
|}];

setupSmoothscroll();


[@bs.module "./registerServiceWorker"]
external register_service_worker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(
  <App/>,
  "root",
);

/* Push our current URI to the router after we render the application.
This will persist the current route when you hit refresh. */
ReasonReact.Router.push("");

register_service_worker();
